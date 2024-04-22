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
    std::vector<b2World*> labelWorlds;
    std::vector<b2Body*> labelBodies;
    int currentX = 0; // Add declaration for currentX
    int currentY = 0; // Add declaration for currentY
    std::array<int, 6> startTimes;
    int currentDelay = 0;
    void moveLabelWithDelay(QLabel* label, int targetX, int targetY, int delay);
};

#endif // MAINWINDOW_H
