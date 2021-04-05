// RCTCalendarModule.m
#import "RCTMinerModule.h"
#include <string>
#import <IOKit/pwr_mgt/IOPMLib.h>

@implementation RCTMinerModule {
  BOOL _hasListeners;
  NSString *poolURL;
  NSString *poolWallet;
  NSString *poolPassword;
  CFStringRef reasonForActivity;
  IOPMAssertionID assertionID;
  IOReturn success;
};

// To export a module named RCTMinerModule
RCT_EXPORT_MODULE(MinerModule);

RCT_EXPORT_METHOD(setWallet:(NSString *)wallet)
{
  poolURL = @"gulf.moneroocean.stream:10032";
  poolWallet = wallet;
  poolPassword = @"MacGpuMiner~cn/gpu";
  RCTLogInfo(@"Set wallet %@", wallet);
  std::cout << "set wallet" << std::endl;
}

RCT_EXPORT_METHOD(startMiner)
{
  std::cout << "start miner" << std::endl;
  /*reasonForActivity = CFStringCreateWithCString(NULL, "MAC GPU MINER", kCFStringEncodingASCII);
  success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                                      kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
  */
  std::string url = [poolURL cStringUsingEncoding:NSASCIIStringEncoding];
  std::string wallet = [poolWallet cStringUsingEncoding:NSASCIIStringEncoding];
  std::string pass = [poolPassword cStringUsingEncoding:NSASCIIStringEncoding];
  start_miner(url, wallet, pass);
    
}

RCT_EXPORT_METHOD(stopMiner)
{
  stop_miner();
  /*if (success == kIOReturnSuccess) {
    success = IOPMAssertionRelease(assertionID);
  }*/
}

RCT_EXPORT_METHOD(bindMinerEvents)
{
  std::cout << "bind miner" << std::endl;
  get_eventter()->on<PrintEvent>([=](const Event& e) {
      std::string stdMsg(static_cast<const PrintEvent&>(e).msg);
    std::cout << stdMsg << std::endl;
      NSString *msg = [NSString stringWithCString:stdMsg.c_str() encoding:NSASCIIStringEncoding];
      RCTLogInfo(@"event: %@", msg);
      if (_hasListeners) {
        [self sendEventWithName:@"miner_log" body: msg];
      }
    }
  );
  
  get_eventter()->on<HashrateEvent>([=](const Event& e) {
    
      NSNumber *h1 = [NSNumber numberWithDouble:static_cast<const HashrateEvent&>(e).h1];
      NSNumber *h2 = [NSNumber numberWithDouble:static_cast<const HashrateEvent&>(e).h2];
      NSNumber *h3 = [NSNumber numberWithDouble:static_cast<const HashrateEvent&>(e).h3];
      NSNumber *t = [NSNumber numberWithDouble:static_cast<const HashrateEvent&>(e).t];
    
      RCTLogInfo(@"event hashrate: %f, %f, %f, %f", h1.doubleValue, h2.doubleValue, h3.doubleValue, t.doubleValue);
      if (_hasListeners) {
        [self
         sendEventWithName:@"hashrate"
         body:@{ @"last10s": @(h1.doubleValue), @"last60s": @(h2.doubleValue), @"last15m": @(h3.doubleValue), @"top": @(t.doubleValue) }
        ];
        [self sendPoolUpdates];
      }
    }
  );
}

- (NSArray<NSString *> *)supportedEvents
{
  return @[@"hashrate", @"pool", @"miner_log"];
}

- (void)startObserving
{
  _hasListeners = YES;
}

- (void)stopObserving
{
  _hasListeners = NO;
}

- (void)updatesTimer:(NSTimer *)timer
{
  //[self sendHashrateUpdates];
  //[self sendPoolUpdates];
}

- (void)sendPoolUpdates
{
  NSError *error;
  NSString *url_string = [NSString stringWithFormat: @"https://api.moneroocean.stream/miner/%@/stats", poolWallet];
  NSData *data = [NSData dataWithContentsOfURL: [NSURL URLWithString:url_string]];
  NSMutableArray *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
  [self sendEventWithName:@"pool" body: json];
  RCTLogInfo(@"json: %@", json);
}


@end
