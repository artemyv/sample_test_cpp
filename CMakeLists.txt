cmake_minimum_required(VERSION 3.15)
project(ClientProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Interface)

include(FetchContent)
FetchContent_Declare(
    dllhelper
    GIT_REPOSITORY https://github.com/artemyv/dllhelper.git
    GIT_TAG        v2.0 # or a commit hash or branch
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


add_subdirectory(Component)
add_subdirectory(Client)