# CMakeLists.txt for tests

if(WIN32)
    set(WIN_COMPONENT_SOURCES
        ../Component/Component.def
        ../Component/dllmain.cpp
    )
endif()
add_library(BadComponent SHARED
    BadComponent.cpp
    ${WIN_COMPONENT_SOURCES}
)

target_link_libraries(BadComponent PRIVATE Interface stduuid)

target_include_directories(BadComponent PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(BadComponent PRIVATE $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>)

target_compile_options(BadComponent PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )


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
add_dependencies(ComponentWrapperTests Component BadComponent)

add_custom_command(TARGET ComponentWrapperTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Component>
        $<TARGET_FILE_DIR:ComponentWrapperTests>
)

add_custom_command(TARGET ComponentWrapperTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:BadComponent>
        $<TARGET_FILE_DIR:ComponentWrapperTests>
)


target_compile_definitions(ComponentWrapperTests
    PRIVATE
        COMPONENT_TARGET_NAME="$<TARGET_FILE_NAME:Component>"
        BAD_COMPONENT_TARGET_NAME="$<TARGET_FILE_NAME:BadComponent>"
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>
)

target_compile_options(ComponentWrapperTests PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )

add_test(NAME ComponentWrapperTests COMMAND ComponentWrapperTests)




