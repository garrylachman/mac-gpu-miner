# MAC GPU MINER
Modifided port of xmrstak compiled as Native module for React-Native.
The miner comes with a nice user interface that allow easy mining, just provide XMR address and we do the rest.
We use MoneroOcean pool and cryptonight_gpu algo, the the miner show the local hashrate / the raw pool hashrate and the final payment
hashrate translated to XMR.

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

## How to build
TO DO

Thanks,
Garry Lachman
