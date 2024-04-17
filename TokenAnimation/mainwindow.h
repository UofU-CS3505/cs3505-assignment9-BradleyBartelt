#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <Box2D/Box2D.h>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void advanceSimulation();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsPixmapItem* spriteItem;
    b2World* world;
    b2Body* spriteBody;
};

#endif // MAINWINDOW_H
