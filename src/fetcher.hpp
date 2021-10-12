#pragma once
#include "base.hpp"

#include <string>

class Bot;

class Fetcher : Base
{
    public:
        Fetcher(Bot* _bot);
        void run();

        std::string get_current_chapter_link() const;
        std::string get_current_episode_link() const;

    private:
        bool read(); // read from fetcher.data
        void write(); // write to fetcher.data

        std::string get_html(const std::string &_url) const;
        bool new_chapter_found();
        bool new_episode_found();

        int m_next_chapter{-1};
        int m_next_episode{-1};

        Bot* m_bot{nullptr};
};