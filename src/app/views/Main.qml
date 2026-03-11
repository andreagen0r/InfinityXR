import QtQuick
import QtQuick.Controls.Basic

import qone.SplashScreen
import Infinity.Core

QtObject {
  id: root
  objectName: "root.app"

  property Window splashWindow: Splash {
    id: splashscreen
    objectName: "splashscreen"

    width: 960
    height: 540

    title: "Infinity"
    titleColor: Qt.color("orange")
    footerColor: Qt.color("darkgray")
    colorOverlay: Qt.alpha("#061322", 1.0)
    textSubTitle: qsTr("Microkernel example application")
    subTitleColor: Qt.alpha(Qt.color("white"), 0.5)
    version: qsTr(`Version 1.0.0`)
    source: Qt.resolvedUrl("qrc:/qone/SplashScreen/Splash")
    loadingText: "Authentication plugin"
    textNotes: "Created by André Luz, This application is under the MIT license."

    // Simulação de Load
    // TODO: Ligar ao PluginLoader
    NumberAnimation on progress {
      id: numAnim
      duration: 2000
      from: 0
      to: 100
      alwaysRunToEnd: true
    }

    onFinished: {
      root.splashWindow.destroy();
      SessionManager.finishInitialization();
    }
  }

  // Login
  property Loader authLoader: Loader {
    active: SessionManager.state === SessionState.Authenticating
    asynchronous: true

    onActiveChanged: {
      if (active) {
        console.debug("AuthLoader activated. Fetching source");
        const url = ViewRegistry.getViewSource("view.login");
        console.debug("Received URL:", url);
        source = url;
      }
    }

    onSourceChanged: console.debug("Source set to:", source)

    onStatusChanged: {
      if (status === Loader.Loading) {
        console.debug("Loading component");
      } else if (status === Loader.Ready) {
        console.debug("Component Ready!");
      } else if (status === Loader.Error) {
        console.error("Loader failed to load source:", source);
      }
    }

    onLoaded: {
      console.debug("Item loaded successfully. Showing...");
      item.showMaximized();
    }

    LoggingCategory {
      id: qml_main
      name: "qone.qml.main"
    }

    Connections {
      target: authLoader.item
      function onLoginSuccess() {
        console.debug(qml_main, "onLoginSuccess()");
        SessionManager.loginSuccess();
      }
    }
  }

  // MainWindow
  property Loader mainLoader: Loader {
    active: SessionManager.state === SessionState.Active

    source: Qt.resolvedUrl("PrimaryWindow.qml")

    onLoaded: {
      item.showMaximized();

      // Logoff
      if (item.hasOwnProperty("requestLogoff")) {
        item.requestLogoff.connect(() => {
          item.close();
          SessionManager.logout();
        });
      }
    }
  }
}
