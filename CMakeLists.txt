cmake_minimum_required(VERSION 3.0.0)
project(Testing VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(RunMainCpp main.cpp Game.hpp Player.hpp Building.hpp Troops.hpp Buildings_Types.hpp)
target_link_libraries(RunMainCpp ncurses)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)  
