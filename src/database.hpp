#pragma once

#include <fstream>
#include <set>

using int_fast64_t = long;
class Bot;

class Database
{
public:
    Database() = delete;
    explicit Database(Bot *_logger);
    bool init();

    std::set<int_fast64_t> get_users();
    bool save_user(int_fast64_t _id);
    bool delete_user(int_fast64_t _id);

private:
    std::fstream m_file;
    std::set<int_fast64_t> m_users;

    Bot *const m_logger{nullptr};

    bool save_set();
};
