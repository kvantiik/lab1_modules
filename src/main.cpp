#include <iostream>
#include <chrono>

import LRUCache;
import LFUCache;
import CacheWrapper;

int main() {
    std::cout << "Choose cache type:" << std::endl;
    std::cout << "1. LRU" << std::endl;
    std::cout << "2. LFU" << std::endl;

    int choice;
    std::cin >> choice;

    std::int64_t ttl;
    std::cout << "Enter TTL in seconds (0 for unlimited): ";
    std::cin >> ttl;

    if (ttl < 0) {
        std::cout << "TTL cannot be negative" << std::endl;
        return 1;
    }

    if (choice == 1) {
        cache::LRUCache cache(100, std::chrono::seconds(ttl));
        cache::CacheWrapper wrapper(cache);
        wrapper.run();
    }
    else if (choice == 2) {
        cache::LFUCache cache(100, std::chrono::seconds(ttl));
        cache::CacheWrapper wrapper(cache);
        wrapper.run();
    }
    else {
        std::cout << "Invalid choice" << std::endl;
        return 1;
    }

    return 0;
}
