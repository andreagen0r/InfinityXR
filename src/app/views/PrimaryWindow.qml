import QtQuick
import QtQuick.Layouts
import QtQuick.Dialogs

import QtQuick.Controls.Basic

import Infinity
import Infinity.Core

ApplicationWindow {
  id: mainWindow

  width: 1024
  height: 768
  title: qsTr("Infinity")

  font.pointSize: 10
  font.letterSpacing: 1.2

  menuBar: MenuBar {
    id: mainMenuBar
    objectName: "mainMenuBar"

    MenuBuilder {
      id: builder
    }

    Component.onCompleted: {
      console.debug(`Component.onCompleted: ${objectName}, MenuModel.rowCount: ${MenuModel.rowCount(MenuModel.root())}`);
      builder.buildMenuBar(mainMenuBar);
    }
  }

  DialogManager {}

  Label {
    anchors.centerIn: parent
    font.pointSize: 34
    text: qsTr("Host Application")
    font.bold: true
    opacity: 0.3
  }
}
