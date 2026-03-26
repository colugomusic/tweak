cmake_minimum_required(VERSION 3.20)
project(tweak-test)
list(APPEND tweak-test-src
	src/doctest.h
	src/main.cpp
)
add_executable(tweak-test ${tweak-test-src})
target_link_libraries(tweak-test tweak::tweak)
add_test(NAME tweak-test COMMAND tweak-test)
