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

    void addCardToPlayerHand(Card);

    void addCardToDealerHand(Card,bool);

public slots:
    void hitSlot();
    void SetLevel(int level);
    void standSlot();

private:
    Deck deck;
    Player playerOne;
    Player dealer;
    Game game;
    int currentLevel;
    void initalDeal();
};

#endif // MODEL_H
