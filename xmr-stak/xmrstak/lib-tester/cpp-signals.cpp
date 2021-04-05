//
//  cpp-signals.cpp
//  xmr-stak-lib-tester
//
//  Created by Dr G on 30/03/2021.
//


#include <cstring> // strsignal
#include <csignal>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <mutex>
#include "cpp-signals.hpp"

namespace CppSignal {

std::timed_mutex signalHandlersMutex;
std::unordered_map<int, std::function<void(int)>> signalHandlers;

// generic handler (free function) to set as a handler for any signal
void dispatcher(int signal) {
  std::unique_lock<std::timed_mutex> lock(signalHandlersMutex, std::defer_lock);
  if (!lock.try_lock_for(std::chrono::seconds(1))) {
    // unable to get the lock. should be a strange case
    return;
  }
  auto it = signalHandlers.find(signal);
  if (it != signalHandlers.end()) {
    it->second(signal);
  }
}

void registerHandler(int signal, const std::function<void(int)>& handler) {
  std::lock_guard<std::timed_mutex> lock(signalHandlersMutex);
  signalHandlers.emplace(signal, handler);
}

// this is the only method you will use
void setHandler(int signal, const std::function<void(int)>& handler, int flags) {
  // configure sigaction structure
  struct sigaction action;
  if (sigfillset(&action.sa_mask) == -1) {
    throw std::runtime_error("sigfillset failed");
  }
  action.sa_flags = flags;
  action.sa_handler = dispatcher;
    
  // set handler for the signal
  if (sigaction(signal, &action, nullptr) == -1 && signal < 65) {
    throw std::runtime_error("Fail at configuring handler for signal: " + std::string(strsignal(signal)));
  }
  registerHandler(signal, handler);
}

}
