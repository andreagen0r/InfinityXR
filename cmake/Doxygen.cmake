include_guard(GLOBAL)

find_package(Doxygen REQUIRED)

find_program(DOT_PATH dot)
if(NOT DOT_PATH)
    message(FATAL_ERROR "Graphviz 'dot' not found! Please install Graphviz.")
endif()

if(BUILD_DOCUMENTATION)

    if(DOXYGEN_FOUND)

        if(NOT DOXYGEN_AWESOME_VERSION)
            set(DOXYGEN_AWESOME_VERSION "v2.3.4")
        endif()

        set(DOXYGEN_THEME_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen-awesome")
        file(MAKE_DIRECTORY ${DOXYGEN_THEME_ASSETS_DIR})

        set(CSS_FILES
            "doxygen-awesome.css"
            "doxygen-awesome-sidebar-only.css"
        )
        set(JS_FILES
            "doxygen-awesome-darkmode-toggle.js"
        )

        message(STATUS "Configuring Doxygen with doxygen-awesome-css version ${DOXYGEN_AWESOME_VERSION}")

        set(local_extra_stylesheets "")
        set(local_extra_js_files "")

        # Download dos arquivos CSS
        foreach(file_name IN LISTS CSS_FILES)
            set(target_file_path "${DOXYGEN_THEME_ASSETS_DIR}/${file_name}")
            if(NOT EXISTS ${target_file_path})
                message(STATUS "Downloading ${file_name} to ${target_file_path}")
                file(DOWNLOAD "https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/${DOXYGEN_AWESOME_VERSION}/${file_name}"
                    "${target_file_path}"
                    SHOW_PROGRESS
                    TIMEOUT 15 # Timeout em segundos
                    STATUS download_status LOG ${CMAKE_BINARY_DIR}/doxygen_theme_download.log)

                list(GET download_status 0 download_error_code)
                if(NOT download_error_code EQUAL 0)
                    list(GET download_status 1 download_error_message)
                    message(WARNING "Failed to download ${file_name}: [${download_error_code}] ${download_error_message}. Documentation might not use the custom theme correctly.")
                endif()
            endif()
            if(EXISTS ${target_file_path})
                list(APPEND local_extra_stylesheets "${target_file_path}")
            endif()
        endforeach()

        foreach(file_name IN LISTS JS_FILES)
            set(target_file_path "${DOXYGEN_THEME_ASSETS_DIR}/${file_name}")
            if(NOT EXISTS ${target_file_path})
                message(STATUS "Downloading ${file_name} to ${target_file_path}")
                file(DOWNLOAD "https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/${DOXYGEN_AWESOME_VERSION}/${file_name}"
                    "${target_file_path}"
                    SHOW_PROGRESS
                    TIMEOUT 15 # Timeout em segundos
                    STATUS download_status LOG ${CMAKE_BINARY_DIR}/doxygen_theme_download.log)

                list(GET download_status 0 download_error_code)
                if(NOT download_error_code EQUAL 0)
                    list(GET download_status 1 download_error_message)
                    message(WARNING "Failed to download ${file_name}: [${download_error_code}] ${download_error_message}. Documentation might not use the custom theme correctly.")
                endif()
            endif()
            if(EXISTS ${target_file_path})
                list(APPEND local_extra_stylesheets "${target_file_path}")
            endif()
        endforeach()

        set(DOXYGEN_GENERATE_HTML YES)
        set(DOXYGEN_HTML_OUTPUT ${CMAKE_BINARY_DIR}/docs)
        set(DOXYGEN_GENERATE_TREEVIEW YES)
        set(DOXYGEN_DISABLE_INDEX NO)
        set(DOXYGEN_FULL_SIDEBAR NO)
        set(DOXYGEN_EXCLUDE_PATTERNS */build/*)
        set(DOXYGEN_DOT_PATH ${DOT_PATH})
        set(DOXYGEN_HAVE_DOT YES)

        # list(APPEND DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/docs/resources/doxygen-awesome.css)
        # list(APPEND DOXYGEN_HTML_EXTRA_STYLESHEET ${CMAKE_CURRENT_SOURCE_DIR}/docs/resources/doxygen-awesome-sidebar-only.css)

        if(local_extra_stylesheets)
            set(DOXYGEN_HTML_EXTRA_STYLESHEET ${local_extra_stylesheets})
        endif()
        if(local_extra_js_files)
            set(DOXYGEN_HTML_EXTRA_FILES ${local_extra_js_files})
        endif()

        doxygen_add_docs(documentation ${DOCUMENTATION_SOURCE_DIR} COMMENT "Generating documentation")

    else()
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()


endif()
