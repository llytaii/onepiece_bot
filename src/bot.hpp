#pragma once
#include "base.hpp"
#include "database.hpp"
#include "fetcher.hpp"

#define TELEGRAM_NO_LISTENER_FCGI
#include "libtelegram/libtelegram.h"

#include <array>
#include <functional>
#include <string>
#include <tuple>

class Bot : public Base
{
public:
    Bot();
    ~Bot();

    bool init();
    void run();

    void send_message(const std::string _message, const int_fast64_t _id) const;
    void send_message_to_all_users(const std::string _message);
    void send_message_to_admin(const std::string _message) const;
    bool user_has_chat(const int_fast64_t _id) const;

private:
    // bot user commands; every command is a function that takes in the users id and message
    #define CMD_ARGS const int_fast64_t _id, const std::string &_msg

    // user cmds
    void start_cmd(CMD_ARGS);
    void end_cmd(CMD_ARGS);
    void status_cmd(CMD_ARGS);

    // info cmds
    void source_cmd(CMD_ARGS) const;
    void help_cmd(CMD_ARGS) const;
    void chapter_cmd(CMD_ARGS);
    void episode_cmd(CMD_ARGS);

    // admin cmds
    void announce_cmd(CMD_ARGS);
    void list_user_cmd(CMD_ARGS);

    // map the command-strings to the matching function using a pair, 
    // where pair.first is the commandname (e.g. "/start") and pair.second a lambda that calls the function
    using CommandTable = std::pair<std::string, std::function<void(CMD_ARGS)>>;

    std::array<CommandTable, 9> m_command_table{
        // user cmds
        std::make_pair(std::string{"/start"},       [this](CMD_ARGS) { this->start_cmd(_id, _msg); }),
        std::make_pair(std::string{"/end"},         [this](CMD_ARGS) { this->end_cmd(_id, _msg); }),
        std::make_pair(std::string{"/status"},      [this](CMD_ARGS) { this->status_cmd(_id, _msg); }),
        // info cmds 
        std::make_pair(std::string{"/source"},      [this](CMD_ARGS) { this->source_cmd(_id, _msg); }),
        std::make_pair(std::string{"/help"},        [this](CMD_ARGS) { this->help_cmd(_id, _msg); }),
        std::make_pair(std::string{"/chapter"},     [this](CMD_ARGS) { this->chapter_cmd(_id, _msg); }),
        std::make_pair(std::string{"/episode"},     [this](CMD_ARGS) { this->episode_cmd(_id, _msg); }),
        // admin cmds
        std::make_pair(std::string{"/announce"},    [this](CMD_ARGS) { this->announce_cmd(_id, _msg); }),
        std::make_pair(std::string{"/list_user"},   [this](CMD_ARGS) { this->list_user_cmd(_id, _msg); }),
        };

private:
    telegram::sender *m_sender{nullptr};
    telegram::listener::poll *m_listener{nullptr};
    
    std::string m_token;
    int_fast64_t m_admin_id;

    Database m_database;
    Fetcher m_fetcher{nullptr};
};
