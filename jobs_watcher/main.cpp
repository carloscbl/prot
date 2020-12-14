#include "jobs_watcher.hpp"
#ifdef __linux__
#include <vector>
#include <signal.h>
#include <unistd.h>
#endif

#include <iostream>
#include "spdlog/spdlog.h"


bool end_program = false;
#ifdef __linux__
static void sigHandler [[noreturn]] (int sig){
    switch(sig){
        case SIGINT:
        case SIGQUIT:
        case SIGTERM:
        case SIGHUP:
        default:
            end_program = true;
            break;
    }
    exit(0);
}

static void setUpUnixSignals(std::vector<int> quitSignals) {
    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = sigHandler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);
}
#endif

int main(int argc, char *argv[]){
#ifdef __linux__
    std::vector<int> sigs{SIGQUIT, SIGINT, SIGTERM, SIGHUP};
    setUpUnixSignals(sigs);
#endif
    spdlog::set_pattern("[%H:%M:%S %z] %^%l%$ %! %s:%# %v");
    SPDLOG_INFO("Welcome to spdlog!");
    SPDLOG_ERROR("Welcome to spdlog!");
    SPDLOG_DEBUG("Welcome to spdlog!");
    SPDLOG_CRITICAL("Welcome to spdlog!");
    std::cout << "Starting Prot Jobs Watcher Service..." << std::endl;
    jobs_watcher_start(prot_jobs_scheduling, 3000);
    while (!end_program){
        sleep(2);
    }
    std::cout << "Ending Prot Jobs Watcher Service" << std::endl;
    return 0;
}