# CMakeLists.txt for tests

if(WIN32)
    set(WIN_COMPONENT_SOURCES
        ../Component/Component.def
        ../Component/dllmain.cpp
    )
else()
    set(LINUX_TEST_SOURCES 
        ClientExeTest.cpp
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
    ${LINUX_TEST_SOURCES}
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
add_dependencies(ComponentWrapperTests Component BadComponent Client)

target_compile_definitions(ComponentWrapperTests
    PRIVATE
        COMPONENT_TARGET_NAME="$<TARGET_FILE:Component>"
        BAD_COMPONENT_TARGET_NAME="$<TARGET_FILE:BadComponent>"
        CLIENT_TARGET_NAME="$<TARGET_FILE:Client>"
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>
)

target_compile_options(ComponentWrapperTests PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )

add_executable(ComponentTests 
    "ComponentTests.cpp"
    "../Component/Component.cpp"
)
target_link_libraries(ComponentTests
    PRIVATE
        gtest_main
        gtest
        Interface
        Component
        stduuid
)

target_include_directories(ComponentTests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Interface
        ${CMAKE_SOURCE_DIR}/Component
)
target_compile_definitions(ComponentTests
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN NOMINMAX>
)
target_compile_options(ComponentTests PRIVATE
     $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
     $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    )

add_dependencies(ComponentTests Component)

add_test(NAME ComponentTests COMMAND ComponentTests)
add_test(NAME ComponentWrapperTests COMMAND ComponentWrapperTests)



