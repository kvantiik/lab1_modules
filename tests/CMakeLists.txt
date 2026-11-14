add_executable(cache_tests
        LRUCacheTest.cpp
        LFUCacheTest.cpp
)

target_link_libraries(cache_tests
        PRIVATE
        cache_modules
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(cache_tests)
