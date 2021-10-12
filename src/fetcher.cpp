#include "fetcher.hpp"
#include "bot.hpp"
#include "logger.hpp"

#include "HTTPRequest.hpp"

#include <fstream>

Fetcher::Fetcher(Bot *_bot) : m_bot{_bot}
{
}

void Fetcher::run()
{
    Logger::log("Fetcher::run(): start running..", LOG::INFO);
    while (true)
    {
        if (!read())
            return; // TODO: Exceptions!

        if (new_episode_found() && m_bot != nullptr)
            m_bot->send_message_to_all_users(get_current_episode_link());

        if (new_chapter_found() && m_bot != nullptr)
            m_bot->send_message_to_all_users(get_current_chapter_link());
            
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
}

bool Fetcher::read()
{
    std::fstream file{get_res_path() /= "fetcher.data", std::ios::in};
    if (!file.is_open())
    {
        Logger::log("Database::read(): opening fetcher.data failed!", LOG::ERROR);
        return false;
    }
    file >> m_next_chapter >> m_next_episode;
    return true;
}

void Fetcher::write()
{
    std::fstream file{get_res_path() /= "fetcher.data", std::ios::out};
    if (!file.is_open())
        Logger::log("Database::write(): opening fetcher.data failed!", LOG::ERROR);
    file << m_next_chapter << ' ' << m_next_episode;
}

std::string Fetcher::get_html(const std::string &_url) const
{
    http::Response response = http::Request{_url}.send("GET");
    Logger::log("Fetcher::get_html(): fetched " + _url + ", status code: " + std::to_string(response.status), LOG::TRACE);
    return std::string{response.body.begin(), response.body.end()};
}

bool Fetcher::new_chapter_found()
{
    std::string html{get_html("http://onepiece-tube.com/kapitel-mangaliste")};

    if (html.find("/kapitel/" + std::to_string(m_next_chapter)) != std::string::npos)
    {
        Logger::log("Fetcher::new_chapter_cound(): return true!", LOG::TRACE);
        ++m_next_chapter;
        write();
        return true;
    }
    return false;
}

bool Fetcher::new_episode_found()
{
    std::string html{get_html("http://onepiece-tube.com/episoden-streams")};

    if (html.find("/folge/" + std::to_string(m_next_episode)) != std::string::npos)
    {
        Logger::log("Fetcher::new_chapter_cound(): return false!", LOG::TRACE);
        ++m_next_episode;
        write();
        return true;
    }
    return false;
}

std::string Fetcher::get_current_chapter_link() const
{
    return std::string{"https://onepiece-tube.com/kapitel/" + std::to_string(m_next_chapter - 1) + "/1"};
}

std::string Fetcher::get_current_episode_link() const
{
    return std::string{"https://onepiece-tube.com/folge/" + std::to_string(m_next_episode - 1)};
}