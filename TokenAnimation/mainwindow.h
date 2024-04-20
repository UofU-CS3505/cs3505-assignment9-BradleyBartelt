#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <Box2D/Box2D.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updatePhysics();

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *circleItem;
    QTimer *timer;
    b2World *world;
};

#endif // MAINWINDOW_H
