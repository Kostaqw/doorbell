#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>

class QLabel;
class QTreeView;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QFileSystemModel;

class CustomTitleBar : public QWidget {
    Q_OBJECT

public:
    explicit CustomTitleBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *titleLabel;
};

class CustomButtonPanel : public QWidget {
    Q_OBJECT

public:
    explicit CustomButtonPanel(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *button1;
    QPushButton *button2;
    QHBoxLayout *buttonLayout;
};

class CustomWidget : public QWidget {
    Q_OBJECT

public:
    explicit CustomWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;
    QTreeView *treeView;
    QFileSystemModel *fileSystemModel;
    CustomTitleBar *titleBar;
    CustomButtonPanel *buttonPanel;
};

#endif // CUSTOMWIDGET_H
