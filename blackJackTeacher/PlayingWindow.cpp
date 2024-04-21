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
    ui->winButton->setVisible(false);
    ui->loseButton->setVisible(false);
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
void PlayingWindow::flipDealerCard(QImage image){
    QLabel* cardToUpdate = cards.at(1);
    QPixmap pixmap = QPixmap::fromImage(image);
    cardToUpdate->setPixmap(pixmap);
    cardToUpdate->setScaledContents(true);
}

void PlayingWindow::mainMenuClicked()
{
    mainMenu->show();
    delete this;
}

// not working yet
void PlayingWindow::clearOldImages(){
    for(int i = 0; i < cards.size(); i++){
        delete cards.at(i);
    }
    cards.clear();
}

void PlayingWindow::gameStateUpdateView(bool bust, bool won, bool canSplit){

}

void PlayingWindow::messageRecieved(QString message)
{
    //Setlabel to the message recieved
    ui->textDisplay->setText(message);
}

void PlayingWindow::recievedLock(QString allBut)
{
    if(allBut == "stand\n")
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(true);
        ui->nextButton->setEnabled(false);
    }
    if(allBut == "hit\n")
    {
        ui->hitButton->setEnabled(true);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(false);
    }
    if(allBut == "next\n")
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
    }
}
void PlayingWindow::unlockStand()
{
    ui->hitButton->setEnabled(false);
    ui->standButton->setEnabled(false);
    ui->nextButton->setEnabled(true);
}
void PlayingWindow::unlockHit()
{
    ui->hitButton->setEnabled(false);
    ui->standButton->setEnabled(false);
    ui->nextButton->setEnabled(true);
}
//=========================== CONECTIONS =========================

void PlayingWindow::SetUpConnections(Model& model){
    //============ hit connections
    //connect(this,&PlayingWindow::hit,&model,&Model::hitSlot);
    connect(ui->hitButton,&QPushButton::clicked,&model, &Model::hitSlot);
    connect(ui->mainMenu,&QPushButton::clicked,this,&PlayingWindow::mainMenuClicked);
    //connect(&model,&Model::SendCardImage,this,&PlayingWindow::updateCardImage);

    // ============ stand connections

    connect(ui->standButton,&QPushButton::clicked,&model,&Model::standSlot);
    connect(&model,&Model::SendCardImage,this,&PlayingWindow::flipDealerCard);
    connect(ui->standButton, &QPushButton::clicked, &model, &Model::standSlot);
    connect(&model, &Model::disableButtons, ui->hitButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->standButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->splitButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->doubleButton, &QPushButton::setEnabled);
    connect(&model, &Model::winMessage, this, &PlayingWindow::winPopUp);
    connect(&model, &Model::lossMessage, this, &PlayingWindow::lossPopUp);

    //============= Reset game connections

    //connect(ui->mainMenu, &QPushButton::clicked, &model, &Model::resetGame);

    //============= initalDeal connections

    connect(&model,&Model::addCardToDealerHand,this,&PlayingWindow::addCardToDealerHand);
    connect(&model,&Model::addCardToPlayerHand,this,&PlayingWindow::addCardToPlayerHand);

    //============= dealCards

    connect(ui->dealCards,&QPushButton::clicked,this,&PlayingWindow::clearOldImages);
    connect(&model, &Model::enableDealCards, ui->dealCards, &QPushButton::setEnabled);
    connect(ui->dealCards, &QPushButton::clicked, &model, &Model::dealCards);

    //============= update Counts

    connect(&model, &Model::updateDealerCount, ui->dealerTotal, &QLabel::setText);
    connect(&model, &Model::updatePlayerCount, ui->playerTotal, &QLabel::setText);


    //============= script Handling connections
    connect(&model,&Model::sendMessage,this,&PlayingWindow::messageRecieved);
    connect(this, &PlayingWindow::nextLine,&model,&Model::readyForNextLine);
    connect(&model,&Model::endLevel,this,&PlayingWindow::endLevel);
    connect(ui->standButton,&QPushButton::clicked,this,&PlayingWindow::unlockStand);
    connect(ui->hitButton,&QPushButton::clicked,this,&PlayingWindow::unlockHit);
    connect(&model,&Model::revealHole,this,&PlayingWindow::flipDealerCard);
    connect(&model,&Model::sendLock,this,&PlayingWindow::recievedLock);



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
    sendLock("next");
    ui->nextButton->setEnabled(false);
}
void PlayingWindow::winPopUp(bool isVisible)
{
    ui->winButton->setVisible(isVisible);
}
void PlayingWindow::lossPopUp(bool isVisible)
{
    ui->loseButton->setVisible(isVisible);
}
