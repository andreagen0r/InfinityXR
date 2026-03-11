include_guard(GLOBAL)

function(library_generate_export_header TARGET_NAME)
    include(GenerateExportHeader)
    generate_export_header("${TARGET_NAME}")
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_export.h
        DESTINATION ${INCLUDE_INSTALL_DIR}/${TARGET_NAME})
    
    target_include_directories(${TARGET_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )
endfunction()
