#include <iostream>

import LRUCache;
import LFUCache;
import CacheWrapper;

int main() {
    std::cout << "Choose cache type:" << std::endl;
    std::cout << "1. LRU" << std::endl;
    std::cout << "2. LFU" << std::endl;

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        cache::LRUCache cache(100);
        cache::CacheWrapper wrapper(cache);
        wrapper.run();
    }
    else if (choice == 2) {
        cache::LFUCache cache(100);
        cache::CacheWrapper wrapper(cache);
        wrapper.run();
    }
    else {
        std::cout << "Invalid choice" << std::endl;
        return 1;
    }

    return 0;
}
