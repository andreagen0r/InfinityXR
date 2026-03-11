include_guard(GLOBAL)

function(add_git_versioning TARGET_NAME)

    find_package(Git QUIET)

    if(NOT GIT_FOUND)
        message(WARNING "Git not found. Version info disabled.")
        return()
    endif()

    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/generated")

    string(TOLOWER ${TARGET_NAME} TARGET_DIR)

    set(GENERATED_DIR "${OUTPUT_DIR}/${TARGET_DIR}")
    set(GENERATED_HEADER "${GENERATED_DIR}/version.hpp")

    file(MAKE_DIRECTORY "${GENERATED_DIR}")

    configure_file(
        ${GENERATED_HEADER}
        "${GENERATED_DIR}/version.hpp"
    )

    add_custom_command(
        OUTPUT ${GENERATED_HEADER}

        COMMAND ${CMAKE_COMMAND}
            -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
            -DSOURCE_DIR=${CMAKE_SOURCE_DIR}
            -DTEMPLATE_FILE=${CMAKE_CURRENT_LIST_DIR}/version.hpp.in
            -DOUTPUT_FILE=${GENERATED_HEADER}
            -DPROJECT_VERSION=${PROJECT_VERSION}
            -P ${CMAKE_CURRENT_LIST_DIR}/GenerateGitVersion.cmake

        DEPENDS
            ${CMAKE_CURRENT_LIST_DIR}/version.hpp.in
            ${CMAKE_CURRENT_LIST_DIR}/GenerateGitVersion.cmake

        COMMENT "Generating git version header"
        VERBATIM
    )

    add_custom_target(${TARGET_NAME}_version_header
        DEPENDS ${GENERATED_HEADER}
    )

    add_library(${TARGET_NAME} INTERFACE)
    add_library(${TARGET_NAME}::Version ALIAS ${TARGET_NAME})

    target_include_directories(${TARGET_NAME} INTERFACE
        "${OUTPUT_DIR}"
    )

    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_version_header)

endfunction()
