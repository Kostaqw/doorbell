#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QMap>
#include <QByteArray>
#include <QDebug>

// Функция, которая создает словарь из XML
QMap<QString, QByteArray> createXmlDictionary(const QDomDocument& doc) {
    QMap<QString, QByteArray> xmlDictionary;

    // Рекурсивная функция для прохода по узлам XML
    std::function<void(const QDomNode&, const QString&)> traverseXml = [&](const QDomNode& node, const QString& path) {
        if (node.isElement()) {
            QDomElement element = node.toElement();
            QString currentPath = path.isEmpty() ? element.tagName() : path + "." + element.tagName();

            // Добавляем значение в словарь
            xmlDictionary[currentPath] = element.text().toUtf8();

            // Рекурсивно обрабатываем дочерние элементы
            QDomNodeList children = element.childNodes();
            for (int i = 0; i < children.size(); ++i) {
                traverseXml(children.at(i), currentPath);
            }
        }
    };

    // Начинаем обработку с корня документа
    traverseXml(doc.firstChild(), "");

    return xmlDictionary;
}

int main() {
    QDomDocument originalDoc;
    QDomDocument sizeDoc;

    // Загружаем XML из файлов
    QFile originalFile("original.xml");
    QFile sizeFile("size.xml");

    if (originalFile.open(QIODevice::ReadOnly | QIODevice::Text) && sizeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (originalDoc.setContent(&originalFile) && sizeDoc.setContent(&sizeFile)) {
            originalFile.close();
            sizeFile.close();

            // Создаем словари для обоих XML-документов
            QMap<QString, QByteArray> originalDictionary = createXmlDictionary(originalDoc);
            QMap<QString, QByteArray> sizeDictionary = createXmlDictionary(sizeDoc);

            // Пример использования словарей
            for (auto it = originalDictionary.begin(); it != originalDictionary.end(); ++it) {
                const QString& path = it.key();
                const QByteArray& value = it.value();

                // Если второй словарь содержит размер для данного пути
                if (sizeDictionary.contains(path)) {
                    int size = sizeDictionary[path].toInt();
                    qDebug() << "Path:" << path << ", Value:" << value << ", Size:" << size;
                }
            }
        } else {
            qDebug() << "Failed to set content for one or both XML documents";
        }
    } else {
        qDebug() << "Failed to open one or both XML files";
    }

    return 0;
}
