#include "bot.hpp"
#include "logger.hpp"

int main()
{
  if(!Base::find_res_path())
  {
    Logger::log("finding resource directory failed!", LOG::ERROR);
    return -1;
  }

  Bot b{};

  if(!b.init())
  {
    Logger::log("initializing bot failed!", LOG::ERROR);
    return -1;
  }

  b.run();
}