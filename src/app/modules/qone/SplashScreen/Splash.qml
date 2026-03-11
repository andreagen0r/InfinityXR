import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

import QtQuick.Controls.Basic

Window {
  id: control
  objectName: "SplashScreen"

  visible: true
  color: "transparent"
  modality: Qt.ApplicationModal
  flags: Qt.SplashScreen | Qt.WindowStaysOnTopHint
  screen: Qt.application.screens[0]

  property string loadingText: ""
  property color colorOverlay: Qt.color("white")
  property alias titleColor: title.color
  property alias subTitleColor: subTitle.color
  property alias textSubTitle: subTitle.text
  property alias footerColor: footerBackground.color
  property alias textNotes: legalNotes.text
  property alias source: backgroundImage.source
  property alias progress: progressBar.value
  property alias version: version.text

  signal finished

  Item {
    objectName: "splashscreen_shadow_padding"

    anchors.fill: parent
    anchors.margins: shadow.blur + shadow.offset.x + shadow.offset.y

    RectangularShadow {
      id: shadow
      objectName: "splashscreen_window_shadow"

      anchors.fill: parent

      radius: 16
      blur: 32
      offset.y: 8

      ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Item {
          Layout.fillWidth: true
          Layout.fillHeight: true

          Image {
            id: backgroundImage
            objectName: "splashscreen_background_image"
            anchors.fill: parent
            visible: false
            fillMode: Image.PreserveAspectCrop
          }

          MultiEffect {
            source: backgroundImage
            anchors.fill: parent
            maskEnabled: true
            maskSource: mask
            maskThresholdMin: 0.5
            maskSpreadAtMin: 1.0
          }

          Item {
            id: mask
            width: parent.width
            height: parent.height
            layer.enabled: true
            layer.smooth: true

            visible: false
            antialiasing: true

            Rectangle {
              width: parent.width
              height: parent.height

              antialiasing: true
              topLeftRadius: shadow.radius
              topRightRadius: shadow.radius
              color: "black"
            }
          }

          Rectangle {
            objectName: "spashscreen_image_overlay"

            anchors.fill: parent

            topLeftRadius: shadow.radius
            topRightRadius: shadow.radius
            gradient: Gradient {
              orientation: Gradient.Horizontal
              GradientStop {
                position: 0.4
                color: control.colorOverlay
              }
              GradientStop {
                position: 0.9
                color: Qt.color("transparent")
              }
            }

            ColumnLayout {
              anchors.fill: parent
              anchors.rightMargin: parent.width * 0.6
              anchors.margins: 12

              Item {
                Layout.fillHeight: true
              }

              ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 0

                Label {
                  id: title
                  objectName: "spashscreen_app_title"

                  Layout.fillWidth: true
                  Layout.alignment: Qt.AlignBottom
                  verticalAlignment: Text.AlignBottom
                  horizontalAlignment: Text.AlignHCenter
                  text: control.title
                  font {
                    pointSize: 24
                    letterSpacing: 1.2
                    capitalization: Font.AllUppercase
                  }
                }

                Label {
                  id: subTitle
                  objectName: "spashscreen_app_subTitle"

                  Layout.fillWidth: true
                  Layout.alignment: Qt.AlignTop
                  verticalAlignment: Text.AlignVCenter
                  horizontalAlignment: Text.AlignHCenter
                  font {
                    pointSize: 8
                    letterSpacing: 1.2
                    capitalization: Font.AllUppercase
                  }
                }

                Label {
                  id: version
                  objectName: "spashscreen_app_version"

                  Layout.fillWidth: true
                  Layout.topMargin: 8
                  Layout.alignment: Qt.AlignTop
                  horizontalAlignment: Text.AlignHCenter
                  font {
                    pointSize: 8
                    letterSpacing: 1.2
                  }
                }
              }

              Item {
                Layout.fillHeight: true
              }

              Label {
                id: legalNotes
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignHCenter
                opacity: 0.5
                font.pointSize: 8
                font.letterSpacing: 1.2
                antialiasing: true
                color: Qt.color("white")
                wrapMode: Text.WordWrap
              }
            }
          }
        }

        Rectangle {
          id: footerBackground
          objectName: "splashscreen_footer_container"

          Layout.fillWidth: true
          Layout.preferredHeight: 60

          bottomLeftRadius: shadow.radius
          bottomRightRadius: shadow.radius

          Rectangle {
            objectName: "splashscreen_footer_line"
            width: parent.width
            height: 1
            color: Qt.color("orange")
          }

          RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 24

            ColumnLayout {
              Layout.fillHeight: true
              Layout.fillWidth: true

              Label {
                Layout.fillWidth: true
                Layout.fillHeight: true
                verticalAlignment: Text.AlignVCenter
                text: qsTr("Loading: ") + control.loadingText
              }

              ProgressBar {
                id: progressBar
                objectName: "Splashscreen_progress_bar"
                Layout.fillWidth: true
                from: 0
                to: 100
                background: Item {}

                onValueChanged: if (value >= to) {
                  control.finished();
                }
              }
            }
          }
        }
      }
    } // container
  }
}
