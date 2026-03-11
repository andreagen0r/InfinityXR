set_source_files_properties(assets/splash.jpg PROPERTIES QT_RESOURCE_ALIAS "Splash")

set(MODULE_NAME "SplashScreen")

qt_add_qml_module(${MODULE_NAME}_
    STATIC
    URI "qone.${MODULE_NAME}"
    RESOURCE_PREFIX /
    QML_FILES
        Splash.qml
    RESOURCES
        assets/splash.jpg
)

target_link_libraries(${MODULE_NAME}_
    PRIVATE
    Qt6::Qml
    Qt6::Quick
)
unset(MODULE_NAME)
