#ifndef MODEL_H
#define MODEL_H

#include "game.h"
#include <QObject>
#include "script.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void SendCardImage(QImage);
    void disableButtons(bool);
    void addCardToPlayerHand(Card);
    void addCardToDealerHand(Card, bool);
    void endLevel(bool errorState);
    void sendMessage(QString message);
    void revealHole();

public slots:
    void hitSlot();
    void SetLevel(int level);
    void standSlot();
    void readyForNextLine();

private:
    Deck deck;
    Player playerOne;
    Player dealer;
    Game game;
    int currentLevel;
    QString scriptOutputDetails;
    Script levelScript;
    void initalDeal();
    void interpretCommand(QString messagetype);

};

#endif // MODEL_H
