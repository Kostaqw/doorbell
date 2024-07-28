В Qt 5.8, QML не поддерживает прямой доступ к данным модели через get(row) в стиле JavaScript-объектов, как показано в вашем вопросе. Вместо этого, вы должны использовать ListView (или TableView, если работаете с QtQuick.Controls 2) и делегаты для работы с элементами модели.

Для получения данных из выделенной строки в Qt 5.8 вы можете использовать currentIndex из TableView и функции data или modelData в QML, чтобы получить доступ к данным в модели.

Вот обновленный пример, подходящий для Qt 5.8:

qml
Копировать код
import QtQuick 2.15
import QtQuick.Controls 2.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480

    TableView {
        id: tableView
        anchors.fill: parent
        model: myModel
        selectionBehavior: TableView.SelectRows
        selectionMode: TableView.SingleSelection

        TableViewColumn {
            role: "name"
            title: "Name"
            width: 200
        }

        onCurrentRowChanged: {
            if (currentRow >= 0) {
                var index = tableView.selectionModel.currentIndex
                var name = myModel.data(index, MyModel.NameRole)
                var detail = myModel.data(index, MyModel.DetailRole)
                dialog.text = "Name: " + name + "\nDetail: " + detail
            } else {
                dialog.text = "No item selected!"
            }
        }
    }

    Button {
        text: "Show Details"
        onClicked: dialog.open
    }

    MessageDialog {
        id: dialog
        title: "Item Details"
        text: "Details will be shown here"
    }
}
Пояснения:
TableView:

selectionBehavior и selectionMode настроены на выбор строки и одновременное выделение одной строки соответственно.
onCurrentRowChanged — это сигнал, который вызывается при изменении текущей строки. Внутри него можно получить текущий индекс строки и использовать метод data для доступа к данным модели.
Получение данных из модели:

myModel.data(index, MyModel.NameRole): Получает данные из модели, используя индекс и роль. Вы должны убедиться, что роль зарегистрирована в модели (например, NameRole и DetailRole).
MessageDialog:

Этот элемент отображает подробную информацию, извлеченную из модели. Открывается при нажатии кнопки "Show Details".
Регистрация ролей в модели
Убедитесь, что роли зарегистрированы в модели и доступны для использования в QML. Пример регистрации ролей:

cpp
Копировать код
QHash<int, QByteArray> MyModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DetailRole] = "detail";
    return roles;
}
В QML, MyModel.NameRole и MyModel.DetailRole соответствуют NameRole и DetailRole из модели, используемой для доступа к данным.
