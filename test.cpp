#include <QDialog>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class Overlay : public QWidget {
public:
    explicit Overlay(QWidget *parent = nullptr)
        : QWidget(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents);  // Игнорировать события мыши
        setAttribute(Qt::WA_NoSystemBackground);
        setStyleSheet("background-color: rgba(0, 0, 0, 150);");  // Полупрозрачное затемнение
    }

protected:
    void resizeEvent(QResizeEvent *event) override {
        // Расширяем overlay на всё окно
        if (parentWidget()) {
            setGeometry(parentWidget()->rect());
        }
    }
};

class CustomDialog : public QDialog {
public:
    explicit CustomDialog(QWidget *parent = nullptr)
        : QDialog(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(new QLabel("Это кастомный диалог", this));
        setLayout(layout);
    }

    int execWithOverlay(QMainWindow *mainWindow) {
        // Создаем overlay
        Overlay *overlay = new Overlay(mainWindow);
        overlay->show();

        // Запуск анимации затемнения
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(overlay);
        overlay->setGraphicsEffect(effect);
        QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
        animation->setDuration(200);  // Длительность анимации
        animation->setStartValue(0);
        animation->setEndValue(1);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        // Показываем диалог
        int result = exec();

        // Убираем overlay после закрытия диалога
        overlay->hide();
        overlay->deleteLater();

        return result;
    }
};

// Пример использования
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Главное окно");
    mainWindow.resize(400, 300);
    mainWindow.show();

    QPushButton *button = new QPushButton("Открыть диалог", &mainWindow);
    mainWindow.setCentralWidget(button);

    CustomDialog dialog;

    QObject::connect(button, &QPushButton::clicked, [&dialog, &mainWindow]() {
        dialog.execWithOverlay(&mainWindow);
    });

    return app.exec();
}
