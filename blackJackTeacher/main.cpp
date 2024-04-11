#include "mainwindow.h"

#include <QApplication>
#include "model.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Model* model = new Model();
    MainWindow w = new MainWindow(nullptr, model);
    w.show();
    return a.exec();
}
