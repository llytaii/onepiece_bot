#pragma once
#include "base.hpp"
#include "bot.hpp"

#include <chrono>
#include <ctime>
#include <fstream>
#include <string>

class Bot;

enum class LOG
{
    TRACE,
    INFO,
    WARNING,
    ERROR,
    MAX
};

class Logger : public Base
{
public:
    static void write(const std::string &_log_message)
    {
        std::fstream file{Base::get_res_path() /= "log.txt", std::ios::app};
        if(!file.is_open()) return;
        file << _log_message << '\n';
    }

    static void set_log_bot(Bot *const _bot)
    {
        m_log_bot = _bot;
    }

    static void log(const std::string &_log_message, const LOG _level)
    {
        std::string log_message;
        switch (_level)
        {
        case LOG::TRACE:
            log_message += "[TRACE]";
            break;
        case LOG::INFO:
            log_message += "[INFO]";
            break;
        case LOG::WARNING:
            log_message += "[WARNING]";
            break;
        case LOG::ERROR:
            log_message += "[ERROR]";
            break;
        default:
            break;
        }
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        log_message += "[ ";
        log_message += ctime(&now);
        log_message.erase(--log_message.end()); // erase \n from ctime
        log_message += " ] ";
        log_message += _log_message;

        std::cout << log_message << std::endl;

        write(log_message);

        if(m_log_bot && _level > LOG::TRACE)
            m_log_bot->send_message_to_admin(log_message);
    }

private:
    static Bot* m_log_bot;
};

inline Bot* Logger::m_log_bot{nullptr};