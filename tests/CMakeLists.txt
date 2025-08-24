# CMakeLists.txt for tests

set(TEST_SOURCES
    ComponentWrapperTests.cpp
)

add_executable(ComponentWrapperTests ${TEST_SOURCES})

target_link_libraries(ComponentWrapperTests
    PRIVATE
        gtest_main
        gtest
        Interface
        Component
        dllhelper
        GSL
        stduuid
)

target_include_directories(ComponentWrapperTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Interface
        ${CMAKE_SOURCE_DIR}/Component
)
add_dependencies(ComponentWrapperTests Component)

add_custom_command(TARGET ComponentWrapperTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Component>
        $<TARGET_FILE_DIR:ComponentWrapperTests>
)

target_compile_definitions(ComponentWrapperTests
    PRIVATE
        COMPONENT_TARGET_NAME="$<TARGET_FILE_NAME:Component>"
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>
)

target_compile_options(ComponentWrapperTests PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )

add_test(NAME ComponentWrapperTests COMMAND ComponentWrapperTests)

