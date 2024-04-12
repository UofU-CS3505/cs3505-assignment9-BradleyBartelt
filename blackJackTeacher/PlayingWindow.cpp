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
}

PlayingWindow::~PlayingWindow()
{
    delete ui;
}


void PlayingWindow::addCardToPlayerHand(QImage cardImage){
    QPushButton* newCard = new QPushButton( ui->handArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->handArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    updateCardImage(cardImage);
}


void PlayingWindow::addCardToDealerHand(QImage cardImage){
    QPushButton* newCard = new QPushButton( ui->dealerArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->dealerArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    updateCardImage(cardImage);
}


void PlayingWindow::hitButtonClicked()
{
    QPushButton* newCard = new QPushButton( ui->handArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->handArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    emit hit();
}


void PlayingWindow::updateCardImage(QImage image){
    QPushButton* cardToUpdate = cards.back();
    QPixmap pixmap = QPixmap::fromImage(image);
    cardToUpdate->setIcon(QIcon(pixmap));
    cardToUpdate->setIconSize(QSize(125,175));
    std::cout << "here" << std::endl;
}


void PlayingWindow::mainMenuClicked()
{
    mainMenu->show();
    delete this;
}



//=========================== CONECTIONS =========================

void PlayingWindow::SetUpConnections(Model& model){
    //============ hit connections
    connect(this,&PlayingWindow::hit,&model,&Model::hitSlot);
    connect(ui->hitButton,&QPushButton::clicked,this,&PlayingWindow::hitButtonClicked);
    connect(ui->mainMenu,&QPushButton::clicked,this,&PlayingWindow::mainMenuClicked);
    connect(&model,&Model::SendCardImage,this,&PlayingWindow::updateCardImage);

    //============= initalDeal connections

    connect(&model,&Model::addCardToDealerHand,this,&PlayingWindow::addCardToDealerHand);
    connect(&model,&Model::addCardToPlayerHand,this,&PlayingWindow::addCardToPlayerHand);
}

