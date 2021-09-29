#include "bot.hpp"
#include "websites.hpp"

#include <chrono>
#include <thread>

// run bot while fetching websites in a separate thread every 10 minutes

void run_fetchers(Bot *_b);

int main()
{
  Bot b{};
  if(!b.init()) return -1;
  std::thread t1{run_fetchers, &b};

  b.run();
  t1.join();
}

void run_fetchers(Bot *_b)
{
  while (true)
  {
    ChapterOnePieceTube{_b};
    EpisodeOnePieceTube{_b};

    std::this_thread::sleep_for(std::chrono::minutes(10));
  }
}