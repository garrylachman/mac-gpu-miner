//
//  RCTMinerModule.h
//  macgpuminer-macOS
//
//  Created by Dr G on 30/03/2021.
//


//  RCTCalendarModule.h
#import <React/RCTBridgeModule.h>
#import <React/RCTLog.h>
#import <React/RCTEventEmitter.h>
#import "lib-miner.hpp"

@interface RCTMinerModule : RCTEventEmitter <RCTBridgeModule>
@end

