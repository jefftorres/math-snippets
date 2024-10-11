cmake_minimum_required(VERSION 3.28)
project(math_snippets C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

add_executable(monte_carlo monte_carlo.c)
add_executable(conway conway.c)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
# add_executable(${PROJECT_NAME} scripts/main.cpp)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(conway ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})