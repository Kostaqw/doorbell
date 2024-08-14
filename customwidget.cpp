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

    mainLayout->addWidget(treeView);

    // Создаем и добавляем панель с кнопками внизу
    buttonPanel = new CustomButtonPanel(this);
    mainLayout->addWidget(buttonPanel);

    mainLayout->setContentsMargins(0, 0, 0, 0);  // Убираем внешние отступы
    mainLayout->setSpacing(0);  // Убираем промежутки между элементами
}
