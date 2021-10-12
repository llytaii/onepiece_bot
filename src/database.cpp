#include "database.hpp"
#include "logger.hpp"

bool Database::init()
{
    std::fstream file{get_res_path() /= "user.data", std::ios::in};
    if (!file.is_open())
    {
        Logger::log("Database::init(): opening user.data failed!", LOG::ERROR);
        return false;
    }

    int_fast64_t tmp;
    while (file >> tmp)
        m_users.insert(tmp);

    // remove possible dups by saving set
    return write_users_to_file();
}

std::set<int_fast64_t>& Database::get_users()
{
    return m_users;
}

bool Database::has_user(const int_fast64_t _id)
{
    return (m_users.find(_id) != m_users.end());
}


bool Database::add_user(const int_fast64_t _id)
{
    if (!m_users.insert(_id).second)
    {
        Logger::log("Database::add_user(): adding id " + std::to_string(_id) + " failed!\n/status for more information", LOG::ERROR);
        return false;
    }
    
    return write_users_to_file();
}

bool Database::remove_user(const int_fast64_t _id)
{
    if (!m_users.erase(_id))
    {
        Logger::log("Database::remove_user(): removing id " + std::to_string(_id) + " failed!", LOG::ERROR);
        return false;
    }

    return write_users_to_file();
}

bool Database::write_users_to_file()
{
    std::fstream file{get_res_path() /= "user.data", std::ios::out};
    if (!file.is_open())
    {
        Logger::log("Database::write_users_to_file(): opening user.data failed!", LOG::ERROR);
        return false;
    }

    for (auto u : m_users)
    {
        file << u;
        file << ' ';
    }

    return true;
}