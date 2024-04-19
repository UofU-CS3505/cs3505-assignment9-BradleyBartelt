#include "PlayingWindow.h"
#include "ui_PlayingWindow.h"
#include "model.h"
#include <iostream>
#include <ostream>
PlayingWindow::PlayingWindow(Model &model,QMainWindow* menu, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayingWindow)
{
    ui->setupUi(this);
    mainMenu = menu;
    SetUpConnections(model);
    ui->splitButton->setEnabled(false);
    ui->dealCards->setEnabled(false);
    ui->doubleButton->setEnabled(false);

}

PlayingWindow::~PlayingWindow()
{
    delete ui;
}


void PlayingWindow::addCardToPlayerHand(Card card){
    QLabel* newCard = new QLabel( ui->handArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->handArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    updateCardImage(card.image);
    QLabel* playerTotal = ui->playerTotal;
    int currentTotal = playerTotal->text().toInt(nullptr,10);
    playerTotal->setText(QString(QString::number(currentTotal + card.rank))); // calculates the new card plus all the other cards
}


void PlayingWindow::addCardToDealerHand(Card card, bool isFirstCard){
    QLabel* newCard = new QLabel( ui->dealerArea);
    newCard->setFixedSize(125, 175);
    cards.push_back(newCard);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->dealerArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    if(isFirstCard){
        QImage cardBack;
        cardBack.load(":/images/cardImages/cardBack.png");
        updateCardImage(cardBack);
    }
    else{updateCardImage(card.image);}
}


void PlayingWindow::updateCardImage(QImage image){
    QLabel* cardToUpdate = cards.back();
    QPixmap pixmap = QPixmap::fromImage(image);
    cardToUpdate->setPixmap(pixmap);
    cardToUpdate->setScaledContents(true);
}


void PlayingWindow::mainMenuClicked()
{
    mainMenu->show();
    delete this;
}


void PlayingWindow::gameStateUpdateView(bool bust, bool won, bool canSplit){

}

void PlayingWindow::messageRecieved(QString message)
{
    //Setlabel to the message recieved
    ui->textDisplay->setText(message);
    //Show next button
}
//=========================== CONECTIONS =========================

void PlayingWindow::SetUpConnections(Model& model){
    //============ hit connections
    //connect(this,&PlayingWindow::hit,&model,&Model::hitSlot);
    connect(ui->hitButton,&QPushButton::clicked,&model, &Model::hitSlot);
    connect(ui->mainMenu,&QPushButton::clicked,this,&PlayingWindow::mainMenuClicked);
    connect(&model,&Model::SendCardImage,this,&PlayingWindow::updateCardImage);

    // ============ stand connections

    connect(ui->standButton,&QPushButton::clicked,&model,&Model::standSlot);
    connect(&model,&Model::SendCardImage,this,&PlayingWindow::updateCardImage);
    connect(ui->standButton, &QPushButton::clicked, &model, &Model::standSlot);
    connect(&model, &Model::disableButtons, ui->hitButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->standButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->splitButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->doubleButton, &QPushButton::setEnabled);
    //============= initalDeal connections

    connect(&model,&Model::addCardToDealerHand,this,&PlayingWindow::addCardToDealerHand);
    connect(&model,&Model::addCardToPlayerHand,this,&PlayingWindow::addCardToPlayerHand);

    //============= script Handling connections
    connect(&model,&Model::sendMessage,this,&PlayingWindow::messageRecieved);
    connect(this, &PlayingWindow::nextLine,&model,&Model::readyForNextLine);
    connect(&model,&Model::endLevel,this,&PlayingWindow::endLevel);



}


void PlayingWindow::on_nextButton_clicked()
{
    emit nextLine();
}
void PlayingWindow::endLevel(bool errorState)
{
    if(!errorState)
    {
        //Play animation
        //Change text box to display a "hooray" message?
    }
    //Disable next
    ui->nextButton->setEnabled(false);
}

