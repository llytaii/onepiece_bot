#include "websites.hpp"
#include "bot.hpp"

#include "HTTPRequest.hpp"

#include <fstream>

std::string Website::get_html(const std::string &_url)
{
    http::Response response = http::Request{_url}.send("GET");
    return std::string{response.body.begin(), response.body.end()};
}

Website::Website(Bot *_bot) : m_bot{_bot}
{
}

// ChapterOnePieceTube
bool ChapterOnePieceTube::new_x_found_in(const std::string &_html)
{
    if (_html.find("/kapitel/" + std::to_string(m_next_chapter)) != std::string::npos)
    {
        std::fstream f{"./res/chapter.data", std::ios::out};

        if (!f.is_open())
            return false;
            
        f << (m_next_chapter + 1);
        f.close();
        return true;
    }
    return false;
}

std::string ChapterOnePieceTube::get_notification_message()
{
    return std::string{"https://onepiece-tube.com/kapitel/" + std::to_string(m_next_chapter) + "/1"};
}

ChapterOnePieceTube::ChapterOnePieceTube(Bot *_bot) : Website{_bot}
{
    std::fstream f{"./res/chapter.data", std::ios::in};
    if(!f.is_open()) return;
    f >> m_next_chapter;
    f.close();

    if(new_x_found_in(get_html("http://onepiece-tube.com/kapitel-mangaliste")))
    {
        if(m_bot)
            m_bot->notify_all(get_notification_message());
    }
}

// EpisodeOnePieceTube
bool EpisodeOnePieceTube::new_x_found_in(const std::string &_html)
{
    if (_html.find("/folge/" + std::to_string(m_next_episode)) != std::string::npos)
    {
        std::fstream f{"./res/episode.data", std::ios::out};

        if (!f.is_open())
            return false;

        f << (m_next_episode + 1);
        f.close();
        return true;
    }
    return false;
}

std::string EpisodeOnePieceTube::get_notification_message()
{
    return std::string{"https://onepiece-tube.com/folge/" + std::to_string(m_next_episode)};
}

EpisodeOnePieceTube::EpisodeOnePieceTube(Bot *_bot) : Website{_bot}
{
    std::fstream f{"./res/episode.data", std::ios::in};

    if(!f.is_open()) return;
    f >> m_next_episode;
    f.close();

    if(new_x_found_in(get_html("http://onepiece-tube.com/episoden-streams")))
    {
        if(m_bot)
            m_bot->notify_all(get_notification_message());
    }
}