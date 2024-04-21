#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Box2D/Box2D.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    world(b2Vec2(0.0f, -10.0f)) { // Initialize Box2D world with gravity
    ui->setupUi(this);

    //ui->label->setPixmap(QPixmap("/Users/manyanair/Downloads/pokerchip.png"));


    setupBox2D(); // Setup Box2D world and body

    // Initialize and configure the timer
    connect(timer, &QTimer::timeout, this, &MainWindow::updateWorld);
    timer->start(1000 / 60); // 60Hz
}

MainWindow::~MainWindow() {
    delete ui;
    // Cleanup Box2D objects
    world.DestroyBody(circleBody);
}

void MainWindow::setupBox2D() {
    // Ground body setup
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -15.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Dynamic body setup for the circle
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    circleBody = world.CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f; // Adjusted for Box2D's scale

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9f; // High restitution for bouncing

    circleBody->CreateFixture(&fixtureDef);
}

void MainWindow::updateWorld() {

    // Step the world
    world.Step(1.0f / 60.0f, 6, 2);

    // Get the position of the Box2D object
    b2Vec2 position = circleBody->GetPosition();


    int x = static_cast<int>(position.x * 100); // Scale x-position
    int y = static_cast<int>(-position.y * 100); // Scale and invert y-position



    QPixmap pixmap("/Users/manyanair/Downloads/pokerchip.png");
    QPixmap scaledPixmap1 = pixmap.scaled(50, 50, Qt::KeepAspectRatio);
    ui->label->setPixmap(scaledPixmap1);
    ui->label->setGeometry(100 + x, 100 + y, scaledPixmap1.width(), scaledPixmap1.height());

    QPixmap pixmap2("/Users/manyanair/Downloads/pokerchip.png");
    QPixmap scaledPixmap2 = pixmap2.scaled(50, 50, Qt::KeepAspectRatio);
    ui->label_2->setPixmap(scaledPixmap2);
    ui->label_2->setGeometry(250 + x, 100 + y, scaledPixmap2.width(), scaledPixmap2.height());

    QPixmap pixmap3("/Users/manyanair/Downloads/pokerchip.png");
    QPixmap scaledPixmap3 = pixmap3.scaled(50, 50, Qt::KeepAspectRatio);
    ui->label_3->setPixmap(scaledPixmap1);
    ui->label_3->setGeometry(400 + x, 100 + y, scaledPixmap3.width(), scaledPixmap3.height());



}
