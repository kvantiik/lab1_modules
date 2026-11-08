add_executable(lab1
        ../src/main.cpp
        LRUCacheTest.cpp
        LFUCacheTest.cpp
)

target_link_libraries(lab1
        PRIVATE
        cache_modules
        GTest::gtest
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(lab1)