import QtQuick
import QtQuick.Controls.Basic
import QtQml.Models

import Infinity

Instantiator {
  id: loader

  property var rootIndex
  property var targetContainer

  model: DelegateModel {
    id: delegateModel
    model: MenuModel
    rootIndex: loader.rootIndex

    delegate: DelegateChooser {
      role: "commandId"

      DelegateChoice {
        roleValue: ""

        delegate: Menu {
          id: currentMenu
          title: model.text
          icon.name: model.iconName
          enabled: model.enabled

          Loader {
            id: childLoader
            active: false
            source: "MenuItemLoader.qml"

            onLoaded: {
              item.targetContainer = currentMenu;
              item.rootIndex = Qt.binding(function () {
                return childLoader.nextIndex;
              });
            }

            property var nextIndex

            Component.onCompleted: {
              nextIndex = MenuModel.index(index, 0, loader.rootIndex);

              active = true;
            }
          }
        }
      }

      DelegateChoice {
        delegate: MenuItem {
          action: Action {
            text: model.text
            icon.name: model.iconName
            shortcut: model.shortcut
            checkable: model.checkable
            checked: model.checked
            enabled: model.enabled
            onTriggered: CommandRegistry.dispatch(model.commandId)
          }
        }
      }
    }
  }

  onObjectAdded: (index, object) => {
    if (!targetContainer)
      return;
    if (typeof targetContainer.insertMenu === 'function') {
      targetContainer.insertMenu(index, object);
    } else if (typeof targetContainer.insertItem === 'function') {
      targetContainer.insertItem(index, object);
    }
  }

  onObjectRemoved: (index, object) => {
    if (!targetContainer)
      return;
    if (typeof targetContainer.removeMenu === 'function') {
      targetContainer.removeMenu(object);
    } else if (typeof targetContainer.removeItem === 'function') {
      targetContainer.removeItem(object);
    }
  }
}
