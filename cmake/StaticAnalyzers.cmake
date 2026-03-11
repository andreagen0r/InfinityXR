include_guard(GLOBAL)

function(enable_static_analysis TARGET_NAME)
    # ----------------------------------------------
    # CLANG-TIDY (Integrado à compilação)
    # ----------------------------------------------
    if(ENABLE_CLANG_TIDY)
        find_program(CLANG_TIDY_PROGRAM NAMES clang-tidy)
        if(CLANG_TIDY_PROGRAM)
            # Constrói o comando: clang-tidy; -p; build_dir
            set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM};-p;${CMAKE_BINARY_DIR}")
            
            # Aplica ao target. O CMake roda o tidy automaticamente ao compilar cada arquivo.
            set_target_properties(${TARGET_NAME} PROPERTIES CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
            message(STATUS "Clang-Tidy enabled for ${TARGET_NAME}")
        else()
            message(WARNING "Clang-Tidy requested but not found.")
        endif()
    endif()

    # ----------------------------------------------
    # CPPCHECK (Integrado à compilação)
    # ----------------------------------------------
    if(ENABLE_CPPCHECK)
        find_program(CPPCHECK_PROGRAM NAMES cppcheck)
        if(CPPCHECK_PROGRAM)
            set(CMAKE_CXX_CPPCHECK 
                "${CPPCHECK_PROGRAM}"
                "--enable=warning,style,performance,portability,information,unusedFunction"
                "--inconclusive"
                "--suppress=missingInclude" # Otimização: ignora includes de sistema
                "--template=gcc"
                "--library=qt"
                "--xml"
                "--xml-version=2"
                "--output-file=cppcheck.xml"
            )
            set_target_properties(${TARGET_NAME} PROPERTIES CXX_CPPCHECK "${CMAKE_CXX_CPPCHECK}")
            message(STATUS "Cppcheck enabled for ${TARGET_NAME}")
        endif()
    endif()

    # ----------------------------------------------
    # IWYU
    # ----------------------------------------------
    if(ENABLE_INCLUDE_WHAT_YOU_USE)
        find_program(IWYU_PROGRAM NAMES include-what-you-use iwyu)
        if(IWYU_PROGRAM)
            set_target_properties(${TARGET_NAME} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}")
        endif()
    endif()

endfunction()


# ----------------------------------------------
# CLANG_FORMAT
# ----------------------------------------------
if(ENABLE_CLANG_FORMAT)

  find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format
    HINTS ${LLVM_TOOLS_BINARY_DIR}
  )

  if(CLANG_FORMAT_EXECUTABLE)

    add_custom_target(clang-format
      COMMAND ${CLANG_FORMAT_EXECUTABLE}
      -i
      -style=file
      ${ALL_CXX_SOURCE_FILES}
      COMMENT "Running clang-format on ${CMAKE_SOURCE_DIR}"
      VERBATIM
    )

  else()
    message(WARNING "clang-format not found! Formatting disabled.")
  endif()

endif()
