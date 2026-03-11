if(NOT EXISTS "${PRE_CONFIGURE_FILE}")
    message(FATAL_ERROR "GitWatcher: Template file not found: ${PRE_CONFIGURE_FILE}")
endif()

# Hash curto
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --short HEAD
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# Hash completo
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_HASH_FULL
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# Branch
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# Tag
execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --tags --abbrev=0
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if("${GIT_TAG}" STREQUAL "")
    set(GIT_TAG "v0.0.0")
endif()

# Data do commit
execute_process(
    COMMAND "${GIT_EXECUTABLE}" log -1 --format=%cd --date=format:%Y-%m-%d
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# Dirty working tree
execute_process(
    COMMAND "${GIT_EXECUTABLE}" status --porcelain
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_STATUS
    ERROR_QUIET
)

if("${GIT_STATUS}" STREQUAL "")
    set(GIT_DIRTY false)
else()
    set(GIT_DIRTY true)
endif()

# Timestamp do build
string(TIMESTAMP BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S UTC" UTC)

# Lê template
file(READ "${PRE_CONFIGURE_FILE}" FILE_CONTENT)

# Substitui variáveis
string(REPLACE "@GIT_HASH@" "${GIT_HASH}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_HASH_FULL@" "${GIT_HASH_FULL}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_BRANCH@" "${GIT_BRANCH}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_TAG@" "${GIT_TAG}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_DATE@" "${GIT_DATE}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_DIRTY@" "${GIT_DIRTY}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@BUILD_TIMESTAMP@" "${BUILD_TIMESTAMP}" FILE_CONTENT "${FILE_CONTENT}")

# Verifica mudança antes de gravar
if(EXISTS "${POST_CONFIGURE_FILE}")
    file(READ "${POST_CONFIGURE_FILE}" OLD_CONTENT)
else()
    set(OLD_CONTENT "")
endif()

if(NOT "${FILE_CONTENT}" STREQUAL "${OLD_CONTENT}")
    file(WRITE "${POST_CONFIGURE_FILE}" "${FILE_CONTENT}")
    message(STATUS "Git version updated: ${GIT_HASH} (${GIT_BRANCH})")
endif()
