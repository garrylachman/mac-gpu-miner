#define EXPORT __attribute__((visibility("default")))

#include <string>
#include <vector>
#include "event.hpp"

enum EXPORT class RETURN_CODES {
    EXIT,
    CONFIG_PARSE_ERROR,
    SELF_TEST_ERROR
};

RETURN_CODES start_miner(std::string poolURL, std::string poolUsername, std::string poolPasswd);
void stop_miner();
std::vector<double> get_total_hashrate();
double get_top_hashrate();
event* get_eventter();
