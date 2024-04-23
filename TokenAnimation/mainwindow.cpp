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
    static float delays[] = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0}; // Delays for each label
    static int numLabels = 6;
    static float totalTime = 2.0; // Total time for the animation

    // Calculate the current time in the animation
    static float elapsedTime = 0.0f;
    elapsedTime += 1.0f / 60.0f; // Increment by 1/60 seconds

    // Apply animation only if within the total animation time
    if (elapsedTime <= totalTime) {
        // Step the world
        world.Step(1.0f / 60.0f, 10, 5); // 60Hz

        // Get the position of the Box2D object
        b2Vec2 position = circleBody->GetPosition();

        // Smoothly interpolate the position for smoother animation
        float ratio = elapsedTime / totalTime; // Adjust the ratio for smoother animation
        currentX = currentX * (1 - ratio) + position.x * 100 * ratio;
        currentY = currentY * (1 - ratio) - position.y * 100 * ratio;

        // Update the positions of the QLabel objects
        QPixmap pixmap("/Users/manyanair/Downloads/pokerchip.png");
        QPixmap scaledPixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio);

        // Calculate the initial Y position for all labels (top of the window)
        float initialY = 0;

        // Update the positions of the QLabel objects smoothly
        for (int i = 0; i < numLabels; ++i) {
            QLabel *label = nullptr;
            switch(i) {
            case 0: label = ui->label; break;
            case 1: label = ui->label_2; break;
            case 2: label = ui->label_3; break;
            case 3: label = ui->label_4; break;
            case 4: label = ui->label_5; break;
            case 5: label = ui->label_6; break;
            default: break;
            }

            if (label) {
                float labelDelay = delays[i] * totalTime;
                if (elapsedTime >= labelDelay) {
                    label->setPixmap(scaledPixmap);
                    float adjustedX = 100 * (i + 1) + currentX;
                    float adjustedY = initialY + (elapsedTime - labelDelay) / totalTime * (this->height() - scaledPixmap.height()); // Drop gradually
                    label->move(adjustedX, adjustedY);
                }
            }
        }
    }
}
