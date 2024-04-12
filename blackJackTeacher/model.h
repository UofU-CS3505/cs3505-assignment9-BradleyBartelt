#ifndef MODEL_H
#define MODEL_H

#include "game.h"
#include <QObject>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void SendCardImage(QImage);
    void addCardToPlayerHand(QImage);
    void addCardToDealerHand(QImage);
public slots:
    void hitSlot();
    void SetLevel(int level);

private:
    Game* game;
    Player* playerOne;
    Player* dealer;
    Deck deck;
    int currentLevel;
    void initalDeal();
};

#endif // MODEL_H
