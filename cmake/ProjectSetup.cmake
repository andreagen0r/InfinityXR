include_guard(GLOBAL)

message(STATUS "Configuring Project: ${PROJECT_NAME}...")

# =============================================================================
# General Settings
# =============================================================================
# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Folder Support for Visual Studio / XCode
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Generate position independent code
if(UNIX)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Enable folder support for IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# --- BUILD_TYPE ---
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)

    message(STATUS "Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui, ccmake
    set_property(CACHE CMAKE_BUILD_TYPE
        PROPERTY STRINGS
        "Debug"
        "Release"
        "MinSizeRel"
        "Profile"
        "RelWithDebInfo")

endif()

# --- General ---
option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so)" ON)
option(ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" OFF)
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
option(ENABLE_CACHE "Enable cache if available" ON)
option(ENABLE_MARCH_NATIVE "Enable march=native if build on the host machine" OFF)
option(ENABLE_USER_LINKER "Enable a specific linker if available" OFF)
option(ENABLE_USE_POSTFIX "Enable postfix to target (Debug -d | MinSizeRel -mr | RelWithInfo -rd)" OFF)


# --- Warnings ---
option(ENABLE_WARNINGS "Enable warnings" ON)
option(ENABLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# --- Testing ---
option(BUILD_TESTING "Build the testing suite" ON)

# --- Static Analysis ---
option(ENABLE_CPPCHECK "Enable static analysis with cppcheck" ON)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" ON)
option(ENABLE_CLANG_FORMAT "Enable format code with clang-format" ON)
option(ENABLE_INCLUDE_WHAT_YOU_USE "Enable static analysis with include-what-you-use" OFF)

# --- Documentation ---
option(BUILD_DOCUMENTATION "Build Doxygen documentation" ON)
set(DOCUMENTATION_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE STRING "Choose the path of source files.")

# =============================================================================
# Options by Compiler type
# =============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace" OFF)
endif()

# Sanitizers funcionam tanto em GCC quanto em Clang
if(NOT MSVC)
    option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
    option(ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable UB sanitizer" OFF)
    option(ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
endif()

include(Misc)
include(PreventSourceBuild)
include(Warnings)
include(GenerateExportHeaders)
include(Sanitizers)
include(StandardProjectSetupI18n)
include(Git)
include(BaseSettings)
include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)
include(Doxygen)

set(AUTO_GENERATED_FILES_WARNING_MESSAGE
"/****************************************************************************
* ******************************* CAUTION ***********************************
* All changes made in this file will be lost!
*
* If you need to edit this file, look for the file with the same name
* and *.in as a suffix.
*****************************************************************************/")




