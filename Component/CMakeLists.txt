add_library(Component SHARED
    Component.cpp
    Component.def
    dllmain.cpp
)

target_link_libraries(Component PRIVATE Interface)

target_include_directories(Component PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(Component PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )

