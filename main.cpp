#include <iostream>

#include "config.hpp"

int main(int, char**) {
    Config config("config.ini");
    if (config.is_open()) {
        config.load();

        std::cout << "============== Last =============" << std::endl;
        config.print();
        std::cout << "===========================" << std::endl;
    }
    config.clear();
    config.setCategory("global");
    config.setValue<int>("count", 1);

    config.setCategory("player");
    config.setValue<float>("health", 1.0f);

    config.setCategory("enemy");
    config.setValue<float>("health", 1.0f);
    config.setValue<float>("max_health", 100.0f);
    config.save();

    std::cout << "============== Next =============" << std::endl;
    config.print();
    std::cout << "===========================" << std::endl;
    return 0;
}
