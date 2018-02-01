include(CTest)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB sources *.cc)

add_executable(test_${PROJECT_NAME} ${sources})
target_link_libraries(test_${PROJECT_NAME} lib${PROJECT_NAME}_shared ${GTEST_BOTH_LIBRARIES})
add_test(test_${PROJECT_NAME} test_${PROJECT_NAME})

