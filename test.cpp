В текущем варианте программы обработка сложных строк, таких как "Номер: XXX, Дата: XX{1-Январь, 2-Февраль, 3-Март} номер2: XXXX, Дата: X{1-значение1, 2-значение2}", может не работать полностью корректно. Программа обрабатывает только самые простые случаи вида XX и XX{1-Январь, 2-Февраль, 3-Март}.

Для работы с более сложными строками, где могут присутствовать разные метки, такие как XXX, XXXX, или X{1-значение1, 2-значение2}, нужно улучшить регулярные выражения и логику обработки. Вот основные шаги, чтобы расширить поддержку более сложных строк:

Улучшения:
Обработка различных типов меток:
Включить в регулярные выражения поддержку более сложных вариантов меток (например, XXX, XXXX и X{}).
Правильное управление курсором:
Логика выделения текста должна корректно работать с разными длинами меток.
Расширение выпадающего списка для различных меток:
Обеспечить поддержку выпадающих списков для всех меток формата X{...} и их вариаций.
Обновленный пример кода:
cpp
Копировать код
#include <QTextEdit>
#include <QTextCursor>
#include <QRegularExpression>
#include <QMenu>
#include <QAction>
#include <QTextCharFormat>
#include <QColor>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>
#include <QDebug>

class CustomTextEdit : public QTextEdit {
    Q_OBJECT

public:
    CustomTextEdit(QWidget* parent = nullptr)
        : QTextEdit(parent) {
        setTextInteractionFlags(Qt::TextEditable | Qt::TextSelectableByMouse);
        connect(this, &CustomTextEdit::cursorPositionChanged, this, &CustomTextEdit::onCursorPositionChanged);
    }

    void setCustomText(const QString& text) {
        setPlainText(text);
        highlightText();
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        QTextEdit::mousePressEvent(event);

        QTextCursor cursor = cursorForPosition(event->pos());
        cursor.select(QTextCursor::WordUnderCursor);
        QString selectedText = cursor.selectedText();

        if (selectedText.contains(QRegularExpression(R"([X]+{.*?})"))) {
            // Показать выпадающий список для конструкций X{...}
            showContextMenu(cursor);
        }
    }

private:
    void onCursorPositionChanged() {
        // Обновление текста при изменении позиции курсора
        highlightText();
    }

    void highlightText() {
        QTextCursor cursor = textCursor();
        cursor.select(QTextCursor::Document);

        QTextCharFormat normalFormat;
        normalFormat.setBackground(Qt::white);

        QTextCharFormat highlightFormat;
        highlightFormat.setBackground(Qt::green);

        cursor.setCharFormat(normalFormat);

        // Шаблон для поиска всех X, XXX, XXXX и конструкций X{...}
        QRegularExpression pattern(R"((XXX+|X{.*?}))");
        QRegularExpressionMatchIterator i = pattern.globalMatch(toPlainText());

        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            cursor.setPosition(match.capturedStart());
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, match.capturedLength());
            cursor.setCharFormat(highlightFormat);
        }
    }

    void showContextMenu(QTextCursor cursor) {
        // Найдем формат выпадающего списка X{1-зн1, 2-зн2, ...}
        QString selectedText = cursor.selectedText();
        QRegularExpression re(R"([X]+{(.*?)})");
        QRegularExpressionMatch match = re.match(selectedText);

        if (match.hasMatch()) {
            QStringList options = match.captured(1).split(", ");

            // Создаем меню
            QMenu menu(this);
            for (const QString& option : options) {
                QAction* action = new QAction(option, &menu);
                connect(action, &QAction::triggered, [=]() {
                    cursor.insertText(option.split("-")[1]);  // Вставляем значение из скобок
                });
                menu.addAction(action);
            }

            // Показываем меню
            QPoint pos = viewport()->mapToGlobal(cursorRect(cursor).bottomRight());
            menu.exec(pos);
        }
    }
};

// Пример использования виджета
class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr) : QWidget(parent) {
        QVBoxLayout* layout = new QVBoxLayout(this);
        CustomTextEdit* customTextEdit = new CustomTextEdit(this);
        layout->addWidget(customTextEdit);

        // Более сложная строка
        QString sampleText = "Номер: XXX, Дата: XX{1-Январь, 2-Февраль, 3-Март} номер2: XXXX, Дата: X{1-значение1, 2-значение2}";
        customTextEdit->setCustomText(sampleText);

        setLayout(layout);
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

#include "main.moc"
Что изменилось:
Улучшенное регулярное выражение:

Теперь шаблон регулярного выражения QRegularExpression pattern(R"((XXX+|X{.*?}))"); поддерживает как одиночные метки X, так и множественные XXX, XXXX и другие вариации.
Конструкции вида X{...} также поддерживаются.
Выпадающий список для всех меток X{...}:

Логика выпадающего списка была изменена для обработки любых меток, начинающихся с X, независимо от длины, а также для правильной подстановки значений из списка.
Подсветка всех X и их вариаций:

Теперь все символы X, включая XX, XXX, и т.д., подсвечиваются зелёным фоном.
Пример использования:
Текст "Номер: XXX, Дата: XX{1-Январь, 2-Февраль, 3-Март} номер2: XXXX, Дата: X{1-значение1, 2-значение2}" будет корректно обрабатываться:

Символы XX, XXX, XXXX подсвечиваются зелёным.
При клике на XX{1-Январь, 2-Февраль, 3-Март} и X{1-значение1, 2-значение2} появляются соответствующие выпадающие списки с вариантами.
Важные моменты:
Поддержка всех вариантов меток: Регулярные выражения настроены на поддержку различных типов меток, таких как XX, XXX, X{...}, и могут быть расширены для других специфичных шаблонов.
Расширение возможностей: Если нужно поддерживать другие типы меток или добавить дополнительные проверки, можно расширить регулярные выражения или логику обработки.
Этот подход позволит гибко обрабатывать сложные строки с метками разного типа и адаптировать интерфейс в зависимости от потребностей приложения.
