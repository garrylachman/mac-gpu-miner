# MAC GPU MINER
Modifided port of xmrstak compiled as Native module for React-Native using embedd AMD Radeon GPU.
The miner comes with a nice user interface that allow easy mining, just provide XMR address and we do the rest.
We use MoneroOcean pool and cryptonight_gpu algo, the the miner show the local hashrate / the raw pool hashrate and the final payment
hashrate translated to XMR.

Using Radeon Pro 560 4 GB (mid 2017 macbook pro), using GPU only you can run almost for 5kh/s (convert to XMR hashrate).

## Chages in XMRStak
* Covert the miner to dynamic lib
* Expose and make start and stop function
* Event based communication for log and hashrate report
* Tweeks in threads
* Library tester
* Remove unneed code

Unlike other Miner UIs that run seperate process of the miner, here the miner is 
part of the Miner UI.

![](https://i.imgur.com/jHvWEPV.png)
![](https://i.imgur.com/8hlXZhs.png)

## Download
https://github.com/garrylachman/mac-gpu-miner/releases/

## How to use libxmrstak
The XMRStak can be used as library in any osx c++ project.
The files can be found here:
https://github.com/garrylachman/mac-gpu-miner/tree/main/app/macos/miner-lib

### Example
```
#include <stdio.h>
#include <string.h>
#include "lib-miner.h"
#include <thread>
#include <iostream>
#include <pthread.h>
#include "event.hpp"

int main(int argc, char **argv) {
   
    RETURN_CODES r;
    
    get_eventter()->on<PrintEvent>([](const Event& e) { std::cout << "PrintEvent: " << static_cast<const PrintEvent&>(e).msg << std::endl; });
    
    get_eventter()->on<HashrateEvent>([](const Event& e) {
        std::cout << "HashrateEvent: last 10s hash: " << static_cast<const HashrateEvent&>(e).h1 << std::endl;
        std::cout << "HashrateEvent: last 60s hash: " << static_cast<const HashrateEvent&>(e).h2 << std::endl;
        std::cout << "HashrateEvent: last 15m hash: " << static_cast<const HashrateEvent&>(e).h3 << std::endl;
        std::cout << "HashrateEvent: last top hash: " << static_cast<const HashrateEvent&>(e).t << std::endl;
    });
    
    std::thread th =  std::thread([&r]() {
        r = start_miner("gulf.moneroocean.stream:10032", "44j8Qpnc11h1L3FHswp5XNUz7mDGnjaTZ28UjYrMS8fyPyWkrQMGwj22SCrxeNfaR8TwwGCQeXaZvdw5A7UbZEZg5HNv4Lt", "libtester~cn/gpu");
    });
    
    pthread_t thId = th.native_handle();
    th.detach();
    
     std::thread([&_execute, r, &thId, &th]()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*60*5));
        stop_miner();
        pthread_cancel(thId);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000*10));
    ).detach();
    
     while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "Bye Bye" << std::endl;
    
    return 0;
}
```

Thanks,
Garry Lachman
