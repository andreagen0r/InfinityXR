# Recebe os argumentos passados pelo comando -P
# PRE_CONFIGURE_FILE: O caminho do arquivo .in
# POST_CONFIGURE_FILE: O caminho do arquivo .hpp gerado
# GIT_EXECUTABLE: O caminho do binário git
# SOURCE_DIR: A raiz do repo

if(NOT EXISTS "${PRE_CONFIGURE_FILE}")
    message(FATAL_ERROR "GitWatcher: Template file not found: ${PRE_CONFIGURE_FILE}")
endif()

# 1. Pega o Hash
execute_process(
    COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# 2. Pega o Branch
execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# 3. Pega a Tag (se houver)
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

# 4. Pega a Data
execute_process(
    COMMAND "${GIT_EXECUTABLE}" log -1 --format=%cd --date=format:%Y-%m-%d
    WORKING_DIRECTORY "${SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

# 5. Configura o conteúdo em memória (sem gravar ainda)
#    Lê o template
file(READ "${PRE_CONFIGURE_FILE}" FILE_CONTENT)

#    Substitui as variáveis
string(REPLACE "@GIT_HASH@" "${GIT_HASH}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_BRANCH@" "${GIT_BRANCH}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_TAG@" "${GIT_TAG}" FILE_CONTENT "${FILE_CONTENT}")
string(REPLACE "@GIT_DATE@" "${GIT_DATE}" FILE_CONTENT "${FILE_CONTENT}")

# 6. Verifica se mudou antes de gravar (O PULO DO GATO)
if(EXISTS "${POST_CONFIGURE_FILE}")
    file(READ "${POST_CONFIGURE_FILE}" OLD_CONTENT)
else()
    set(OLD_CONTENT "")
endif()

if(NOT "${FILE_CONTENT}" STREQUAL "${OLD_CONTENT}")
    file(WRITE "${POST_CONFIGURE_FILE}" "${FILE_CONTENT}")
    message(STATUS "Git Version updated to: ${GIT_HASH} (${GIT_BRANCH})")
endif()