#include "CustomWidget.hpp"
#include <QLabel>
#include <QTreeView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QFileSystemModel>

// Реализация заголовка
CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent) {
    setFixedHeight(50);  // Задаем высоту заголовка
    titleLabel = new QLabel("Custom Title", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->setContentsMargins(0, 0, 0, 0);
}

void CustomTitleBar::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    p.setBrush(QColor(100, 100, 255));  // Цвет заголовка
    p.setPen(Qt::NoPen);
    p.drawRect(rect());
}

// Реализация панели с кнопками
CustomButtonPanel::CustomButtonPanel(QWidget *parent) : QWidget(parent) {
    setFixedHeight(50);  // Высота панели

    button1 = new QPushButton("Button 1", this);
    button2 = new QPushButton("Button 2", this);

    button1->setStyleSheet(
                                "QPushButton {"
                           "    background-color: #2f2f2f;" /* Темно-серый цвет по умолчанию */
                           "    border-left: 2px solid #000000;" /* Левая граница черная */
                           "    border-bottom: 2px solid #000000;" /* Нижняя граница черная */
                           "    border-right: 2px solid #ffffff;" /* Правая граница белая */
                           "    border-top: 2px solid #ffffff;" /* Верхняя граница белая */
                           "    color: #ffffff;" /* Цвет текста */
                           "}"
                           "QPushButton:hover {"
                           "    background-color: #4f4f4f;" /* Более светлый цвет при наведении */
                           "    border-left: 2px solid #888888;" /* Подсвеченная левая граница */
                           "    border-bottom: 2px solid #888888;" /* Подсвеченная нижняя граница */
                           "    border-right: 2px solid #cccccc;" /* Подсвеченная правая граница */
                           "    border-top: 2px solid #cccccc;" /* Подсвеченная верхняя граница */
                           "}"
                           "QPushButton:pressed {"
                           "    background-color: #1f1f1f;" /* Более темный цвет при нажатии */
                           "    border-left: 2px solid #444444;" /* Измененная левая граница */
                           "    border-bottom: 2px solid #444444;" /* Измененная нижняя граница */
                           "    border-right: 2px solid #777777;" /* Измененная правая граница */
                           "    border-top: 2px solid #777777;" /* Измененная верхняя граница */
                           "}"
                           );
    buttonLayout = new QHBoxLayout(this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);
    buttonLayout->setContentsMargins(10, 0, 10, 0);  // Отступы от краев
}

void CustomButtonPanel::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);

    // Получаем палитру для текущей темы
    QPalette palette = this->palette();
    QColor backgroundColor = palette.color(QPalette::Button);  // Цвет кнопки (подойдет для фона панели)

    p.setBrush(backgroundColor);  // Используем цвет из палитры
    p.setPen(Qt::NoPen);
    p.drawRect(rect());
}

// Реализация основного виджета
CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::FramelessWindowHint);  // Отключаем стандартные границы окна
    setAttribute(Qt::WA_TranslucentBackground);  // Для прозрачного фона

    mainLayout = new QVBoxLayout(this);

    // Создаем и добавляем кастомный заголовок
    titleBar = new CustomTitleBar(this);
    mainLayout->addWidget(titleBar);

    // Создаем и настраиваем модель файловой системы
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath(QDir::rootPath());

    // Создаем и добавляем TreeView в центр
    treeView = new QTreeView(this);
    treeView->setModel(fileSystemModel);
    treeView->setRootIndex(fileSystemModel->index(QDir::rootPath()));
    treeView->setStyleSheet(
        "QTreeView {"
        "    background-color: #f0f0f0;" /* Цвет фона дерева */
        "    alternate-background-color: #e0e0e0;" /* Альтернативный цвет фона для четных строк */
        "}"
        "QHeaderView::section {"
        "    background-color: #4f81bd;" /* Цвет фона заголовков столбцов */
        "    color: white;" /* Цвет текста заголовков */
        "    padding: 4px;" /* Отступы для текста */
        "    font-weight: bold;" /* Жирный текст */
        "    border: 1px solid #6d9eeb;" /* Цвет границы заголовков */
        "}"
        "QScrollBar:vertical {"
        "    border: 2px solid #6d9eeb;"
        "    background: #f0f0f0;"
        "    width: 10px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #4f81bd;"
        "    border-radius: 5px;"
        "}"
        );
    treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout->addWidget(treeView);

    // Создаем и добавляем панель с кнопками внизу
    buttonPanel = new CustomButtonPanel(this);
    mainLayout->addWidget(buttonPanel);

    mainLayout->setContentsMargins(0, 0, 0, 0);  // Убираем внешние отступы
    mainLayout->setSpacing(0);  // Убираем промежутки между элементами
}
