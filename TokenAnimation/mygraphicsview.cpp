#include "mygraphicsview.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem> // Include QGraphicsPixmapItem header
#include <QTimer>
#include <Box2D/Box2D.h>

MyGraphicsView::MyGraphicsView() {
    scene = new QGraphicsScene();
    setScene(scene);

    // Load sprite image
    QPixmap spritePixmap(":/images/cardImages/pokerChip.png");

    // Create QGraphicsPixmapItem and add it to the scene
    spriteItem = new QGraphicsPixmapItem(spritePixmap);
    scene->addItem(spriteItem);

    // Position the item at the top of the scene
    spriteItem->setPos(0, 0);

    // Set up Box2D world
    b2Vec2 gravity(0.0f, 9.81f); // Define gravity
    world = new b2World(gravity);

    // Create a dynamic body for the sprite
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0, 0); // Set initial position
    spriteBody = world->CreateBody(&bodyDef);

    // Define a shape for the sprite
    b2PolygonShape shape;
    shape.SetAsBox(spritePixmap.width() / 2.0f, spritePixmap.height() / 2.0f); // Assuming rectangular shape

    // Define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f; // Bounciness

    // Attach fixture to the body
    spriteBody->CreateFixture(&fixtureDef);

    // Start simulation timer
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(advanceSimulation()));
    timer->start(16); // Update every 16 ms (roughly 60 fps)
}

void MyGraphicsView::advanceSimulation() {
    // Step Box2D world simulation
    world->Step(1.0f / 60.0f, 8, 3); // Assuming 60 Hz timestep

    // Update position of QGraphicsPixmapItem based on Box2D body's position
    b2Vec2 position = spriteBody->GetPosition();
    spriteItem->setPos(position.x, position.y);

    // Check for collisions (not implemented in this example)
    // You would typically handle collisions and update sprite's motion here
}
