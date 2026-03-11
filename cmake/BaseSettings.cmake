include_guard(GLOBAL)

function(set_project_options TARGET_NAME)

  message(STATUS "Project Options for target: " ${TARGET_NAME})

  # ----------------------------------------------
  # SCOPE DETECTION
  # ----------------------------------------------
  get_target_property(target_type ${TARGET_NAME} TYPE)

  set(is_interface_library  target_type STREQUAL "INTERFACE_LIBRARY")
  
  if(is_interface_library)
  set(SCOPE INTERFACE)
  else()
  set(SCOPE PRIVATE)
  endif()
  

  # ----------------------------------------------
  # Warnings
  # ----------------------------------------------
  target_compile_definitions(${TARGET_NAME}
      ${SCOPE}
          $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
          $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>
  )

  # ----------------------------------------------
  # Functions call
  # ----------------------------------------------
  if(ENABLE_WARNINGS)
    set_project_warnings(${TARGET_NAME})
  endif()


  include(StaticAnalyzers)
  enable_static_analysis(${TARGET_NAME})

  # ----------------------------------------------
  # IPO / LTO
  # ----------------------------------------------

  if(ENABLE_IPO AND NOT is_interface_library)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
      message(SEND_ERROR "IPO is not supported: ${output}")
    endif()
  endif()


  # ----------------------------------------------
  # PCH
  # ----------------------------------------------
  if(ENABLE_PCH AND NOT is_interface_library)
    message(STATUS "Compiling using pre-compiled header support ${ENABLE_PCH}")
    target_precompile_headers(${TARGET_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../src/pch.hpp)
    set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
  endif()

  # ----------------------------------------------
  # TRACE TIME
  # ----------------------------------------------
  if(ENABLE_BUILD_WITH_TIME_TRACE)
    target_compile_options(${TARGET_NAME} INTERFACE -ftime-trace)
  endif()

  # ----------------------------------------------
  # CPU ISTRUCTION MARCH NATIVE
  # ----------------------------------------------
  if(ENABLE_MARCH_NATIVE AND NOT is_interface_library)
    if(MSVC)
       # MSVC uses /arch:AVX2 etc., not -march=native directly in the same way.
    else()
       target_compile_options(${TARGET_NAME} PRIVATE -march=native)
    endif()
    
    message(STATUS "Enabling march=native for ${TARGET_NAME}")
  endif()

  # ----------------------------------------------
  # Color output
  # ----------------------------------------------
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM")
    add_compile_options(-fcolor-diagnostics)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
  else()
    message(WARNING "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
  endif()

  # ----------------------------------------------
  # CACHE
  # ----------------------------------------------
  if(ENABLE_CACHE)
    set(CACHE_OPTION "ccache" CACHE STRING "Compiler cache to be used")
    set(CACHE_OPTION_VALUES "ccache" "sccache")
    set_property(CACHE CACHE_OPTION PROPERTY STRINGS ${CACHE_OPTION_VALUES})

    list(FIND CACHE_OPTION_VALUES ${CACHE_OPTION} CACHE_OPTION_INDEX)

    if(${CACHE_OPTION_INDEX} EQUAL -1)
      message(STATUS "Using custom compiler cache system: '${CACHE_OPTION}', explicitly supported entries are ${CACHE_OPTION_VALUES}")
    endif()

    find_program(CACHE_BINARY ${CACHE_OPTION})

    if(CACHE_BINARY)
      message(STATUS "${CACHE_OPTION} found and enabled")
      set(CMAKE_CXX_COMPILER_LAUNCHER ${CACHE_BINARY})
    else()
      message(WARNING "${CACHE_OPTION} is enabled but was not found. Not using it")
    endif()
  endif()

endfunction()

# ----------------------------------------------
# POSTFIX
# ----------------------------------------------
function(set_postfix_to_target TARGET_NAME)

  message(STATUS "Set postfix to target ${TARGET_NAME}")

  get_target_property(target_type ${TARGET_NAME} TYPE)
  
  if(target_type STREQUAL "INTERFACE_LIBRARY")
    message(FATAL_ERROR "Postfix can't be applied to INTERFACE libraries.")
  else()
    set(STATIC_POSTFIX "")
  
    if(NOT BUILD_SHARED_LIBS)
      set(STATIC_POSTFIX "_s")

      string(TOUPPER "${TARGET_NAME}" TARGET_NAME_UPPER)
      string(REPLACE "-" "_" TARGET_NAME_UPPER "${TARGET_NAME_UPPER}")
  
      target_compile_definitions(${TARGET_NAME} PRIVATE ${TARGET_NAME_UPPER}_STATIC_DEFINE)
  
    endif()
  
    set_target_properties(${TARGET_NAME} PROPERTIES
      DEBUG_POSTFIX "${STATIC_POSTFIX}_d"
      RELEASE_POSTFIX "${STATIC_POSTFIX}"
      MINSIZEREL_POSTFIX "${STATIC_POSTFIX}_mr"
      RELWITHDEBINFO_POSTFIX "${STATIC_POSTFIX}_rd"
    )
  endif()

endfunction()
