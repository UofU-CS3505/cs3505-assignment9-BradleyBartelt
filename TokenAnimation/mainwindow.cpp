#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Box2D/Box2D.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    world(b2Vec2(0.0f, -50.0f)), // Initialize Box2D world with gravity
    currentX(0), // Initialize currentX
    currentY(0) { // Initialize currentY
    ui->setupUi(this);

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
    fixtureDef.restitution = 0.6f; // High restitution for bouncing

    circleBody->CreateFixture(&fixtureDef);
}

void MainWindow::updateWorld() {

    // Step the world
    world.Step(1.0f / 60.0f, 10, 5); // 60Hz

    // Get the position of the Box2D object
    b2Vec2 position = circleBody->GetPosition();

    int commonEndX = 100; // Adjust as needed
    int commonEndY = 100;

    // Smoothly interpolate the position for smoother animation
    float ratio = 0.2f; // Adjust the ratio for smoother animation
    currentX = currentX * (1 - ratio) + position.x * 100 * ratio;
    currentY = currentY * (1 - ratio) - position.y * 100 * ratio;

    // Update the positions of the QLabel objects
    QPixmap pixmap("/Users/manyanair/Downloads/pokerchip.png");
    QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);

    // Update the positions of the QLabel objects smoothly
    ui->label->setPixmap(scaledPixmap);
    ui->label->move(100 + currentX, 100 + currentY);

    ui->label_2->setPixmap(scaledPixmap);
    ui->label_2->move(200 + currentX, 100 + currentY);

    ui->label_3->setPixmap(scaledPixmap);
    ui->label_3->move(300 + currentX, 100 + currentY);

    ui->label_4->setPixmap(scaledPixmap);
    ui->label_4->move(400 + currentX, 100 + currentY);

    ui->label_5->setPixmap(scaledPixmap);
    ui->label_5->move(500 + currentX, 100 + currentY);

    ui->label_6->setPixmap(scaledPixmap);
    ui->label_6->move(600 + currentX, 100 + currentY);
}
