import QtQuick
import QtQuick.Controls.Basic

import Infinity.Core

Item {
  id: root

  Loader {
    id: dialogLoader
    anchors.fill: parent

    Connections {
      target: DialogService

      function onOpenDialogRequested(dialogId) {
        console.debug("DialogManager: Request to show [", dialogId, " ]");

        const url = ViewRegistry.getUrl(dialogId);

        console.debug("DialogManager: Request to URL [", url, " ]");

        if (url.toString() !== "") {
          dialogLoader.source = url;

          if (dialogLoader.item && typeof dialogLoader.item.open === 'function') {
            dialogLoader.item.open();
          }
        } else {
          console.warn("DialogManager: No URL registered for", dialogId);
        }
      }
    }
  }
}
