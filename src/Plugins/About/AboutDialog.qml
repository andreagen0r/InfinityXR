import QtQuick
import QtQuick.Layouts

import Infinity
import Infinity.Core
import QtQuick.Controls.Basic

Dialog {
  id: aboutDialog
  objectName: "view.about"

  anchors.centerIn: Overlay.overlay

  title: qsTr("About Infinity")
  modal: true

  padding: 24

  function orientationToString(o) {
    switch (o) {
    case Qt.PrimaryOrientation:
      return "primary";
    case Qt.PortraitOrientation:
      return "portrait";
    case Qt.LandscapeOrientation:
      return "landscape";
    case Qt.InvertedPortraitOrientation:
      return "inverted portrait";
    case Qt.InvertedLandscapeOrientation:
      return "inverted landscape";
    }
    return "unknown";
  }

  Grid {
    id: propertyGrid

    anchors.fill: parent

    columns: 2
    spacing: 12

    Label {
      text: `Screen " ${Screen.name} "`
      font.bold: true
    }
    Item {
      width: 1
      height: 1
    } // spacer

    Label {
      text: "manufacturer"
    }
    Label {
      text: Screen?.manufacturer || "unknown"
    }

    Label {
      text: "model"
    }
    Label {
      text: Screen?.model || "unknown"
    }

    Label {
      text: "serial number"
    }
    Label {
      text: Screen?.serialNumber || "unknown"
    }

    Label {
      text: "dimensions"
    }
    Label {
      text: `${Screen.width}  x  ${Screen.height}`
    }

    Label {
      text: "pixel density"
    }
    Label {
      text: `${Screen.pixelDensity.toFixed(2)} dots/mm (${(Screen.pixelDensity * 25.4).toFixed(2)} dots/inch)`
    }

    Label {
      text: "logical pixel density"
    }
    Label {
      text: `${Screen.logicalPixelDensity.toFixed(2)} dots/mm (${(Screen.logicalPixelDensity * 25.4).toFixed(2)} dots/inch)`
    }

    Label {
      text: "device pixel ratio"
    }
    Label {
      text: Screen.devicePixelRatio.toFixed(2)
    }

    Label {
      text: "available virtual desktop"
    }
    Label {
      text: `${Screen.desktopAvailableWidth} x ${Screen.desktopAvailableHeight}`
    }

    Label {
      text: "position in virtual desktop"
    }
    Label {
      text: `${Screen.virtualX} , ${Screen.virtualY}`
    }

    Label {
      text: "orientation"
    }
    Label {
      text: `${aboutDialog.orientationToString(Screen.orientation)} (${Screen.orientation})`
    }

    Label {
      text: "primary orientation"
    }
    Label {
      text: `${aboutDialog.orientationToString(Screen.primaryOrientation)} (${Screen.primaryOrientation})`
    }

    Label {
      text: qsTr("Real size scale (mm):")
    }
    Item {
      id: ruler

      readonly property string currentScreenName: Window.window?.screen?.name ?? ""
      readonly property real screenDensity: ScreenHelper.getDensityByName(currentScreenName)

      property int divisions: 30
      property real step: width / divisions

      width: Math.round(screenDensity * divisions) // Math.round evita sub-pixels borrados
      height: Math.round(screenDensity * 5)

      Repeater {
        model: ruler.divisions + 1   // inclui a última marca

        Item {
          id: rulerLine
          required property int index

          x: Math.round(index * ruler.step)
          width: 1
          height: parent.height

          // linha
          Rectangle {
            anchors.bottom: parent.bottom
            width: 1
            height: rulerLine.index % 10 === 0 ? ruler.height : rulerLine.index % 5 === 0 ? (ruler.height * 0.75) : (ruler.height * 0.5)
            color: rulerLine.index % 10 === 0 ? Qt.color("orange") : rulerLine.index % 5 === 0 ? Qt.color("orange") : Qt.alpha(Qt.color("gray"), 0.5)
          }

          // label somente nas marcas principais
          Label {
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            visible: rulerLine.index % 10 === 0
            text: rulerLine.index
            font.pixelSize: 9
          }
        }
      }
    }
  }
}
