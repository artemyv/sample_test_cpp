add_executable(Client
    Client.cpp
    IOUtils.cpp
    IOUtils.h
)

target_link_libraries(Client PRIVATE Interface dllhelper GSL)

target_include_directories(Client PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(Client PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()