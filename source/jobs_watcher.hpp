#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include "db_ops.hpp"

void jobs_watcher_start(std::function<void(void)> func, unsigned int interval)
{
  std::thread([func, interval]()
  { 
    while (true)
    { 
      auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
      func();
      std::this_thread::sleep_until(x);
    }
  }).detach();
}

void prot_jobs_scheduling()
{
  std::cout << "I am doing something" << std::endl;
}

int main()
{
  jobs_watcher_start(prot_jobs_scheduling, 1000);
  while (true)
    ;
}