    // Создаем основной виджет
    QWidget mainWindow;
    QVBoxLayout *mainLayout = new QVBoxLayout(&mainWindow);

    // Создаем QScrollArea
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true); // Разрешаем изменять размеры виджета

    // Создаем QGroupBox
    QGroupBox *groupBox = new QGroupBox("Dynamic Items");
    groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Создаем FlowLayout
    QWidget *flowWidget = new QWidget(); // Обертка для FlowLayout
    FlowLayout *flowLayout = new FlowLayout(flowWidget);

    flowWidget->setLayout(flowLayout);
    groupBox->setLayout(new QVBoxLayout()); // Чтобы QGroupBox корректно отображался
    groupBox->layout()->addWidget(flowWidget);

    scrollArea->setWidget(groupBox);
    mainLayout->addWidget(scrollArea);
