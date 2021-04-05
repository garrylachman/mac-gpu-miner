//
//  Header.h
//  xmr-stak-backend
//
//  Created by Garry Lachman on 03/04/2021.
//  Taken from https://gist.github.com/darkf/8882611
#pragma once

#ifndef Event_h
#define Event_h

#include <functional>
#include <map>
#include <typeinfo>
#include <iostream>
#include <mutex>

struct Event {
    virtual ~Event() {}
};
struct PrintEvent : Event {
    std::string msg;
    PrintEvent(std::string msg) : msg(msg) {}
};
struct HashrateEvent : Event {
    double h1;
    double h2;
    double h3;
    double t;
    HashrateEvent(double h1, double h2, double h3, double t) : h1(h1), h2(h2), h3(h3), t(t) {}
};


typedef std::multimap<const std::type_info*,
                      const std::function<void(const Event&)>> EventsMap;


class event {
    
    private:
        EventsMap eventMap;

    public:
        static event* inst() {
            static event instance;
            return &instance;
        }
    
        template<typename EventWanted>
        void on(const std::function<void(const Event&)>& fn) {
            this->eventMap.emplace(&typeid(EventWanted), fn);
        }

        void emit(const Event& event);
};



#endif /* Event_h */
