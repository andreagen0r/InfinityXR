project(InfinityApp
    VERSION 1.0.0
    LANGUAGES CXX
)

include(QtBootstrap)

list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}/style/")
list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_BINARY_DIR}/modules/")
list(APPEND QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/src/libs/")
set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
    CACHE STRING "Import paths for Qt Creator's code model"
    FORCE
)

# Herda outros packages do CMakeLists raiz
find_package(Qt6 REQUIRED COMPONENTS Qml Quick QuickControls2 Sql)

# -----------------------------------------------------------------------------
# Organização dos Fontes (Source Groups para IDEs)
# -----------------------------------------------------------------------------
set(SRC_BOOTSTRAP
    "bootstrap/appbootstrap.h"
    "bootstrap/appbootstrap.cpp"
    "bootstrap/authextensionhandler.h"
    "bootstrap/appbootstrapcontext.h"
    "bootstrap/ibootstrapinstallers.h"
    "bootstrap/installers/coreservicesinstaller.h"
    "bootstrap/installers/coreservicesinstaller.cpp"
    "bootstrap/installers/qmltypesinstaller.h"
    "bootstrap/installers/qmltypesinstaller.cpp"
)

set(SRC_SERVICES
    "services/screenhelper.h"
    "services/screenhelper.cpp"
    "services/systemsignalhandler.h"
    "services/systemsignalhandler.cpp"
)

# -----------------------------------------------------------------------------
# Executável
# -----------------------------------------------------------------------------
qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    main.cpp
    ${SRC_BOOTSTRAP}
)

# Permite adicionar sufixo aos nomes do executável e bibliotecas
set_postfix_to_target(${PROJECT_NAME})

# -----------------------------------------------------------------------------
# Módulo QML Principal (Views)
# -----------------------------------------------------------------------------
qt_add_qml_module(${PROJECT_NAME}
    URI "Infinity"
    VERSION 1.0
    NO_PLUGIN
    QML_FILES
        views/Main.qml
        views/PrimaryWindow.qml
        views/MenuBuilder.qml
        views/MenuItemLoader.qml
        views/DialogManager.qml
        views/PreferencesDialog.qml
        views/ToastItem.qml
        views/CircularProgress.qml
    SOURCES
        ${SRC_SERVICES}
    IMPORTS
        qone.SplashScreen
        QML_FILES views/NotificationOverlay.qml
)


qt_add_translations(${PROJECT_NAME}
    RESOURCE_PREFIX "/i18n"
    TS_FILE_DIR "${CMAKE_SOURCE_DIR}/translations"
)

# -----------------------------------------------------------------------------
# Subdiretórios (Módulos Internos)
# -----------------------------------------------------------------------------

add_subdirectory(modules/qone)


# -----------------------------------------------------------------------------
# Obtem dados do Git e ger o verion.hpp
# -----------------------------------------------------------------------------
git_versioning(Infinity)

# -----------------------------------------------------------------------------
# Linkagem e Propriedades
# -----------------------------------------------------------------------------

add_library(my_options INTERFACE)

set_project_options(my_options)

# TODO: Add this comment to the documentation
# enable_sanitizers(<target>  <INTERFACE|PUBLIC|PRIVATE>)
enable_sanitizers(my_options INTERFACE)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
    my_options # TODO: Add a comment into the documentation
    Infinity::Version # Gerado automaticamente pelo GitWatcher.cmake
    Infinity::SDK
    Infinity::Core
    SplashScreen_plugin
)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bootstrap
    ${CMAKE_CURRENT_SOURCE_DIR}/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/services
)

if(WIN32)
    set_target_properties(Infinity PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# -----------------------------------------------------------------------------
# Instalação (Binário apenas)
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# ---------------------------------------------------------------------------------------
# Deploy
# ---------------------------------------------------------------------------------------
include(CMakePackageConfigHelpers)

qt_generate_deploy_qml_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
)

install(SCRIPT ${deploy_script})


# ---------------------------------------------------------------------------------------
# Print what modules were found and save a log file
# ---------------------------------------------------------------------------------------
include(FeatureSummary)
feature_summary(WHAT ALL)
feature_summary(FILENAME ${CMAKE_BINARY_DIR}/features.log WHAT ALL)

qt_finalize_executable(${PROJECT_NAME})
