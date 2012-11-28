/*
  Copyright (C) 2012 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Kevin Krammer, krake@kdab.com
  Author: Tobias Koenig, tokoe@kdab.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

import QtQuick 1.0
import QtGui 1.0

MainWindow {
  id: mainWindow

  windowTitle: textEdit.modified ? qsTr("Declarative Widget Editor *modified*") : qsTr("Declarative Widget Editor")
  windowIcon: _editor.iconFromFile(":/editor.png")

  size: Qt.size(800, 600)

  MenuBar {
    Menu {
      title: qsTr("File")

      Action {
        id: newAction
        text: qsTr("New")
        icon: _editor.iconFromTheme("document-new");
        onTriggered: _editor.newDocument()
      }

      Action {
        id: openAction
        text: qsTr("Open")
        icon: _editor.iconFromTheme("document-open")
        onTriggered: {
          FileDialog.nameFilters = [ qsTr("Plain text files (*.txt)"), qsTr("All files (*.*)") ]
          var fileName = FileDialog.getOpenFileName()
          if (fileName.length > 0)
            _editor.open(fileName);
        }
      }

      Action {
        id: saveAction
        text: qsTr("Save")
        icon: _editor.iconFromTheme("document-save")
        onTriggered: _editor.save()
      }

      Separator {}

      Action {
        text: qsTr("Print")
        icon: _editor.iconFromTheme("document-print")
      }

      Separator {}

      Action {
        text: qsTr("Close")
        icon: _editor.iconFromTheme("application-exit")
        onTriggered: mainWindow.close()
      }
    }

    Menu {
      title: qsTr("Edit")

      Action {
        id: undoAction
        text: qsTr("Undo")
        icon: _editor.iconFromTheme("edit-undo")
        enabled: _editor.undoAvailable
        onTriggered: textEdit.undo()
      }

      Action {
        id: redoAction
        text: qsTr("Redo")
        icon: _editor.iconFromTheme("edit-redo")
        enabled: _editor.redoAvailable
        onTriggered: textEdit.redo()
      }

      Separator {}

      Action {
        id: cutAction
        text: qsTr("Cut")
        icon: _editor.iconFromTheme("edit-cut")
        onTriggered: textEdit.cut()
      }

      Action {
        id: copyAction
        text: qsTr("Copy")
        icon: _editor.iconFromTheme("edit-copy")
        onTriggered: textEdit.copy()
      }

      Action {
        id: pasteAction
        text: qsTr("Paste")
        icon: _editor.iconFromTheme("edit-paste")
        onTriggered: textEdit.paste()
      }

      Separator {}

      Action {
        text: qsTr("Select All")
        icon: _editor.iconFromTheme("edit-select-all")
        onTriggered: textEdit.selectAll()
      }
    }

    Menu {
      title: qsTr("View")

      Action {
        text: qsTr("Enlarge Font")
        icon: _editor.iconFromTheme("zoom-in")
        onTriggered: textEdit.zoomIn()
      }

      Action {
        text: qsTr("Shrink Font")
        icon: _editor.iconFromTheme("zoom-out")
        onTriggered: textEdit.zoomOut()
      }
    }

    Menu {
      title: qsTr("Help")

      Action {
        text: qsTr("About")
        icon: mainWindow.windowIcon
        onTriggered: MessageBox.about(qsTr("About Declarative Widgets Editor Example"),
                                      qsTr("This is an example of a simple text editor written in QML using DeclarativeWidgets"))
      }

      Action {
        text: qsTr("About Qt")
        onTriggered: MessageBox.aboutQt(qsTr("About Qt"))
      }
    }
  }

  ToolBar {
    ActionItem {
      action: newAction
    }

    ActionItem {
      action: openAction
    }

    ActionItem {
      action: saveAction
    }
  }

  ToolBar {
    ActionItem {
      action: undoAction
    }

    ActionItem {
      action: redoAction
    }
  }

  TextEdit {
    id: textEdit

    contextMenuPolicy: Qt.ActionsContextMenu

    ActionItem {
      action: cutAction
    }

    ActionItem {
      action: copyAction
    }

    ActionItem {
      action: pasteAction
    }
  }

  StatusBar {
    id: statusBar
    Label {
      text: "File: " + ( _editor.fileName.length == 0 ? qsTr("unnamed") : _editor.fileName )
    }
  }

  Component.onCompleted: {
    textEdit.document = _editor.document
    _editor.requestSaveFileName.connect(askForSaveFileName)
    _editor.information.connect(informationMessage)
    _editor.critical.connect(criticalMessage)
  }

  function askForSaveFileName() {
    var fileName = FileDialog.getSaveFileName()
    if (fileName.length > 0) {
      _editor.fileName = fileName
      _editor.save()
    }
  }

  function informationMessage(message) {
    console.log("information:" + message)
    statusBar.showMessage(message, 3000)
  }

  function criticalMessage(message) {
    MessageBox.critical(qsTr("Error"), message)
  }
}
