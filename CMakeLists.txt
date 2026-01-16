cmake_minimum_required(VERSION 3.16)

project(Casino
    VERSION 1.0
    LANGUAGES C
)

# Use modern C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PLATFORM_SRC "")
if(WIN32)
    set(PLATFORM_SRC src/platform/platform_windows.c)
else()
    set(PLATFORM_SRC src/platform/platform_unix.c)
endif()

add_executable(Casino
    src/main.c
    src/blackjack.c
    src/beg.c
    ${PLATFORM_SRC}  # Only the correct platform file
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
