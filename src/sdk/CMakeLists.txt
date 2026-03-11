cmake_minimum_required(VERSION 4.0)

project(InfinitySDK LANGUAGES CXX)

find_package(spdlog CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui)

message(STATUS "SDK Found Qt Version ${Qt6_VERSION_MAJOR}.${Qt6_VERSION_MINOR}.${Qt6_VERSION_PATCH}")

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDK_VERSION_MAJOR 1)
set(SDK_VERSION_MINOR 0)
set(SDK_VERSION_PATCH 0)

# ---------------------------------------------------------------------------------------
# Definição da Biblioteca
# ---------------------------------------------------------------------------------------
add_library(${PROJECT_NAME} SHARED)
add_library(Infinity::SDK ALIAS ${PROJECT_NAME})

# ---------------------------------------------------------------------------------------
# LOG Categories
# Write file .h and .cpp with template names
# ---------------------------------------------------------------------------------------
set(LOGGING_CATEGORIES "")
list(APPEND LOGGING_CATEGORIES "Sdk")
list(APPEND LOGGING_CATEGORIES "SdkEventBus")
list(APPEND LOGGING_CATEGORIES "SdkPlugin")
list(APPEND LOGGING_CATEGORIES "SdkAuth")
list(APPEND LOGGING_CATEGORIES "SdkHttp")
list(APPEND LOGGING_CATEGORIES "SdkCommand")

list(APPEND LOGGING_CATEGORIES "Core")
list(APPEND LOGGING_CATEGORIES "Network")
list(APPEND LOGGING_CATEGORIES "Models")
list(APPEND LOGGING_CATEGORIES "Style")
list(APPEND LOGGING_CATEGORIES "App")
list(APPEND LOGGING_CATEGORIES "qml")


list(REMOVE_DUPLICATES LOGGING_CATEGORIES)

set(LOG_CATEGORY_H "")
set(LOG_CATEGORY_CPP "")
set(LOG_CATEGORY_LIST "")

foreach(IDX ${LOGGING_CATEGORIES})
    set( LOG_CATEGORY_H "${LOG_CATEGORY_H}Q_DECLARE_LOGGING_CATEGORY( qone${IDX} )\n")
    set( LOG_CATEGORY_CPP "${LOG_CATEGORY_CPP}Q_LOGGING_CATEGORY( qone${IDX}, \"qone.${IDX}\" )\n")
    set( LOG_CATEGORY_LIST "${LOG_CATEGORY_LIST}\"${IDX}\"_L1, ")
endforeach()

# ---------------------------------------------------------------------------------------
# Configuração de templates
# ---------------------------------------------------------------------------------------
set(SDK_BUILD_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${SDK_BUILD_INCLUDE_DIR}/Infinity/sdk")

set(SDK_BASE_SOURCE_DIR "/include/Infinity/sdk")
set(SDK_INTERFACES_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}${SDK_BASE_SOURCE_DIR}/contracts")
set(SDK_TYPES_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}${SDK_BASE_SOURCE_DIR}/types")
set(SDK_LOGGING_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}${SDK_BASE_SOURCE_DIR}/logging")
set(SDK_MESSAGING_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}${SDK_BASE_SOURCE_DIR}/messaging")

set(SDK_TEMPLATES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/templates")

configure_file(
    ${SDK_TEMPLATES_SOURCE_DIR}/sdkVersion.h.in
    ${CMAKE_CURRENT_BINARY_DIR}${SDK_BASE_SOURCE_DIR}/sdkVersion.h
)

configure_file(
    ${SDK_TEMPLATES_SOURCE_DIR}/logger.h.in
    "${CMAKE_CURRENT_BINARY_DIR}${SDK_BASE_SOURCE_DIR}/logging/logger.h"
)

configure_file(
    ${SDK_TEMPLATES_SOURCE_DIR}/logger.cpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/logging/logger.cpp"
)

set(SDK_HEADERS_INTERFACES
    "${SDK_INTERFACES_BASE_DIR}/ISettingsService.h"
    "${SDK_INTERFACES_BASE_DIR}/IViewContributor.h"
    "${SDK_INTERFACES_BASE_DIR}/IViewFactory.h"
    "${SDK_INTERFACES_BASE_DIR}/IPlugin.h"
    "${SDK_INTERFACES_BASE_DIR}/ICommandRegistry.h"
    "${SDK_INTERFACES_BASE_DIR}/IAuthService.h"
    "${SDK_INTERFACES_BASE_DIR}/IServiceRegistry.h"
    "${SDK_INTERFACES_BASE_DIR}/ICommandContributor.h"
    "${SDK_INTERFACES_BASE_DIR}/IDialogService.h"
    "${SDK_INTERFACES_BASE_DIR}/IRequiresContext.h"
    "${SDK_INTERFACES_BASE_DIR}/IPreferencesContributor.h"
    "${SDK_INTERFACES_BASE_DIR}/PluginEvents.h"
    "${SDK_INTERFACES_BASE_DIR}/AuthEvents.h"
    "${SDK_INTERFACES_BASE_DIR}/IEventBusService.h"
    "${SDK_INTERFACES_BASE_DIR}/IEventPublisher.h"
    "${SDK_INTERFACES_BASE_DIR}/IEventSubscriber.h"
    "${SDK_INTERFACES_BASE_DIR}/IEventBus.h"
    "${SDK_INTERFACES_BASE_DIR}/IHttpService.h"
    )

set(SDK_HEADERS_TYPES
    "${SDK_TYPES_BASE_DIR}/PluginDescriptor.h"
    "${SDK_TYPES_BASE_DIR}/ActionDescriptor.h"
    "${SDK_TYPES_BASE_DIR}/CommandDescriptor.h"
    "${SDK_TYPES_BASE_DIR}/ViewDescriptor.h"
    "${SDK_TYPES_BASE_DIR}/PreferenceDescriptor.h"

    "${SDK_TYPES_BASE_DIR}/Version.h"
    "${SDK_TYPES_BASE_DIR}/Types.h"
    "${SDK_TYPES_BASE_DIR}/CommandId.h"
    "${SDK_TYPES_BASE_DIR}/ViewId.h"
    "${SDK_TYPES_BASE_DIR}/EventSubscription.h"
)


target_sources(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}/logging/logger.cpp
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_BINARY_DIR}/include
        FILES
            ${SDK_HEADERS_INTERFACES}
            ${SDK_HEADERS_TYPES}
            ${CMAKE_CURRENT_BINARY_DIR}${SDK_BASE_SOURCE_DIR}/sdkVersion.h
)

# ---------------------------------------------------------------------------------------
# Configuração de Exportação (API)
# ---------------------------------------------------------------------------------------
include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
    BASE_NAME infinitysdk
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/Infinity/sdk/infinitysdk_export.h"
)

# Agora adicionamos esse arquivo gerado manualmente ao target
target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/include"
        FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Infinity/sdk/infinitysdk_export.h"
)


get_target_property(hdrs ${PROJECT_NAME} HEADER_SET)
message(STATUS "Interface headers: ${hdrs}")
# ---------------------------------------------------------------------------------------
# Includes e Links
# ---------------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
    PUBLIC
    # Permite que quem compila a SDK encontre os headers gerados
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    # Permite que quem compila a SDK encontre os headers fonte
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # Permite que quem INSTALA a SDK encontre os headers na pasta de instalação
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt6::Core
        Qt6::Gui
    PRIVATE
        spdlog::spdlog
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERIFY_INTERFACE_HEADER_SETS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# ---------------------------------------------------------------------------------------
# Instalação
# ---------------------------------------------------------------------------------------
install(TARGETS ${PROJECT_NAME}
    EXPORT InfinityTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION include
)

install(EXPORT InfinityTargets
    FILE InfinitySDKTargets.cmake
    NAMESPACE Infinity::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/InfinitySDK
)

