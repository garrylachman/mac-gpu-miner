//
//  event.cpp
//  MAC GPU MINER
//
//  Created by Garry Lachman on 03/04/2021.
//  Taken from https://gist.github.com/darkf/8882611

#include "xmrstak/misc/event.hpp"


void event::emit(const Event& event) {
    auto range = this->eventMap.equal_range(&typeid(event));
    for(auto it = range.first; it != range.second; ++it)
        it->second(event);

}

//event* event::instance = nullptr;
