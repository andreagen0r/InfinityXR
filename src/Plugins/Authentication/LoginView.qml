import QtQuick

import QtQuick.Controls.Basic
import Infinity.Core

ApplicationWindow {
  id: loginWindow

  width: 800
  height: 600
  title: qsTr("Login")

  // Remove a barra de título do SO se quiser um visual customizado
  flags: Qt.Window | Qt.FramelessWindowHint
  modality: Qt.ApplicationModal // Bloqueia o resto do app enquanto não logar

  // Sinal que o Main.qml espera para saber que o login deu certo
  signal loginSuccess

  Column {
    anchors.centerIn: parent
    spacing: 20

    Label {
      text: "Infinity Login"
      font.pixelSize: 20
      font.bold: true
      anchors.horizontalCenter: parent.horizontalCenter
    }

    TextField {
      id: txtUser
      focus: true
      placeholderText: "Username (admin)"
      width: 200
      text: "eve.holt@reqres.in"
    }

    TextField {
      id: txtPass
      placeholderText: "Password (admin)"
      echoMode: TextInput.Password
      width: 200
      text: "cityslicka"
    }

    Button {
      text: "Entrar"
      width: 200
      anchors.horizontalCenter: parent.horizontalCenter

      onClicked: {
        // COMMAND TRIGGER
        // The host receives this, creates the LoginCommand(C++) with the data, and executes it.
        ActionDispatcher.dispatch("auth.login", {
          "username": txtUser.text,
          "password": txtPass.text
        });
      }
    }
  }
}
