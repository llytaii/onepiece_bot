#include "database.hpp"
#include "bot.hpp"

bool Database::save_set()
{
    m_file.open("./res/user.data", std::ios::out);
    if (!m_file.is_open())
    {
        m_logger->log("Database::save_set(): Failed to open user.data for writing!", LogLevel::ERROR);
        return false;
    }

    for (auto &e : m_users)
    {
        m_file << e;
        m_file << ' ';
    }
    m_file.close();
    return true;
}

Database::Database(Bot *_logger) : m_logger{_logger}
{
}

bool Database::init()
{
    m_file.open("./res/user.data", std::ios::in);
    if (!m_file.is_open())
    {
        m_logger->log("Database::init(): fstream.is_open() returned false while opening user.data for reading!", LogLevel::ERROR);
        return false;
    }

    int_fast64_t tmp;
    while (m_file >> tmp)
    {
        if (!m_users.insert(tmp).second)
        {
            m_logger->log("Database::init(): set.insert() returned false during initialization from file!", LogLevel::ERROR);
        }
    }
    m_file.close();
    // remove dups by saving set
    return save_set();
}

std::set<int_fast64_t> Database::get_users()
{
    return m_users;
}

bool Database::save_user(int_fast64_t _id)
{
    if (!m_users.insert(_id).second)
    {
        m_logger->log("Database::save_user(): set::insert() returned false!", LogLevel::ERROR);
        return false;
    }
    return save_set();
}

bool Database::delete_user(int_fast64_t _id)
{
    if (!m_users.erase(_id))
    {
        m_logger->log("Database::delete_user(): set.erase() returned false!", LogLevel::ERROR);
        return false;
    }
    return save_set();
}