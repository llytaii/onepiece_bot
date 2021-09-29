#pragma once

#include <string>
#include <tuple>

class Bot;

class Website
{
    public:
        explicit Website(Bot *_bot);


    protected:
        Bot *const m_bot{nullptr};
        std::string get_html(const std::string &_url);

};


class ChapterOnePieceTube : public Website
{
    public:
        explicit ChapterOnePieceTube(Bot *_bot);

    private:
        bool new_x_found_in(const std::string &_hmtl);
        std::string get_notification_message();

        int m_next_chapter;
};


class EpisodeOnePieceTube: public Website
{
    public:
        explicit EpisodeOnePieceTube(Bot *_bot);

    private:
        bool new_x_found_in(const std::string &_hmtl);
        std::string get_notification_message();

        int m_next_episode;
};