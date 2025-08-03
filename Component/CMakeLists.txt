if(WIN32)
    set(WIN_COMPONENT_SOURCES
        Component.def
        dllmain.cpp
    )
endif()
add_library(Component SHARED
    Component.cpp
    ${WIN_COMPONENT_SOURCES}
)

target_link_libraries(Component PRIVATE Interface stduuid)

target_include_directories(Component PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(Component PRIVATE $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>)

target_compile_options(Component PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )

