#pragma once

#include "database.hpp"

#define TELEGRAM_NO_LISTENER_FCGI
#include "libtelegram/libtelegram.h"

#include <array>
#include <functional>
#include <string>
#include <tuple>

enum LogLevel
{
    ERROR,
    INFO
};

class Database;

class Bot   // wraps the libtelegram bot components and implements commands/functions
{
public:
    Bot();
    ~Bot();

    bool init();
    void run();

    void send_message(const std::string _message, const int_fast64_t _id);
    void notify_all(const std::string _message);
    bool check_user_state(const int_fast64_t _id);

    void log(const std::string _message, LogLevel _level);


private:
    telegram::sender *m_sender{nullptr};
    telegram::listener::poll *m_listener{nullptr};

    Database m_database{this};
    int_fast64_t m_admin_id;

    std::string m_token;

    // bot user commands; every command is a function that takes in the users id and message
    #define CMD_ARGS const int_fast64_t _id, const std::string &_msg

    void start_cmd(CMD_ARGS);
    void end_cmd(CMD_ARGS);
    void help_cmd(CMD_ARGS);
    void status_cmd(CMD_ARGS);
    void announce_cmd(CMD_ARGS);
    void list_user_cmd(CMD_ARGS);
    void source_cmd(CMD_ARGS);

    // map the command-strings to the matching function using a pair, 
    // where pair.first is the commandname (e.g. "/start") and pair.second a lambda that calls the function
    using CommandTable = std::pair<std::string, std::function<void(CMD_ARGS)>>;

    std::array<CommandTable, 7> m_command_table{
        std::make_pair(std::string{"/start"},       [this](CMD_ARGS) { this->start_cmd(_id, _msg); }),
        std::make_pair(std::string{"/end"},         [this](CMD_ARGS) { this->end_cmd(_id, _msg); }),
        std::make_pair(std::string{"/help"},        [this](CMD_ARGS) { this->help_cmd(_id, _msg); }),
        std::make_pair(std::string{"/status"},      [this](CMD_ARGS) { this->status_cmd(_id, _msg); }),
        std::make_pair(std::string{"/announce"},    [this](CMD_ARGS) { this->announce_cmd(_id, _msg); }),
        std::make_pair(std::string{"/list_user"},   [this](CMD_ARGS) { this->list_user_cmd(_id, _msg); }),
        std::make_pair(std::string{"/source"},      [this](CMD_ARGS) { this->source_cmd(_id, _msg); })
        };
};
