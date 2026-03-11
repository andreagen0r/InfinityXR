import QtQuick

import Infinity.Core

QtObject {

  function buildMenuBar(bar) {
    const root = MenuModel.root();
    const rows = MenuModel.rowCount(root);

    for (let r = 0; r < rows; ++r) {
      const idx = MenuModel.qmlIndex(r, 0, root);

      const menu = createMenu(idx, bar);
      if (menu)
        bar.addMenu(menu);
    }
  }

  function createMenu(idx, parent) {
    const menu = Qt.createQmlObject('import QtQuick.Controls.Basic; Menu {}', parent);

    menu.title = MenuModel.dataAtRole(idx, "text");

    const rows = MenuModel.rowCount(idx);

    for (let r = 0; r < rows; ++r) {
      const childIdx = MenuModel.qmlIndex(r, 0, idx);

      const type = MenuModel.dataAtRole(childIdx, "itemType");

      if (type === MenuModel.Separator) {
        menu.addItem(Qt.createQmlObject('import QtQuick.Controls.Basic; MenuSeparator {}', menu));
      } else if (type === MenuModel.SubMenu)
        menu.addMenu(createMenu(childIdx, menu));
      else
        menu.addItem(createAction(childIdx, menu));
    }

    return menu;
  }

  function createAction(idx, parent) {
    const item = Qt.createQmlObject('import QtQuick.Controls.Basic; MenuItem { action: Action {} }', parent);

    const a = item.action;

    a.text = MenuModel.dataAtRole(idx, "text");
    a.enabled = MenuModel.dataAtRole(idx, "enabled");
    a.shortcut = MenuModel.dataAtRole(idx, "shortcut");
    a.checkable = MenuModel.dataAtRole(idx, "checkable");
    a.checked = MenuModel.dataAtRole(idx, "checked");

    const cmd = MenuModel.dataAtRole(idx, "commandId");
    a.triggered.connect(() => CommandRegistry.dispatch(cmd));

    return item;
  }
}
