pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import Infinity.Core

Item {
  id: root
  z: 9999

  property int globalPosition: 8
  property int maxWidth: 350
  property int maxHeight: 600

  // ==========================================================================
  // HELPERS LÓGICOS DE POSIÇÃO
  // ==========================================================================
  readonly property bool isTop: (globalPosition === 0 || globalPosition === 1 || globalPosition === 2 || globalPosition === 9)
  readonly property bool isBottom: (globalPosition === 6 || globalPosition === 7 || globalPosition === 8 || globalPosition === 11)
  readonly property bool isMiddle: (globalPosition === 3 || globalPosition === 4 || globalPosition === 5 || globalPosition === 10)

  // ==========================================================================
  // HELPERS DE ANIMAÇÃO
  // ==========================================================================
  readonly property bool animLeft: (globalPosition === 0 || globalPosition === 3 || globalPosition === 6)
  readonly property bool animRight: (globalPosition === 2 || globalPosition === 5 || globalPosition === 8)
  readonly property bool animTop: (globalPosition === 1 || globalPosition === 9)
  readonly property bool animBottom: (globalPosition === 7 || globalPosition === 11)

  // Offset horizontal — igual para entrada e saída
  readonly property real offX: animLeft ? -root.width : (animRight ? root.width : 0)

  readonly property real offYIn: animTop ? -150 : (animBottom ? 150 : 0)

  // baseado em maxHeight, garante que o item da lista saia completamente da área visível
  readonly property real offYOut: animTop ? -(maxHeight + 20) : (animBottom ? (maxHeight + 20) : 0)

  width: (globalPosition >= 9) ? undefined : maxWidth
  height: isMiddle ? Math.min(listView.contentHeight, maxHeight) : maxHeight
  anchors.margins: 20

  anchors.top: isTop ? parent.top : undefined
  anchors.bottom: isBottom ? parent.bottom : undefined
  anchors.verticalCenter: isMiddle ? parent.verticalCenter : undefined

  anchors.left: (globalPosition === 0 || globalPosition === 3 || globalPosition === 6 || globalPosition === 9 || globalPosition === 10 || globalPosition === 11) ? parent.left : undefined
  anchors.right: (globalPosition === 2 || globalPosition === 5 || globalPosition === 8 || globalPosition === 9 || globalPosition === 10 || globalPosition === 11) ? parent.right : undefined
  anchors.horizontalCenter: (globalPosition === 1 || globalPosition === 4 || globalPosition === 7) ? parent.horizontalCenter : undefined

  NotificationFilterModel {
    id: localFilter
    sourceModel: NotificationService.model
    acceptedPosition: root.globalPosition
  }

  ListView {
    id: listView
    anchors.fill: parent
    model: localFilter
    spacing: 10
    clip: false
    interactive: false
    verticalLayoutDirection: isBottom ? ListView.BottomToTop : ListView.TopToBottom

    onCountChanged: {
      if (count > 0)
        positionViewAtBeginning();
    }

    add: Transition {
      NumberAnimation {
        property: "opacity"
        from: 0
        to: 1.0
        duration: 350
      }
      NumberAnimation {
        property: "x"
        from: ViewTransition.destination.x + root.offX
        duration: 350
        easing.type: Easing.OutCubic
      }
      NumberAnimation {
        property: "y"
        from: ViewTransition.destination.y + root.offYIn
        duration: 350
        easing.type: Easing.OutCubic
      }
    }

    remove: Transition {
      NumberAnimation {
        property: "opacity"
        from: 1.0
        to: 0
        duration: 300
      }
      NumberAnimation {
        property: "x"
        to: root.offX
        duration: 300
        easing.type: Easing.InCubic
      }
      NumberAnimation {
        property: "y"
        to: ViewTransition.item.y + root.offYOut
        duration: 300
        easing.type: Easing.InCubic
      }
    }

    displaced: Transition {
      NumberAnimation {
        properties: "x,y"
        duration: 300
        easing.type: Easing.OutCubic
      }
    }

    delegate: ToastItem {
      required property string idRole
      required property int severityRole
      required property string titleRole
      required property string textRole
      required property int timeoutRole
      required property bool actionRequiredRole
      required property var actionsRole

      width: ListView.view.width

      notificationId: idRole
      notificationSeverity: severityRole
      title: titleRole
      text: textRole
      timeout: timeoutRole
      actionRequired: actionRequiredRole
      actionsList: actionsRole

      onTimeoutReached: NotificationService.dismissByTimeout(idRole)
      onUserDismissed: NotificationService.dismissByUser(idRole)
      onActionTriggered: NotificationService.dismissByAction(idRole)
    }
  }
}
