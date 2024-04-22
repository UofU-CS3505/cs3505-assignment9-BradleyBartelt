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
    void sendLock(QString allBut);
    void SendCardImage(QImage);
    void disableButtons(bool);
    void addCardToPlayerHand(Card);
    void addCardToDealerHand(Card, bool);
    void enableDealCards(bool);
    void lossMessage(bool);
    void winMessage(bool);
    void updatePlayerCount(QString);
    void updateDealerCount(QString);
    ///
    /// \brief endLevel send a signal to the view to display game end information
    /// \param errorState whether the game was terminated due to an error or naturally
    ///
    void endLevel(bool errorState);
    ///
    /// \brief sendMessage sends a message to be displayed to the user
    /// \param message a QString containing text to display to the user
    ///
    void sendMessage(QString message);
    ///
    /// \brief revealHole signals the view to display the hole card to the user
    ///
    void revealHole(QImage cardFront);

    void sendClear();

public slots:
    void hitSlot();
    void dealCards();
    void SetLevel(int level);
    void standSlot();
    ///
    /// \brief readyForNextLine parse the next line and prepare to send it to the view
    ///
    void readyForNextLine();

private:
    Deck deck;
    Player playerOne;
    Player dealer;
    Game game;
    bool isRigged;
    int currentLevel;
    void endGame();
    std::vector<Card> riggedCards;
    int nextCard = 0;
    ///
    /// \brief scriptOutputDetails a string used as an additional output by the script object
    ///
    QString scriptOutputDetails;
    ///
    /// \brief levelScript a script file that is read in by a QFile object. represents the current level's script
    ///
    Script levelScript;
    void initialDeal();
    ///
    /// \brief interpretCommand parses the output of the line provided by levelScript's nextLine and handles it in the context of the game
    /// \param messagetype the string returned by levelScript. Represents a command type
    ///
    void interpretCommand(QString messagetype);
    Rank stringToRank(QString text);
};

#endif // MODEL_H
