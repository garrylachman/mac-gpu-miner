#ifndef __CPPSIGNAL_HPP
#define __CPPSIGNAL_HPP

#include <functional>

namespace CppSignal {

void setHandler(int signal, const std::function<void(int)>& handler, int flags=0);

}

#endif
