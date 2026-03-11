function(validate_scope SCOPE)

    set(_valid_scopes PRIVATE PUBLIC INTERFACE)
    list(FIND _valid_scopes "${SCOPE}" _scope_index)
    if(_scope_index EQUAL -1)
        message(FATAL_ERROR "Invalid scope '${SCOPE}'. Use PRIVATE, PUBLIC or INTERFACE.")
    endif()

endfunction()

