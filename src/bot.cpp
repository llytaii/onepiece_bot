#include "bot.hpp"

#include <fstream>
#include <httplib.h>
#include <json.hpp>
#include <string>
#include <sstream>

Bot::Bot()
{}

Bot::~Bot()
{
    delete m_sender;
    delete m_listener;
}

bool Bot::init()
{
    std::cout << "Starting init()..\n";
    if (m_sender || m_listener)
        return false;

    std::cout << "Reading bot.data..\n";
    std::fstream bot_data{"./res/bot.data"};
    if (!bot_data.is_open())
        return false;

    bot_data >> m_token >> m_admin_id;

    std::cout << "Creating sender/listener..\n";
    m_sender = new telegram::sender{m_token};
    m_listener = new telegram::listener::poll{*m_sender};

    std::cout << "Setting callback message..\n";
    m_listener->set_callback_message([this](const telegram::types::message &message) {
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

    std::cout << "Initialising Database..\n";
    if (!m_database.init())
    {
        log("Initialization failed! m_database.init() returned false! Aborting!", LogLevel::ERROR);
        return false;
    }

    log("Initialization finished successfully!", LogLevel::INFO);
    return true;
}

void Bot::run()
{
    std::cout << "Start running..\n";
    if (m_listener)
        m_listener->run();
}

void Bot::send_message(const std::string _message, const int_fast64_t _id)
{
    if (m_sender)
        m_sender->send_message(_id, _message);
}

void Bot::notify_all(const std::string _message)
{
    for (auto e : m_database.get_users())
    {
        if(check_user_state(e))
            send_message(_message, e);
    }
}

bool Bot::check_user_state(const int_fast64_t _id)
{
    nlohmann::json tree;
    tree["chat_id"] = _id;
    tree["action"] = "typing";

    std::string addr{"https://api.telegram.org/bot" + m_token + "/" + "sendChatAction"};
    const urdl::url url{addr};
    httplib::SSLClient http_client(url.host().c_str(), url.port(), 30);

    auto http_result{http_client.Post((url.path() + "?" + url.query()).c_str(), tree.dump().c_str(), "application/json")};

    if(http_result->status == 403)
    {
        log("ID " + std::to_string(_id) + " failed active user check and is skipped!", LogLevel::ERROR);
        return false;
    }
    return true;
}

void Bot::log(const std::string _message, LogLevel _level)
{
    if (m_sender)
    {
        std::string msg;
        msg += (_level == LogLevel::ERROR) ? "[ERROR]: " : "[INFO]: ";
        msg += _message;
        m_sender->send_message(m_admin_id, msg);
    }
}


// Commands:

void Bot::start_cmd(const int_fast64_t _id, const std::string &_msg)
{
    std::string answer;

    if (m_database.save_user(_id))
    {
        answer = "Welcome to the OnePieceNotifyBot.\nSee /help for more info.";
    }
    else
    {
        answer = "There was a problem saving your ID!";
        log("Failed to save ID: " + std::to_string(_id), LogLevel::ERROR);
    }
    send_message(answer, _id);
}

void Bot::end_cmd(const int_fast64_t _id, const std::string &_msg)
{
    std::string answer;

    if (m_database.delete_user(_id))
    {
        answer = "Your ID was deleted, you wont be bothered anymore.";
    }
    else
    {
        answer = "There was a problem deleting your ID!";
        log("Failed to delete ID: " + std::to_string(_id), LogLevel::ERROR);
    }

    send_message(answer, _id);
}

void Bot::help_cmd(const int_fast64_t _id, const std::string &_msg)
{
    std::string answer;

    answer += "/start : saves your id for notifications\n";
    answer += "/end : deletes your id\n";
    answer += "/source : link to source code\n";
    answer += "/status : show bot status\n";
    
    send_message(answer, _id);
}

void Bot::status_cmd(const int_fast64_t _id, const std::string &_msg)
{
    send_message("Online!", _id);
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
            log(std::to_string(e), LogLevel::INFO);
    }
}

void Bot::source_cmd(const int_fast64_t _id, const std::string &_msg)
{
    send_message("https://github.com/llytaii/onepiece_bot", _id);
}