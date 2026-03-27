cmake_minimum_required(VERSION 4.0)

project(InfinityCore
    VERSION 1.0.0
    LANGUAGES CXX
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml LinguistTools Sql)

include(QtBootstrap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED)
add_library(Infinity::Core ALIAS ${PROJECT_NAME})

qt_add_translations(${PROJECT_NAME}
    RESOURCE_PREFIX "/i18n"
    TS_FILE_DIR "${CMAKE_SOURCE_DIR}/translations"
)

# -----------------------------------------------------------------------------
# Fontes (Implementação Privada)
# -----------------------------------------------------------------------------

# Kernel
set(SRC_KERNEL
    "src/kernel/pluginmanager.cpp"
    "src/kernel/pluginloader.cpp"
    "src/kernel/pluginscanner.cpp"
    "src/kernel/plugininstance.cpp"
    "src/kernel/viewextensionhandler.cpp"
    "src/kernel/commandextensionhandler.cpp"
    "src/kernel/contextextensionhandler.cpp"
    "src/kernel/preferencesextensionhandler.cpp"
)

# UI
set(SRC_UI
    "src/ui/viewregistry.cpp"
    "src/ui/sessionmanager.cpp"
    "src/ui/menumodel.cpp"
    "src/ui/preferencesmenumodel.cpp"
    "src/ui/pluginlistmodel.cpp"
    "src/ui/notificationmodel.cpp"
    "src/ui/notificationfiltermodel.cpp"
)

# Interaction
set(SRC_INTERACTION
    "src/interaction/actiondispatcher.cpp"
    "src/interaction/commandregistry.cpp"
)

# Services
set(SRC_SERVICES
    "src/services/settingsservice.cpp"
    "src/services/qsettingsbackend.cpp"
    "src/services/serviceregistry.cpp"
    "src/services/dialogservice.cpp"
    "src/services/preferencesregistry.cpp"
    "src/services/httpservice.cpp"
    "src/services/sqlitenotificationrepository.cpp"
    "src/services/notificationservice.cpp"
)

# messaging
set(SRC_MESSAGING
    "src/messaging/eventbus.cpp"
)
# -----------------------------------------------------------------------------
# Headers (Interface Pública)
# -----------------------------------------------------------------------------

set(HDR_KERNEL
    "include/Infinity/core/kernel/pluginmanager.h"
    "include/Infinity/core/kernel/pluginloader.h"
    "include/Infinity/core/kernel/pluginscanner.h"
    "include/Infinity/core/kernel/plugininstance.h"
    "include/Infinity/core/kernel/IPluginExtensionHandler.h"
    "include/Infinity/core/kernel/viewextensionhandler.h"
    "include/Infinity/core/kernel/commandextensionhandler.h"
    "include/Infinity/core/kernel/contextextensionhandler.h"
    "include/Infinity/core/kernel/preferencesextensionhandler.h"
)

set(HDR_UI
    "include/Infinity/core/ui/viewregistry.h"
    "include/Infinity/core/ui/sessionmanager.h"
    "include/Infinity/core/ui/dialogregistry.h"
    "include/Infinity/core/ui/menumodel.h"
    "include/Infinity/core/ui/menunode_p.h"
    "include/Infinity/core/ui/dialogservice.h"
    "include/Infinity/core/ui/preferencesmenumodel.h"
    "include/Infinity/core/ui/systemuimanifest.h"
    "include/Infinity/core/ui/pluginlistmodel.h"
    "include/Infinity/core/ui/notificationmodel.h"
    "include/Infinity/core/ui/notificationfiltermodel.h"
)

set(HDR_INTERACTION
    "include/Infinity/core/interaction/actiondispatcher.h"
    "include/Infinity/core/interaction/commandregistry.h"
    "include/Infinity/core/interaction/showpreferencescommand.h"
)

set(HDR_SERVICES
    "include/Infinity/core/services/settingsservice.h"
    "include/Infinity/core/services/qsettingsbackend.h"
    "include/Infinity/core/services/serviceregistry.h"
    "include/Infinity/core/services/preferencesregistry.h"
    "include/Infinity/core/services/httpservice.h"
    "include/Infinity/core/services/sqlitenotificationrepository.h"
    "include/Infinity/core/services/notificationservice.h"
)

set(HDR_UTILS
    "include/Infinity/core/utils/singleton.hpp"
    "include/Infinity/core/utils/enumUtils.hpp"
)

# messaging
set(HDR_MESSAGING
    "include/Infinity/core/messaging/eventbus.h"
)

# -----------------------------------------------------------------------------
# Esse módulo não é usado, é apenas um hack para o Code Model conseguir
# ver os imports no formato "import Infinity.Core"
# -----------------------------------------------------------------------------
qt_add_qml_module(${PROJECT_NAME}
    URI "Infinity.Core"
    VERSION 1.0
    OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/libs/Infinity/Core
    NO_PLUGIN
    SOURCES
        ${SRC_KERNEL}
        ${SRC_UI}
        ${SRC_INTERACTION}
        ${SRC_SERVICES}
        ${HDR_KERNEL}
        ${HDR_UI}
        ${HDR_INTERACTION}
        ${HDR_SERVICES}
        ${HDR_UTILS}
        ${HDR_MESSAGING}
)

# -----------------------------------------------------------------------------
# Configuração do Target
# -----------------------------------------------------------------------------
target_sources(${PROJECT_NAME}
    PRIVATE
        ${SRC_KERNEL}
        ${SRC_UI}
        ${SRC_INTERACTION}
        ${SRC_SERVICES}
        ${SRC_MESSAGING}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
        FILES
            ${HDR_KERNEL}
            ${HDR_UI}
            ${HDR_INTERACTION}
            ${HDR_SERVICES}
            ${HDR_UTILS}
            ${HDR_MESSAGING}
)

# -----------------------------------------------------------------------------
# Export Header
# -----------------------------------------------------------------------------
include(GenerateExportHeader)
set(CORE_EXPORT_DIR "${CMAKE_CURRENT_BINARY_DIR}/include/Infinity/core")
file(MAKE_DIRECTORY ${CORE_EXPORT_DIR})

generate_export_header(${PROJECT_NAME}
    BASE_NAME core
    EXPORT_FILE_NAME "${CORE_EXPORT_DIR}/core_export.h"
)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include"
        FILES "${CORE_EXPORT_DIR}/core_export.h"
)


# -----------------------------------------------------------------------------
# Includes e Links
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Infinity::SDK
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Sql
)


set_target_properties(${PROJECT_NAME} PROPERTIES
    VERIFY_INTERFACE_HEADER_SETS ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Define CORE_LIBRARY para ativar o Q_DECL_EXPORT no Windows
target_compile_definitions(${PROJECT_NAME} PRIVATE CORE_LIBRARY)

# -----------------------------------------------------------------------------
# Instalação
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT_NAME}
    EXPORT InfinityTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION include
)
