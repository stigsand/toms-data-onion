cmake_minimum_required(VERSION 3.17)
project(toms_data_onion LANGUAGES CXX)

find_package(OpenSSL REQUIRED)

add_executable(layer0)
target_compile_features(layer0 PRIVATE cxx_std_17)
target_sources(layer0 PRIVATE layer0.cpp)

add_executable(layer1)
target_compile_features(layer1 PRIVATE cxx_std_17)
target_sources(layer1 PRIVATE layer1.cpp)

add_executable(layer2)
target_compile_features(layer2 PRIVATE cxx_std_17)
target_sources(layer2 PRIVATE layer2.cpp)

add_executable(layer3)
target_compile_features(layer3 PRIVATE cxx_std_17)
target_sources(layer3 PRIVATE layer3.cpp)

add_executable(layer4)
target_compile_features(layer4 PRIVATE cxx_std_17)
target_sources(layer4 PRIVATE layer4.cpp)

add_executable(layer5)
target_compile_features(layer5 PRIVATE cxx_std_17)
target_sources(layer5 PRIVATE layer5.cpp)
target_link_libraries(layer5 PRIVATE OpenSSL::Crypto)

add_executable(tomtel-test)
target_compile_features(tomtel-test PRIVATE cxx_std_17)
target_sources(tomtel-test PRIVATE tomtel_test.cpp)

enable_testing()
add_test(NAME tomtel-test COMMAND tomtel-test)
set_tests_properties(tomtel-test PROPERTIES PASS_REGULAR_EXPRESSION "hello world")
