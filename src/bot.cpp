#include "bot.hpp"
#include "logger.hpp"

#include <chrono>
#include <fstream>
#include <httplib.h>
#include <json.hpp>
#include <string>
#include <sstream>
#include <thread>

Bot::Bot() : m_fetcher{this}
{
}

Bot::~Bot()
{
    delete m_sender;
    delete m_listener;
}

bool Bot::init()
{
    if (m_sender || m_listener)
    {
        Logger::log("Bot::init(): sender || listener not nullptr!", LOG::ERROR);
        return false;
    }

    std::fstream bot_data{get_res_path() /= "bot.data", std::ios::in};
    if (!bot_data.is_open())
    {
        Logger::log("Bot::init(): opening bot.data failed!", LOG::ERROR);
        return false;
    }

    bot_data >> m_token >> m_admin_id;

    m_sender = new telegram::sender{m_token};
    Logger::set_log_bot(this);
    m_listener = new telegram::listener::poll{*m_sender};

    m_listener->set_callback_message([this](const telegram::types::message &message)
                                     {
                                         if (!message.text)
                                             return;

                                         auto chat_id = message.chat.id;

                                         // extract first word (the command) from message
                                         std::stringstream msg{*message.text};
                                         std::string cmd{};
                                         msg >> cmd;
                                         for (auto i : m_command_table)
                                         {
                                             if (cmd == i.first)
                                             {
                                                 i.second(chat_id, msg.str());
                                                 break;
                                             }
                                         }
                                     });

    if (!m_database.init())
    {
        Logger::log("Bot::init(): initializing database failed!", LOG::ERROR);
        return false;
    }

    return true;
}

void Bot::run()
{
    std::thread t{&Fetcher::run, &m_fetcher};

    if (m_listener)
    {
        Logger::log("Bot::run(): start running..", LOG::INFO);
        m_listener->run();
    }
    t.join();
}

void Bot::send_message(const std::string _message, const int_fast64_t _id) const
{
    if (m_sender)
        m_sender->send_message(_id, _message);
}

void Bot::send_message_to_all_users(const std::string _message)
{
    for (auto u : m_database.get_users())
        if (user_has_chat(u))
            send_message(_message, u);
}

void Bot::send_message_to_admin(const std::string _message) const
{
    send_message(_message, m_admin_id);
}

bool Bot::user_has_chat(const int_fast64_t _id) const
{
    nlohmann::json tree;
    tree["chat_id"] = _id;
    tree["action"] = "typing";

    std::string addr{"https://api.telegram.org/bot" + m_token + "/" + "sendChatAction"};
    const urdl::url url{addr};
    httplib::SSLClient http_client(url.host().c_str(), url.port(), 30);

    auto http_result{http_client.Post((url.path() + "?" + url.query()).c_str(), tree.dump().c_str(), "application/json")};

    if (http_result->status == 403) //403 == forbidden == has no chat with bot
    {
        Logger::log("Bot::user_has_chat(): return false for id " + std::to_string(_id), LOG::WARNING);
        return false;
    }

    return true;
}

// Commands:

void Bot::start_cmd(const int_fast64_t _id, const std::string &_msg)
{
    std::string answer;

    if (m_database.add_user(_id))
        answer = "welcome!\n /help for more infos";
    else
        answer = "saving id " + std::to_string(_id) + " failed!\n/status for more information";

    send_message(answer, _id);
}

void Bot::end_cmd(const int_fast64_t _id, const std::string &_msg)
{
    std::string answer;

    if (m_database.remove_user(_id))
        answer = "deleted id " + std::to_string(_id);
    else
        answer = "deleting id " + std::to_string(_id) + " failed\n/status for more information";

    send_message(answer, _id);
}

void Bot::status_cmd(const int_fast64_t _id, const std::string &_msg)
{
    if (m_database.has_user(_id))
        send_message("your id is saved, you will get notified.", _id);
    else
        send_message("your id wasnt found!", _id);
}

void Bot::source_cmd(const int_fast64_t _id, const std::string &_msg) const
{
    send_message("https://github.com/llytaii/onepiece_bot", _id);
}

void Bot::help_cmd(const int_fast64_t _id, const std::string &_msg) const
{
    std::string answer;

    answer += "id commands:\n";
    answer += "/start : saves your id for notifications\n";
    answer += "/end : deletes your id\n";
    answer += "/status : show status of your id\n";
    answer += "\ninfo commands:\n";
    answer += "/source : link to source code\n";
    answer += "/help: list commands\n";
    answer += "/chapter: get link to latest chapter\n";
    answer += "/episode: get link to latest episode\n";

    if (_id == m_admin_id)
    {
        answer += "\nadmin commands:\n";
        answer += "/announce\n";
        answer += "/list_user\n";
    }

    send_message(answer, _id);
}

void Bot::chapter_cmd(const int_fast64_t _id, const std::string &_msg)
{
    send_message(m_fetcher.get_current_chapter_link(), _id);
}

void Bot::episode_cmd(const int_fast64_t _id, const std::string &_msg)
{
    send_message(m_fetcher.get_current_episode_link(), _id);
}

void Bot::announce_cmd(const int_fast64_t _id, const std::string &_msg)
{
    // TODO nicer method
    if (_id == m_admin_id)
    {
        std::stringstream ss{_msg};
        std::string str;
        std::string msg;

        ss >> str; // remove command at the start
        while (ss >> str)
            msg += str + ' ';

        for (auto e : m_database.get_users())
            send_message(msg, e);
    }
}

void Bot::list_user_cmd(const int_fast64_t _id, const std::string &_msg)
{
    if (_id == m_admin_id)
    {
        for (auto e : m_database.get_users())
            send_message(std::to_string(e), _id);
    }
}