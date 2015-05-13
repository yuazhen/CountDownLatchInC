project (countdownlatch_test)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)

add_executable (countdownlatch_test
	test.c
	countdownlatch.c)

target_link_libraries (countdownlatch_test pthread)
