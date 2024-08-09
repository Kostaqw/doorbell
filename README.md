#include <QFile>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QDir>

class RomEmulator {
public:
    RomEmulator(const QString& filePath, quint32 sectors, quint32 sectorSize);
    bool writeArray(quint32 address, const QByteArray& array, quint32 size);
    QByteArray readArray(quint32 address, quint32 size);
    void clearSector(quint8 sectorNumber);

private:
    QString m_filePath;
    quint32 m_sectors;
    quint32 m_sectorSize;
    QFile m_file;

    void initializeFile();
    bool addressInRange(quint32 address, quint32 size) const;
};

RomEmulator::RomEmulator(const QString& filePath, quint32 sectors, quint32 sectorSize)
    : m_filePath(filePath), m_sectors(sectors), m_sectorSize(sectorSize), m_file(filePath) {
    initializeFile();
}

void RomEmulator::initializeFile() {
    if (!QFile::exists(m_filePath)) {
        if (!m_file.open(QIODevice::WriteOnly)) {
            qCritical() << "Cannot create file:" << m_filePath;
            return;
        }
        m_file.resize(m_sectors * m_sectorSize);
        QByteArray emptyData(m_sectors * m_sectorSize, 0xFF);
        m_file.write(emptyData);
        m_file.close();
    }
}

bool RomEmulator::addressInRange(quint32 address, quint32 size) const {
    return address + size <= m_sectors * m_sectorSize;
}

bool RomEmulator::writeArray(quint32 address, const QByteArray& array, quint32 size) {
    if (!addressInRange(address, size)) {
        qCritical() << "Address out of range";
        return false;
    }

    if (!m_file.open(QIODevice::ReadWrite)) {
        qCritical() << "Cannot open file for writing:" << m_filePath;
        return false;
    }

    m_file.seek(address);
    QByteArray currentData = m_file.read(size);

    for (quint32 i = 0; i < size; ++i) {
        if (static_cast<quint8>(currentData[i]) != 0xFF) {
            qCritical() << "Cannot write to non-empty cell at address" << address + i;
            m_file.close();
            return false;
        }
    }

    m_file.seek(address);
    m_file.write(array.left(size));
    m_file.close();
    return true;
}

QByteArray RomEmulator::readArray(quint32 address, quint32 size) {
    if (!addressInRange(address, size)) {
        qCritical() << "Address out of range";
        return QByteArray();
    }

    if (!m_file.open(QIODevice::ReadOnly)) {
        qCritical() << "Cannot open file for reading:" << m_filePath;
        return QByteArray();
    }

    m_file.seek(address);
    QByteArray data = m_file.read(size);
    m_file.close();
    return data;
}

void RomEmulator::clearSector(quint8 sectorNumber) {
    if (sectorNumber >= m_sectors) {
        qCritical() << "Sector number out of range";
        return;
    }

    if (!m_file.open(QIODevice::ReadWrite)) {
        qCritical() << "Cannot open file for clearing sector:" << m_filePath;
        return;
    }

    m_file.seek(sectorNumber * m_sectorSize);
    QByteArray emptySector(m_sectorSize, 0xFF);
    m_file.write(emptySector);
    m_file.close();
}

1. Поправить печать, если много сообщений. (650 и больше)
2. Если много сообщений работает - добавить проверки на непечатные символы - по факту нам нужны только буквы, цифры, знаки препинания и пробелы с переносами строк. Остальное можно фильтровать (погугли).
3. В док для  текст выводится и в поле ПЧ, и в расшифровке - это излишне, невозможно читать и переполняет документ. Можно для в поле ставить прочерк.
4. Надо сделать красивый виджет выбора папки для сохранения файла, выбора файла для загрузки. Это используется, например - кнопка выгрузить на мни, загрузить с мни. Выглядит сейчас просто мега неудобно. Надо чтобы было похоже на упрощенный файловый менеджер ОС (как в текстовых редакторах и т.п., например). Может есть готовое решение в инете?

Qt File Browser based on QML - Stack Overflow
https://stackoverflow.com/questions/6133344/qt-file-browser-based-on-qml
Как пример. Но надо поискать посмотреть, и чтобы наши "стили" применить.
