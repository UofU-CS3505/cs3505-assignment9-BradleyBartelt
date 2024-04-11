#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "levelone.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:

    void on_levelOne_clicked();

private:
    Ui::MainWindow *ui;
    LevelOne* levelOneWindow;
};
#endif // MAINWINDOW_H
