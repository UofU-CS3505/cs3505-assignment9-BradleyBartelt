#ifndef PLAYINGWINDOW_H
#define PLAYINGWINDOW_H

#include <QMainWindow>
#include "QtWidgets/qboxlayout.h"
#include "model.h"
#include "qlabel.h"
#include "qpushbutton.h"

namespace Ui {
class PlayingWindow;
}

class PlayingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayingWindow(Model& model,QMainWindow* mainMenu = nullptr, QWidget *parent = nullptr);
    ~PlayingWindow();
signals:
    /// @brief tells the model that the hit button has been pressed and it needs to update the game.
    void hit();
    void nextLine();
public slots:

    void mainMenuClicked();

    void updateCardImage(QImage);

    void addCardToPlayerHand(Card card, bool isSplit);

    void addCardToDealerHand(Card card, bool isFirstCard);

    void canSplit(bool enableSplit);

    void clearOldImages();

    void flipDealerCard(QImage);
    void messageRecieved(QString message);
    void winPopUp(bool);
    void lossPopUp(bool);

    void recievedLock(QString allBut);

    void setReadingScript(bool reading);

    void unlockStand();
    void unlockHit();
    void unlockSplit();

    void receivedProb(double probOfDealerBust, double probOfDealerWin);
private slots:
    void on_nextButton_clicked();
    void endLevel(bool errorState);
    void split();

private:
    Ui::PlayingWindow *ui;
    QMainWindow* mainMenu;
    Model* model;
    QHBoxLayout* splitLayout;
    void SetUpConnections(Model& model);
    QVector<QLabel*> cards;
    bool readingScript = false;

};

#endif // PLAYINGWINDOW_H
