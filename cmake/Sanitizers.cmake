include_guard(GLOBAL)

function(enable_sanitizers TARGET_NAME)

  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "^(GNU|Clang|IntelLLVM)$")
    return()
  endif()

  set(SCOPE "")
  if(ARGC GREATER 1)
    list(GET ARGV 1 SCOPE)
  endif()

  if(NOT SCOPE OR SCOPE STREQUAL "")
    set(SCOPE "INTERFACE")
  endif()

  validate_scope(${SCOPE})

  if(ENABLE_COVERAGE)
    target_compile_options(${TARGET_NAME} ${SCOPE} --coverage -O0 -g)
    target_link_libraries(${TARGET_NAME} ${SCOPE} --coverage)
  endif()

  set(SANITIZERS "")

  if(ENABLE_SANITIZER_ADDRESS)
    list(APPEND SANITIZERS "address")
  endif()

  if(ENABLE_SANITIZER_LEAK)
    list(APPEND SANITIZERS "leak")
  endif()

  if(ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
    list(APPEND SANITIZERS "undefined")
  endif()

  if(ENABLE_SANITIZER_THREAD)
    if("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
      message(WARNING "Thread sanitizer does not work with Address and Leak sanitizer enabled")
    else()
      list(APPEND SANITIZERS "thread")
    endif()
  endif()

  if(ENABLE_SANITIZER_MEMORY AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(WARNING "Memory sanitizer requires all the code (including libc++) to be MSan-instrumented otherwise it reports false positives")

    if("address" IN_LIST SANITIZERS OR "thread" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
      message(WARNING "Memory sanitizer does not work with Address, Thread and Leak sanitizer enabled")
    else()
      list(APPEND SANITIZERS "memory")
    endif()
  endif()

  list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

  if(LIST_OF_SANITIZERS)
    if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
      # target_compile_options(${TARGET_NAME} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
      target_link_options(${TARGET_NAME} ${SCOPE} -fsanitize=${LIST_OF_SANITIZERS})
    endif()
  endif()

endfunction()
