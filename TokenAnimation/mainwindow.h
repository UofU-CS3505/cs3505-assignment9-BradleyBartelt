#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <Box2D/Box2D.h>
#include "ui_mainwindow.h"
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateWorld();

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    QGraphicsScene *scene;
    void setupBox2D();
    QGraphicsEllipseItem *circleItem;
    QTimer *timer;
    b2World world;
    b2Body *circleBody; // Add a member for the circle body
     QLabel *label;
};

#endif // MAINWINDOW_H
