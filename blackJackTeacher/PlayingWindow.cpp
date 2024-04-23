#include "PlayingWindow.h"
#include "ui_PlayingWindow.h"
#include "model.h"
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

    QPalette palette = this->palette();
    QColor bg = QColor(61, 59, 59);
    palette.setColor(QPalette::Window, bg);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
}

PlayingWindow::~PlayingWindow()
{
    delete ui;
}

void PlayingWindow::receivedProb(double probOfDealerBust, double probOfDealerWin){
    ui->ProbOfDealerBustLabel->setText("probablity of dealer bust: " + QString::number(probOfDealerBust));
}


void PlayingWindow::addCardToPlayerHand(Card card, bool isSplit){
    QLabel* newCard = new QLabel(ui->handArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");
    if(isSplit){
        splitLayout->addWidget(newCard, Qt::AlignLeft);
        updateCardImage(card.image);
    }
    else{
        QHBoxLayout* layout = (QHBoxLayout*)ui->handLayout->widget()->layout();
        layout->addWidget(newCard, Qt::AlignLeft);
        updateCardImage(card.image);
    }
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
    delete splitLayout;
    cards.clear();
}

void PlayingWindow::canSplit(bool enableSplit){
    if(enableSplit){
        ui->splitButton->setEnabled(true);
    }
    else{
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::messageRecieved(QString message)
{
    //Setlabel to the message recieved
    ui->textDisplay->setText(message);
}

void PlayingWindow::recievedLock(QString allBut)
{
    if(readingScript)
    {
        if(allBut == "stand\n")
        {
            ui->hitButton->setEnabled(false);
            ui->standButton->setEnabled(true);
            ui->nextButton->setEnabled(false);
            ui->splitButton->setEnabled(false);
        }
        if(allBut == "hit\n")
        {
            ui->hitButton->setEnabled(true);
            ui->standButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            ui->splitButton->setEnabled(false);
        }
        if(allBut == "next\n")
        {
            ui->hitButton->setEnabled(false);
            ui->standButton->setEnabled(false);
            ui->nextButton->setEnabled(true);
            ui->splitButton->setEnabled(false);
        }
        if(allBut == "split\n")
        {
            ui->hitButton->setEnabled(false);
            ui->standButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            ui->splitButton->setEnabled(true);
        }
    }
}
void PlayingWindow::unlockStand()
{
    if(readingScript)
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::unlockSplit()
{
    if(readingScript)
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::split(){
    // Create a new scroll area
    QScrollArea* splitArea = new QScrollArea();
    QWidget* splitScroll = new QWidget;

    // Create a layout for the new scroll area
    QHBoxLayout* splitHandLayout = new QHBoxLayout();
    splitArea->setWidget(splitScroll);
    splitHandLayout->addWidget(splitArea);


    // scrollAreaWidget->setLayout(splitHandLayout);
    // splitHand->setWidget(scrollAreaWidget);
    // splitHand->setLayout(splitHandLayout);
    // ui->handLayout->addWidget(splitHand);
    // splitHandLayout->addWidget(splitHand);
    // get the card to move from the the player hand to the split hand
    QLabel* moveCard = cards.at(2);
    QHBoxLayout* moveFrom = qobject_cast<QHBoxLayout*>(ui->handArea->widget()->layout());

    // Remove the QLabel from its current layout
    moveFrom->removeWidget(moveCard);

    // Add the QLabel to the layout of the new scroll area
    splitHandLayout->addWidget(moveCard);

    // Disable the split button
    ui->splitButton->setEnabled(false);
    splitLayout = splitHandLayout;

}
void PlayingWindow::unlockHit()
{
    if(readingScript)
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::setReadingScript(bool reading)
{
    readingScript = reading;
}
//=========================== CONECTIONS =========================

void PlayingWindow::SetUpConnections(Model& model){
    //============ hit connections
    //connect(this,&PlayingWindow::hit,&model,&Model::hitSlot);
    connect(ui->hitButton,&QPushButton::clicked,&model, &Model::hitSlot);
    connect(ui->mainMenu, &QPushButton::clicked, &model, &Model::mainMenuSlot);
    connect(ui->mainMenu, &QPushButton::clicked,this,&PlayingWindow::mainMenuClicked);
    //connect(&model,&Model::SendCardImage,this,&PlayingWindow::updateCardImage);

    // ============ stand connections

    connect(&model,&Model::SendCardImage,this,&PlayingWindow::flipDealerCard);
    connect(ui->standButton, &QPushButton::clicked, &model, &Model::standSlot);
    connect(&model, &Model::disableButtons, ui->hitButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->standButton, &QPushButton::setEnabled);
    // connect(&model, &Model::disableButtons, ui->splitButton, &QPushButton::setEnabled);
    // the split button should not be enabled right off the bat
    connect(&model, &Model::disableButtons, ui->doubleButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->mainMenu, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->dealCards, &QPushButton::setEnabled);
    connect(&model, &Model::winMessage, this, &PlayingWindow::winPopUp);
    connect(&model, &Model::lossMessage, this, &PlayingWindow::lossPopUp);
    connect(&model, &Model::enableMainMenu, ui->mainMenu, &QPushButton::setEnabled);

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
    connect(ui->splitButton,&QPushButton::clicked,this,&PlayingWindow::unlockSplit);
    connect(ui->standButton,&QPushButton::clicked,this,&PlayingWindow::unlockStand);
    connect(ui->hitButton,&QPushButton::clicked,this,&PlayingWindow::unlockHit);
    connect(&model,&Model::revealHole,this,&PlayingWindow::flipDealerCard);
    connect(&model,&Model::sendLock,this,&PlayingWindow::recievedLock);
    connect(&model,&Model::sendClear,this,&PlayingWindow::clearOldImages);
    connect(&model,&Model::sendReadingScript,this,&PlayingWindow::setReadingScript);
    connect(&model,&Model::sendProbabilities,this,&PlayingWindow::receivedProb);

    //============= split connect
    connect(ui->splitButton,&QPushButton::clicked,this,&PlayingWindow::split);
    connect(ui->splitButton,&QPushButton::clicked,&model,&Model::splitSlot);
    connect(&model,&Model::enableSplit,this,&PlayingWindow::canSplit);

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
    if(readingScript)
    {
        recievedLock("next");
        ui->nextButton->setEnabled(false);
    }

}
void PlayingWindow::winPopUp(bool isVisible)
{
    ui->winButton->setVisible(isVisible);
}
void PlayingWindow::lossPopUp(bool isVisible)
{
    ui->loseButton->setVisible(isVisible);
}
