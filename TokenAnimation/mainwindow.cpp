#include "mainwindow.h"
#include <QApplication>
#include <Box2D/Box2D.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    view(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    circleItem(new QGraphicsEllipseItem(0, 0, 50, 50)),
    timer(new QTimer(this)),
    world(new b2World(b2Vec2(0.0f, -10.0f))) { // Initialize Box2D world with gravity
    view->setScene(scene);
    view->setSceneRect(0, 0, 800, 600);
    circleItem->setPos(400, 0);
    scene->addItem(circleItem);

    // Create the circle body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(400 / 30.0f, 600 / 30.0f);
    circleBody = world->CreateBody(&bodyDef); // Store the body pointer

    b2CircleShape circleShape;
    circleShape.m_radius = 25 / 30.0f;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    circleBody->CreateFixture(&fixtureDef); // Attach fixture to body

    // Connect timer to update function
    connect(timer, &QTimer::timeout, this, &MainWindow::updatePhysics);
    timer->start(1000 / 60); // 60 FPS
}

MainWindow::~MainWindow() {
    // Clean up resources
    delete world;
    delete timer;
    delete circleItem;
    delete scene;
    delete view;
}

void MainWindow::updatePhysics() {
    world->Step(1.0f / 60.0f, 6, 2);

    // Update circle position
    b2Vec2 position = circleBody->GetPosition();
    circleItem->setPos(position.x * 30.0f, 600 - position.y * 30.0f);
}
