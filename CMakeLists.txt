cmake_minimum_required(VERSION 3.10)

# Set the project name
project(TUIStock)

# Specify the C standard
set(CMAKE_C_STANDARD 99)

# Find the ncurses library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Include directories for libcyaml
include_directories(/usr/local/include)

# Include the header files directory
include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(include)

# Add the executable
add_executable(TUIStock src/main.c src/app.c src/interact.c src/data.c)

# Link the ncurses, libcyaml

target_link_libraries(TUIStock ${CURSES_LIBRARIES} /usr/local/lib/libcyaml.a)