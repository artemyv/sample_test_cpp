cmake_minimum_required(VERSION 3.23..4.2)
project(ClientProject VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(SAMPLE_CPP_MAIN_PROJECT ON)
endif()

# Ensure compile_commands.json is generated
set(CMAKE_EXPORT_COMPILE_COMMANDS  ${SAMPLE_CPP_MAIN_PROJECT})

# Coverage option
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

include(FetchContent)
FetchContent_Declare(
    dllhelper
    GIT_REPOSITORY https://github.com/artemyv/dllhelper.git
    GIT_TAG        v2.1.1 # or a commit hash or branch
)
FetchContent_MakeAvailable(dllhelper)

FetchContent_Declare(
    GSL
    GIT_REPOSITORY https://github.com/microsoft/GSL.git
    GIT_TAG        v4.2.0 # or a commit hash or branch
)
FetchContent_MakeAvailable(GSL)

FetchContent_Declare(
    stduuid
    GIT_REPOSITORY https://github.com/mariusbancila/stduuid.git
    GIT_TAG        v1.2.3 # or a commit hash or branch
)
# Enable std::span instead of gsl::span in stduuid
set(UUID_USING_CXX20_SPAN ON CACHE BOOL "Using span from std instead of gsl")

FetchContent_MakeAvailable(stduuid)

add_subdirectory(Interface)
add_subdirectory(Component)
add_subdirectory(Client)

# ----------------- Testing Support -----------------
option(SAMPLE_CPP_BUILD_TESTING "Build tests" ${SAMPLE_CPP_MAIN_PROJECT})

if(SAMPLE_CPP_BUILD_TESTING)
    include(CTest)
    enable_testing()

    # Fetch GoogleTest
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)

    add_custom_target(run_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --verbose
        DEPENDS ComponentWrapperTests ComponentTests
    )
endif()