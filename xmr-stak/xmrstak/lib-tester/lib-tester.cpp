/* MAC GPU MINER
 * By Garry Lachman 2021
 *****************************************************/
 
#include <stdio.h>
#include <string.h>
#include "../lib/lib-miner.h"
#include <thread>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include "cpp-signals.hpp"
#include <pthread.h>
#include "../misc/event.hpp"

int main(int argc, char **argv) {
   
    RETURN_CODES r;
    
    get_eventter()->on<PrintEvent>([](const Event& e) { std::cout << "from event: " << static_cast<const PrintEvent&>(e).msg << std::endl; });
    
    get_eventter()->on<HashrateEvent>([](const Event& e) {
        std::cout << "from event last 10s hash: " << static_cast<const HashrateEvent&>(e).h1 << std::endl;
        std::cout << "from event last 60s hash: " << static_cast<const HashrateEvent&>(e).h2 << std::endl;
        std::cout << "from event last 15m hash: " << static_cast<const HashrateEvent&>(e).h3 << std::endl;
        std::cout << "from event last top hash: " << static_cast<const HashrateEvent&>(e).t << std::endl;
    });
    
    std::thread th =  std::thread([&r]() {
        r = start_miner("gulf.moneroocean.stream:10128", "44j8Qpnc11h1L3FHswp5XNUz7mDGnjaTZ28UjYrMS8fyPyWkrQMGwj22SCrxeNfaR8TwwGCQeXaZvdw5A7UbZEZg5HNv4Lt", "libtester~cn/gpu");
    });
    
    pthread_t thId = th.native_handle();
    th.detach();
    
    
    
    bool _execute = true;
    
    CppSignal::setHandler(SIGINT, [&_execute] (int sigint) {
        std::cout << "Exit..." << std::endl;
        _execute = false;
        exit(sigint);
    });
    
    int count = 0;
    
    std::thread([&_execute, r, &count, &thId, &th]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*30));
        while (_execute) {
            std::cout << "Total Hashrate: " << std::endl;
            std::vector<double> t(get_total_hashrate());
            std::for_each(t.begin(), t.end(), [](double h) { std::cout << h << "  "; });
            std::cout << std::endl;
            std::cout << "Top: " << get_top_hashrate() << std::endl << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000*10));
            count++;
            
            if (count > 5) {
                std::cout << "Time to stop" << std::endl;
                //_execute = false;
                stop_miner();
                pthread_cancel(thId);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000*60));
                
                th =  std::thread([&r]() {
                    start_miner("gulf.moneroocean.stream:10128", "44j8Qpnc11h1L3FHswp5XNUz7mDGnjaTZ28UjYrMS8fyPyWkrQMGwj22SCrxeNfaR8TwwGCQeXaZvdw5A7UbZEZg5HNv4Lt", "libtester~cn/gpu");
                });
                thId = th.native_handle();
                th.detach();
                count = 0;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000*10));
            }
        }
    }).detach();
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "Bye Bye" << std::endl;
    
    return 0;
}
