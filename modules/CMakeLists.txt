add_library(cache_modules)

target_sources(cache_modules
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ICache.cppm
        LRUCache.cppm
        LFUCache.cppm
        CacheWrapper.cppm
)