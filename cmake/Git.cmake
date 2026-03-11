include_guard(GLOBAL)

function(git_versioning TARGET_NAME)

    find_package(Git QUIET)

    set(LOCAL_TARGET_NAME "${TARGET_NAME}Version")

    if(NOT GIT_FOUND)
        message(WARNING "Git not found! Version info will be empty.")
        return()
    endif()

    # Define caminhos
    set(VERSION_TEMPLATE "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/version.hpp.in")
    set(VERSION_HEADER "${CMAKE_BINARY_DIR}/generated/include/${TARGET_NAME}/version.hpp")
    set(GIT_WATCHER_SCRIPT "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/GitWatcher.cmake")

    # Cria a pasta de saída
    get_filename_component(VERSION_HEADER_DIR ${VERSION_HEADER} DIRECTORY)
    file(MAKE_DIRECTORY ${VERSION_HEADER_DIR})

    # Cria um alvo que RODA SEMPRE (ALL) para checar o git
    # O script GitWatcher.cmake é quem decide se toca no arquivo ou não.
    add_custom_target(git_version_update
        COMMAND ${CMAKE_COMMAND}
            -DSOURCE_DIR=${CMAKE_SOURCE_DIR}
            -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
            -DPRE_CONFIGURE_FILE=${VERSION_TEMPLATE}
            -DPOST_CONFIGURE_FILE=${VERSION_HEADER}
            -P ${GIT_WATCHER_SCRIPT}
        BYPRODUCTS ${VERSION_HEADER}
        COMMENT "Checking Git version info..."
        VERBATIM
    )

    # Cria uma Interface Library para expor o header gerado
    add_library(${LOCAL_TARGET_NAME} INTERFACE)
    add_library("${TARGET_NAME}::Version" ALIAS ${LOCAL_TARGET_NAME})

    # Adiciona o diretório de include gerado
    target_include_directories(${LOCAL_TARGET_NAME} INTERFACE
        "${CMAKE_BINARY_DIR}/generated/include"
    )

    # Garante que o header exista antes de compilar quem usa essa lib
    add_dependencies(${LOCAL_TARGET_NAME} git_version_update)

    message(STATUS "Git versioning system initialized.")

    unset(LOCAL_TARGET_NAME)

endfunction()
