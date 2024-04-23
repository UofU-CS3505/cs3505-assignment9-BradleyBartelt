#ifndef PLAYINGWINDOW_H
#define PLAYINGWINDOW_H

#include <QMainWindow>
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
    /// @brief informs the model that the player is ready to proceed to the next line
    void nextLine();
public slots:

    void mainMenuClicked();

    void updateCardImage(QImage);

    void addCardToPlayerHand(Card card);

    void addCardToDealerHand(Card card, bool isFirstCard);

    void canSplit(bool enableSplit);

    void clearOldImages();

    void flipDealerCard(QImage);

    ///
    /// \brief messageRecieved slot triggered when a instruction message is recieved from the model. displays to the message box in the top left
    /// \param message the message to display to the user
    ///
    void messageRecieved(QString message);

    void winPopUp(bool);
    void lossPopUp(bool);

    ///
    /// \brief recievedLock slot triggered when a lock instruction is recieved. Locks all buttons but the one provided to the method
    /// \param allBut the button that we do not want to lock
    ///
    void recievedLock(QString allBut);

    ///
    /// \brief setReadingScript setter for the readingScript field
    /// \param reading if we are reading from a script or not
    ///
    void setReadingScript(bool reading);

    ///
    /// \brief unlockStand unlocks the next button and locks the other buttons
    ///
    void unlockStand();

    ///
    /// \brief unlockHit unlocks the next button and locks the other buttons
    ///
    void unlockHit();

    ///
    /// \brief unlockSplit unlocks the next button and locks the other buttons
    ///
    void unlockSplit();

    ///
    /// \brief receivedProb updates the probability displayed to the user
    /// \param probOfDealerBust
    /// \param probOfDealerWin
    ///
    void receivedProb(double probOfDealerBust, double probOfDealerWin);
private slots:
    void on_nextButton_clicked();
    void endLevel(bool errorState);
    void split();

private:
    Ui::PlayingWindow *ui;
    QMainWindow* mainMenu;
    Model* model;
    void SetUpConnections(Model& model);
    QVector<QLabel*> cards;
    bool readingScript = false;

};

#endif // PLAYINGWINDOW_H
