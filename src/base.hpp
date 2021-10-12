#pragma once

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class Base
{
public:
    static bool find_res_path()
    {
        fs::path current{fs::current_path()};

        if(fs::path tmp{fs::path{current} /= "res"}; fs::exists(tmp))
        {
            m_res_path = tmp;
            return true;
        }
        if (fs::path tmp{fs::path{current.parent_path()} /= "res"}; fs::exists(tmp))
        {
            m_res_path = tmp;
            return true;
        }
        else
            return false;
    }

protected:
    static fs::path get_res_path()
    {
        return m_res_path;
    }
    
private:
    static fs::path m_res_path;
};

inline fs::path Base::m_res_path{};