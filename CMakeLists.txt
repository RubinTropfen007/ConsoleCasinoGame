cmake_minimum_required(VERSION 3.16)

project(Casino
    VERSION 1.0
    LANGUAGES C
)

# Use modern C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(Casino
    src/main.c
)

# Include headers
target_include_directories(Casino
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Optional: place binary in bin/
set_target_properties(Casino PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
