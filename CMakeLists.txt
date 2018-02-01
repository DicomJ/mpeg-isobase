cmake_minimum_required(VERSION 2.8.8)
project(mpeg-isobase)
include(GNUInstallDirs)

add_definitions(-std=${CXX_STD} -Wno-multichar)
file(GLOB sources src/*.cc src/box/*.cc)
include_directories(PUBLIC include PRIVATE src)
add_library(lib${PROJECT_NAME}_object OBJECT ${sources})
set_target_properties(lib${PROJECT_NAME}_object PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(lib${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:lib${PROJECT_NAME}_object>)
add_library(lib${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:lib${PROJECT_NAME}_object>)
set_target_properties(lib${PROJECT_NAME}_shared lib${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

install(TARGETS lib${PROJECT_NAME}_shared lib${PROJECT_NAME}_static
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# Externs

add_subdirectory(extern)

ExternalProject_Get_Property(bitstream INSTALL_DIR)
add_dependencies(lib${PROJECT_NAME}_object bitstream)
include_directories(PUBLIC ${INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
target_link_libraries(lib${PROJECT_NAME}_shared ${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/libbitstream${CMAKE_SHARED_LIBRARY_SUFFIX})

install(DIRECTORY ${INSTALL_DIR}/${CMAKE_INSTALL_LIBDIR}/ # make sure trailing slash
        DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${INSTALL_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/ # make sure trailing slash
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Tests

if(CMAKE_BUILD_TYPE MATCHES Debug)
    enable_testing()
    add_subdirectory(test)
endif()

