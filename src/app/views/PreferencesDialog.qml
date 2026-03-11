pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts

import QtQuick.Controls.Basic.impl

import QtQuick.Controls.Basic

import Infinity.Core

Dialog {
  id: root
  objectName: "PreferencesDialog"
  title: qsTr("Preferences")
  width: 1000
  height: 700
  modal: true
  anchors.centerIn: Overlay.overlay
  closePolicy: Popup.NoAutoClose

  // Estado reativo global da janela
  property bool hasUnsavedChanges: false

  // Função que varre todos os painéis carregados para ver se algum está sujo
  function updateDirtyState() {
    let dirty = false;
    for (let i = 0; i < rightPanelRepeater.count; ++i) {
      const loader = rightPanelRepeater.itemAt(i);
      // Optional chaining seguro
      if (loader?.item?.isDirty) {
        dirty = true;
        break;
      }
    }
    root.hasUnsavedChanges = dirty;
  }

  SplitView {
    anchors.fill: parent

    Rectangle {
      color: Qt.color("gray")
      SplitView.preferredWidth: 150
    }

    Rectangle {
      color: Qt.color("lightblue")
    }
  }

  // --- RODAPÉ COM OS BOTÕES ---
  footer: DialogButtonBox {
    Button {
      text: qsTr("Save")
      enabled: root.hasUnsavedChanges
      onClicked: {
        // Manda todos os plugins salvarem
        for (let i = 0; i < rightPanelRepeater.count; ++i) {
          const loader = rightPanelRepeater.itemAt(i);
          if (loader?.item?.isDirty) {
            loader.item.applyChanges();
          }
        }
        root.updateDirtyState(); // Deve voltar a false
      }
    }

    Button {
      text: qsTr("Close")
      onClicked: {
        if (root.hasUnsavedChanges) {
          confirmCloseDialog.open();
        } else {
          root.close();
        }
      }
    }
  }

  // --- DIÁLOGO DE AVISO (Unsaved Changes) ---
  Dialog {
    id: confirmCloseDialog
    title: qsTr("Unsaved Changes")
    anchors.centerIn: parent
    modal: true
    standardButtons: Dialog.Save | Dialog.Discard | Dialog.Cancel

    Label {
      text: qsTr("You have unsaved changes. Do you want to save them before closing?")
    }

    onAccepted: {
      // Equivalente a Save
      for (let i = 0; i < rightPanelRepeater.count; ++i) {
        const loader = rightPanelRepeater.itemAt(i);
        if (loader?.item?.isDirty)
          loader.item.applyChanges();
      }
      root.close();
    }

    onDiscarded: {
      // Equivalente a Fechar sem Salvar
      for (let i = 0; i < rightPanelRepeater.count; ++i) {
        const loader = rightPanelRepeater.itemAt(i);
        if (loader?.item?.isDirty)
          loader.item.revertChanges();
      }
      root.close();
    }
  }
}
