#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <Box2D/Box2D.h>

class MyGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    MyGraphicsView();

protected slots:
    void advanceSimulation();

private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* spriteItem;
    b2World* world;
    b2Body* spriteBody;
};

#endif // MYGRAPHICSVIEW_H
