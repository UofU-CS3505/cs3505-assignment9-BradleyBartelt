#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <Box2D/Box2D.h>
#include <QTimer>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget *parent = nullptr);

protected slots:
    void advanceSimulation();

private:
    QGraphicsScene* scene;
    QGraphicsPixmapItem* spriteItem;
    b2World* world;
    b2Body* spriteBody;
};

#endif // MYGRAPHICSVIEW_H
