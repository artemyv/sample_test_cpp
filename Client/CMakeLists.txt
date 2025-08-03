add_executable(Client
    Client.cpp
    IOUtils.cpp
    IOUtils.h
)

target_link_libraries(Client PRIVATE Interface dllhelper GSL stduuid)

target_include_directories(Client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Platform-specific settings
target_compile_definitions(Client PRIVATE $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>)

target_compile_options(Client PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )
