cmake_minimum_required(VERSION 4.0)

project(InfinityXR
    VERSION 0.1.0
    DESCRIPTION "Microkernel example host application"
    HOMEPAGE_URL "https://github.com/andreagen0r"
    LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ProjectSetup)

# Global
find_package(Qt6 REQUIRED COMPONENTS Core LinguistTools)

include(QtBootstrap)
include(Doxygen)

# ------------------------------------------------------------------------------
# RPATH Para Linux/macOS
# ------------------------------------------------------------------------------
# Permite que o executável encontre libs na mesma pasta ou subpastas sem LD_LIBRARY_PATH
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


add_definitions(
    -DQT_NO_KEYWORDS
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_STRICT_ITERATORS
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_NO_QSNPRINTF
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    -DQT_DISABLE_DEPRECATED_BEFORE=0x060900
)

add_subdirectory(src/sdk)
add_subdirectory(src/Plugins)
add_subdirectory(src/libs/infinity_core)
add_subdirectory(src/app)


message("")
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Version     : ${PROJECT_VERSION}")
message(STATUS "Description : ${PROJECT_DESCRIPTION}")
message(STATUS "Homepage Url: ${PROJECT_HOMEPAGE_URL}")
message(STATUS "System      : ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CXX         : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message("")

# ----------------------------------------------
# TESTING
# ----------------------------------------------
include(CTest)

if(BUILD_TESTING)
    message(STATUS "Testing enabled")
    add_subdirectory(tests)
endif()
