cmake_minimum_required(VERSION 3.17)
project(toms_data_onion LANGUAGES CXX)

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
