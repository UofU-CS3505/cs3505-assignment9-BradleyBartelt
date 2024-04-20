#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr)
        : QMainWindow(parent),
        view(new QGraphicsView(this)),
        scene(new QGraphicsScene(this)),
        circleItem(new QGraphicsEllipseItem(0, 0, 50, 50)),
        timer(new QTimer(this)) {
        view->setScene(scene);
        view->setSceneRect(0, 0, 800, 600);
        circleItem->setPos(400, 0);
        scene->addItem(circleItem);

        // Initialize Box2D world
        b2Vec2 gravity(0.0f, -10.0f);
        world = new b2World(gravity);

        // Create the circle body
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(400 / 30.0f, 600 / 30.0f);
        b2Body *body = world->CreateBody(&bodyDef);

        b2CircleShape circleShape;
        circleShape.m_radius = 25 / 30.0f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circleShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;

        body->CreateFixture(&fixtureDef);

        // Connect timer to update function
        connect(timer, &QTimer::timeout, this, &MainWindow::updatePhysics);
        timer->start(1000 / 60); // 60 FPS
    }

    ~MainWindow() {
        // Clean up resources
        delete world;
        delete timer;
        delete circleItem;
        delete scene;
        delete view;
    }

private slots:
    void updatePhysics() {
        world->Step(1.0f / 60.0f, 6, 2);

        // Update circle position
        b2Vec2 position = world->GetBodyList()->GetPosition();
        circleItem->setPos(position.x * 30.0f, 600 - position.y * 30.0f);
    }

private:
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *circleItem;
    QTimer *timer;
    b2World *world;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
