#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "levelone.h"
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,Model* model = nullptr);
    ~MainWindow();
private slots:

    void on_levelOne_clicked();

private:
    Ui::MainWindow *ui;
    LevelOne* levelOneWindow;
    void setupConnections();
    void setupWindowDisplay();
    Model* model;
};
#endif // MAINWINDOW_H
