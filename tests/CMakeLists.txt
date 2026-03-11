find_package(Catch2 CONFIG REQUIRED)

include(Catch)

add_executable(unit_tests
    test_main.cpp
)

target_link_libraries(unit_tests
    PRIVATE
    Catch2::Catch2WithMain
)

# Registrar no CTest
catch_discover_tests(unit_tests
    REPORTER xml
    OUTPUT_DIR ${CMAKE_BINARY_DIR}
    OUTPUT_PREFIX "test_"
    OUTPUT_SUFFIX ".xml"
)