module;

#include <iostream>
#include <cstdint>
#include <stdexcept>

export module CacheWrapper;

import ICache;

export namespace cache {
    class CacheWrapper {
    public:
        CacheWrapper(ICache &cache);

        std::int64_t Fibonacci(std::int32_t n);

        void run();

    private:
        ICache &cache;
    };

    CacheWrapper::CacheWrapper(ICache &cache) : cache(cache) {
    }

    std::int64_t CacheWrapper::Fibonacci(std::int32_t n) {
        if (n < 0) {
            throw std::invalid_argument("n must be greater than or equal to 0");
        }
        if (n > 92) {
            throw std::out_of_range("n is too large");
        }

        if (cache.contains(n)) {
            return cache.get(n);
        }

        std::int64_t result;

        if (n == 0) {
            result = 0;
        } else if (n == 1 || n == 2) {
            result = 1;
        } else {
            result = Fibonacci(n - 1) + Fibonacci(n - 2);
        }

        cache.put(n, result);

        return result;
    }

    void CacheWrapper::run() {
        std::int32_t n;

        while (true) {
            std::cout << "Enter n (-1 to exit): " << std::endl;

            if (!(std::cin >> n)) {
                std::cout << "Invalid input" << std::endl;

                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }

            if (n == -1) {
                break;
            }

            try {
                std::cout << Fibonacci(n) << std::endl;
            } catch (const std::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
    }
}
