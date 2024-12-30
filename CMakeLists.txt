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

# Add the executable
add_executable(TUIStock src/main.c src/app.c
        node_modules/yaml.h)

# Link the ncurses and libcyaml libraries
target_link_libraries(TUIStock ${CURSES_LIBRARIES} /usr/local/lib/libcyaml.a)

# Custom target to use the specified gcc command
add_custom_target(
        custom_build ALL
        COMMAND gcc ${CMAKE_SOURCE_DIR}/src/main.c ${CMAKE_SOURCE_DIR}/src/app.c -o main -lncurses -lcyaml
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Building with custom gcc command"
)