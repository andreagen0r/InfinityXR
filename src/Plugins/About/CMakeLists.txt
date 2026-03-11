cmake_minimum_required(VERSION 4.0)

project(About LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

qt_standard_project_setup()

# Define URI do módulo QML
set(PLUGIN_URI "Plugins.${PROJECT_NAME}")
# Converte pontos em barras para criar o caminho de instalação (Plugins/Monitor)
string(REPLACE "." "/" PLUGIN_PATH ${PLUGIN_URI})

qt_add_qml_module(${PROJECT_NAME}
    URI ${PLUGIN_URI}
    RESOURCE_PREFIX "/"
    CLASS_NAME ${PROJECT_NAME}
    # IMPORTANT:
    # Implementamos o Q_PLUGIN_METADATA manualmente em authplugin.h
    NO_GENERATE_PLUGIN_SOURCE
    # Habilitado pois é um plugin obrigatório, não opcional
    NO_PLUGIN_OPTIONAL
    NO_PLUGIN

    QML_FILES
        AboutDialog.qml

    SOURCES
        manifest.json
        aboutplugin.h
        aboutplugin.cpp
        showaboutcommand.h
        aboutmanifest.h
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Infinity::SDK
        Qt6::Core
        Qt6::Quick
)

if(NOT TARGET Infinity::SDK)
    message(FATAL_ERROR "Infinity::SDK target not found! Check your build order.")
endif()

# --- ESTRATÉGIA DE DEV: Enviar Plugin para o diretório do App ---
# O executável Infinity é gerado em ${CMAKE_BINARY_DIR}/src/app
# Criamos a pasta Plugins lá dentro.
set(APP_OUTPUT_DIR "${CMAKE_BINARY_DIR}/src/app")
set(PLUGIN_DEST_DIR "${APP_OUTPUT_DIR}/Plugins/${PROJECT_NAME}")

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DEST_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DEST_DIR}" # Para DLLs no Windows
    ARCHIVE_OUTPUT_DIRECTORY "${PLUGIN_DEST_DIR}"
)

# Instalação (Deploy final - mantém estrutura limpa)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION "bin/Plugins/${PROJECT_NAME}"
    RUNTIME DESTINATION "bin/Plugins/${PROJECT_NAME}"
    ARCHIVE DESTINATION "lib/Plugins/${PROJECT_NAME}"
)


