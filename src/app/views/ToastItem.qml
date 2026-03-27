import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Infinity.Core

FocusScope {
  id: control
  height: contentLayout.implicitHeight + 20

  property string notificationId
  property int notificationSeverity
  property string title
  property string text
  property int timeout
  property bool actionRequired
  property var actionsList

  signal timeoutReached
  signal userDismissed
  signal actionTriggered

  QtObject {
    id: _private

    property real elapsedMs: 0
    property real progressValue: 1.0
  }

  Timer {
    id: progressTimer
    interval: 16
    running: !control.actionRequired && control.timeout > 0
    repeat: true

    onTriggered: {
      _private.elapsedMs += interval;
      _private.progressValue = Math.max(0, 1.0 - (_private.elapsedMs / control.timeout));

      if (_private.elapsedMs >= control.timeout) {
        progressTimer.stop();
        control.timeoutReached();
      }
    }
  }

  Rectangle {
    anchors.fill: parent
    radius: 6

    color: {
      switch (control.notificationSeverity) {
      case 0:
        return Qt.color("#d1ecf1"); // Info
      case 1:
        return Qt.color("#d4edda"); // Success
      case 2:
        return Qt.color("#fff3cd"); // Warning
      case 3:
        return Qt.color("#f8d7da"); // Critical
      default:
        return Qt.color("lightgray");
      }
    }
    border.color: Qt.darker(color, 1.1)

    RowLayout {
      id: contentLayout
      anchors.fill: parent
      anchors.margins: 10
      spacing: 8

      ColumnLayout {
        Layout.fillWidth: true
        spacing: 4

        Label {
          text: control.title
          font.bold: true
          visible: text !== ""
          color: "#333"
        }

        Label {
          text: control.text
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          color: "#333"
        }

        // Geração dinâmica de Ações
        RowLayout {
          visible: control.actionsList && control.actionsList.length > 0
          Repeater {
            model: control.actionsList
            delegate: Button {
              text: modelData.label
              onClicked: {
                ActionDispatcher.execute(modelData.commandId, modelData.args);
                control.actionTriggered();
              }
            }
          }
        }
      }

      Item {
        Layout.preferredHeight: 35
        Layout.preferredWidth: 35
        Layout.alignment: Qt.AlignTop

        CircularProgress {
          anchors.fill: parent
          visible: !control.actionRequired && control.timeout > 0
          value: _private.progressValue
          strokeWidth: 4
          color: "white"
          trackColor: Qt.lighter(parent.parent.color, 0.7)
          showText: false
        }

        Button {
          anchors.centerIn: parent
          text: "✕"
          flat: true
          background: Item {}
          onClicked: control.userDismissed()
        }
      }
    }
  }
}
