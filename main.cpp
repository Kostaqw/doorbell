#include <QApplication>
#include "CustomWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    CustomWidget w;
    w.resize(400, 300);
    w.show();

    return a.exec();
}
