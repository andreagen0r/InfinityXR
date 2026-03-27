import QtQuick
import QtQuick.Shapes
import QtQuick.Controls

Control {
    id: control
    implicitWidth: 100
    implicitHeight: 100

    property real value: 0.0
    property bool indeterminate: false
    property real strokeWidth: 8
    property color color: "#21a1f1"
    property color trackColor: "#303030"
    property bool showText: false
    property int duration: 1200

    property real startAngle: -90
    property real sweepAngle: indeterminate ? 270 : value * 360

    SequentialAnimation on startAngle {
        running: control.indeterminate
        loops: Animation.Infinite
        NumberAnimation { to: 270; duration: control.duration; easing.type: Easing.InOutQuad }
        NumberAnimation { to: -90; duration: control.duration; easing.type: Easing.InOutQuad }
    }

    SequentialAnimation on sweepAngle {
        running: control.indeterminate
        loops: Animation.Infinite
        NumberAnimation { to: 30; duration: control.duration/2; easing.type: Easing.InOutQuad }
        NumberAnimation { to: 300; duration: control.duration; easing.type: Easing.InOutQuad }
        NumberAnimation { to: 270; duration: control.duration/2; easing.type: Easing.InOutQuad }
    }

    Shape {
        anchors.fill: parent
        smooth: true
        layer.enabled: true
        layer.samples: 8

        ShapePath {
            strokeColor: control.trackColor
            strokeWidth: control.strokeWidth
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                centerX: control.width / 2
                centerY: control.height / 2
                radiusX: (control.width - control.strokeWidth) / 2
                radiusY: (control.height - control.strokeWidth) / 2
                startAngle: 0
                sweepAngle: 360
            }
        }

        // progresso
        ShapePath {
            strokeColor: control.color
            strokeWidth: control.strokeWidth
            fillColor: "transparent"
            capStyle: ShapePath.RoundCap

            PathAngleArc {
                centerX: control.width / 2
                centerY: control.height / 2
                radiusX: (control.width - control.strokeWidth) / 2
                radiusY: (control.height - control.strokeWidth) / 2
                startAngle: control.startAngle
                sweepAngle: control.sweepAngle
            }
        }
    }

    Label {
        visible: control.showText && !control.indeterminate
        anchors.centerIn: parent
        text: Math.round(control.value * 100) + "%"
        font.pixelSize: width * 0.25
        color: control.color
    }
}
