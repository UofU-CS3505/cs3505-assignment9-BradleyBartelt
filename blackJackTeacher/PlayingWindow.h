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
    void nextLine();
public slots:

    void mainMenuClicked();

    void updateCardImage(QImage);

    void addCardToPlayerHand(Card card);

    void addCardToDealerHand(Card card, bool isFirstCard);

    void gameStateUpdateView(bool bust, bool won, bool canSplit);

    void messageRecieved(QString message);
private slots:
    void on_nextButton_clicked();
    void endLevel(bool errorState);

private:
    Ui::PlayingWindow *ui;
    QMainWindow* mainMenu;
    Model* model;
    void SetUpConnections(Model& model);
    QVector<QLabel*> cards;

};

#endif // PLAYINGWINDOW_H
