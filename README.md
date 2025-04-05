Да, эту проблему можно решить несколькими способами. Вот наиболее эффективные подходы для динамического изменения размера блока выбора в зависимости от текущего выбранного варианта:
1. Динамическое изменение размера StackedWidget

Модифицируем ChoiceBlock, чтобы он подстраивался под размер текущего виджета:
cpp
Copy

class ChoiceBlock : public BaseBlock {
    // ... предыдущий код ...
    
private:
    void updateStackSize() {
        if (stack && stack->currentWidget()) {
            stack->setFixedSize(stack->currentWidget()->sizeHint());
            stack->parentWidget()->adjustSize();
        }
    }
    
public:
    QWidget* createWidget(QWidget* parent) override {
        QWidget* widget = new QWidget(parent);
        QVBoxLayout* layout = new QVBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
        
        combo = new QComboBox(widget);
        stack = new QStackedWidget(widget);
        stack->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        
        for (const auto& opt : options) {
            combo->addItem(opt.first);
            QWidget* optionWidget = opt.second->createWidget();
            optionWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            stack->addWidget(optionWidget);
        }
        
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                currentIndex = index;
                stack->setCurrentIndex(index);
                updateStackSize();
            });
        
        layout->addWidget(combo);
        layout->addWidget(stack);
        
        // Первоначальная настройка размера
        QTimer::singleShot(0, this, [this]() { updateStackSize(); });
        
        return widget;
    }
    
    // ... остальные методы ...
};

2. Альтернативный подход с переопределением sizeHint
cpp
Copy

class DynamicStackedWidget : public QStackedWidget {
public:
    using QStackedWidget::QStackedWidget;
    
    QSize sizeHint() const override {
        if (currentWidget()) {
            return currentWidget()->sizeHint();
        }
        return QStackedWidget::sizeHint();
    }
    
    QSize minimumSizeHint() const override {
        if (currentWidget()) {
            return currentWidget()->minimumSizeHint();
        }
        return QStackedWidget::minimumSizeHint();
    }
};

// Затем в ChoiceBlock замените:
// stack = new QStackedWidget(widget);
stack = new DynamicStackedWidget(widget);

3. Оптимальное решение с комбинацией подходов

Лучше всего сработает комбинация обоих методов:
cpp
Copy

class ChoiceBlock : public BaseBlock {
    // ... предыдущий код ...
    
    QWidget* createWidget(QWidget* parent) override {
        QWidget* widget = new QWidget(parent);
        QVBoxLayout* layout = new QVBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
        
        combo = new QComboBox(widget);
        stack = new DynamicStackedWidget(widget); // Используем наш подкласс
        
        for (const auto& opt : options) {
            combo->addItem(opt.first);
            QWidget* optionWidget = opt.second->createWidget();
            optionWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
            stack->addWidget(optionWidget);
        }
        
        connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [this](int index) {
                currentIndex = index;
                stack->setCurrentIndex(index);
                widget->adjustSize(); // Обновляем размер родительского виджета
            });
        
        layout->addWidget(combo);
        layout->addWidget(stack);
        
        return widget;
    }
    
    // ... остальные методы ...
};

4. Дополнительные улучшения

Чтобы сделать поведение еще более плавным:

    Установите правильные политики размеров:

cpp
Copy

widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    Обновляйте layout при изменении содержимого:

cpp
Copy

connect(combo, &QComboBox::currentIndexChanged, this, [widget]() {
    widget->updateGeometry();
});

    Для сложных вложенных блоков:
    Если варианты содержат другие динамические блоки, вам может понадобиться также подписаться на их сигналы изменения размера:

cpp
Copy

// В BaseBlock добавьте сигнал:
signals:
    void sizeChanged();

// В методах, которые меняют размер (например, после setValue)
emit sizeChanged();

// В ChoiceBlock:
connect(optionWidget, &BaseBlock::sizeChanged, 
        this, [this]() { updateStackSize(); });

Эти подходы обеспечат:

    Автоматическое подстраивание размера под текущий выбранный вариант

    Отсутствие лишнего пустого пространства

    Корректное отображение при изменении выбора

    Поддержку вложенных блоков разного размера

Выберите тот вариант, который лучше всего подходит для вашей конкретной реализации, либо комбинируйте их для достижения наилучшего результата.
