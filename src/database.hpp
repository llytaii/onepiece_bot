#pragma once
#include "base.hpp"

#include <filesystem>
#include <fstream>
#include <set>

using int_fast64_t = long;

class Database : public Base
{
public:
    bool init();

    std::set<int_fast64_t>& get_users();
    
    bool has_user(const int_fast64_t _id);
    bool add_user(const int_fast64_t _id);
    bool remove_user(const int_fast64_t _id);

private:
    std::set<int_fast64_t> m_users;

    bool write_users_to_file();
};
