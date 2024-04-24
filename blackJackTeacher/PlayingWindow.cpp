#include "PlayingWindow.h"
#include "ui_PlayingWindow.h"
#include "model.h"
#include <QPixmap>
#include <QTimer>
#include <QLabel>
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

    // Connect timer signal to updateWorld slot
    //connect(timer, &QTimer::timeout, this, &PlayingWindow::updateWorld);
    //timer->start(1000 / 60); // 60Hz


    QPalette palette = this->palette();
    QColor bg = QColor(61, 59, 59);
    palette.setColor(QPalette::Window, bg);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

PlayingWindow::~PlayingWindow()
{
    delete ui;

    //delete timer;
    for (auto& world : worlds) {
        delete world;
    }
}

void PlayingWindow::receivedProb(double probOfDealerBust, double probOfDealerWin){
    ui->ProbOfDealerBustLabel->setStyleSheet("{color: #C0BBFE}");
    ui->ProbOfDealerExceedLabel->setStyleSheet("{color: #C0BBFE}");
    ui->ProbOfDealerBustLabel->setText("Probablity of Dealer Bust: " + QString::number(probOfDealerBust));
    ui->ProbOfDealerExceedLabel->setText("Probability of Dealer Exceeding Current Hand: "+QString::number(probOfDealerWin*100));
}


void PlayingWindow::addCardToPlayerHand(Card card, bool isSplit){
    QLabel* newCard = new QLabel( ui->scrollArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");
    if(isSplit){
        splitLayout->addWidget(newCard,Qt::AlignLeft);
        updateCardImage(card.image);
    }
    else{
        QHBoxLayout* layout = (QHBoxLayout*)ui->scrollArea->widget()->layout();
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

void PlayingWindow::clearOldImages(){
    ui->splitLabel->setEnabled(false);
    ui->splitTotal->setEnabled(false);
    ui->handLayout->setStretch(0,1);
    for(int i = 0; i < cards.size(); i++){
        delete cards.at(i);
    }
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

    // Create a layout for the new scroll area
    splitLayout = (QHBoxLayout*)ui->splitArea->widget()->layout();
    ui->handLayout->setStretch(0,0);


    // get the card to move from the the player hand to the split hand
    QLabel* moveCard = cards.at(2);
    QHBoxLayout* moveFrom = qobject_cast<QHBoxLayout*>(ui->scrollArea->widget()->layout());

    // Remove the QLabel from its current layout
    moveFrom->removeWidget(moveCard);

    // Add the QLabel to the layout of the new scroll area
    splitLayout->addWidget(moveCard);

    // Disable the split button
    ui->splitButton->setEnabled(false);

    ui->splitLabel->setEnabled(true);
    ui->splitTotal->setEnabled(true);


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
    if(!readingScript)
        ui->nextButton->setEnabled(false);
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
    connect(&model, &Model::disableButtons, ui->doubleButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->mainMenu, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->dealCards, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->splitButton, &QPushButton::setEnabled);
    connect(&model, &Model::blackJackButtons, this, &PlayingWindow::blackJack);
    connect(&model, &Model::displayEndGameMessage, this, &PlayingWindow::endGamePopup);
    connect(&model, &Model::changeEndGameMessage, this, &PlayingWindow::changePopupText);
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
     connect(&model, &Model::updateSplitPlayerCount, ui->splitTotal, &QLabel::setText);


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
        setupBox2D();
        //Play animation
        if (!timer->isActive()) {
            timer->start(1000 / 60); // Start the timer with the desired update interval
        }
        //Change text box to display a "hooray" message?
    }
    //Disable next
    if(readingScript)
    {
        recievedLock("next");
        ui->nextButton->setEnabled(false);
    }

}
void PlayingWindow::endGamePopup(bool isVisible)
{
    ui->winButton->setVisible(isVisible);
}
void PlayingWindow::changePopupText(QString text){
    ui->winButton->setText(text);
}
void PlayingWindow::blackJack(bool setState){
    ui->splitButton->setEnabled(setState);
    ui->hitButton->setEnabled(setState);
    ui->standButton->setEnabled(setState);
    ui->doubleButton->setEnabled(setState);
    ui->mainMenu->setEnabled(!setState);
    ui->dealCards->setEnabled(!setState);
}
void PlayingWindow::setupBox2D() {
    // Create Box2D world for each label with different x positions
    float initialX = -10.0f; // Initial x position
    float spacing = 5.0f; // Spacing between bodies
    for (int i = 0; i < 9; ++i) {
        b2Vec2 gravity(0.0f, -50.0f);
        worlds.push_back(new b2World(gravity));
        setupBox2D(initialX + spacing * i, i);
    }
}

void PlayingWindow::setupBox2D(float x, int index) {
    // Ground body setup
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = worlds[index]->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Dynamic body setup for the circle
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, 10.0f); // Adjusted initial Y position to prevent glitch
    b2Body* circleBody = worlds[index]->CreateBody(&bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = 0.5f; // Adjusted for Box2D's scale

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9f; // High restitution for bouncing

    circleBody->CreateFixture(&fixtureDef);

    // Apply initial upward impulse to make it fall
    circleBody->ApplyLinearImpulse(b2Vec2(0.0f, -10.0f), circleBody->GetWorldCenter(), true);

    // Increment index to create the next body, if not reached the limit
    if (++index < 9) {
        float delay = 0.2f; // Delay in seconds
        QTimer::singleShot(static_cast<int>(delay * 1000), [=]() {
            setupBox2D(x + 5.0f, index); // Call setupBox2D with incremented x and index
        });
    }
}

void PlayingWindow::updateWorld() {
    static float totalTime = 2.0;
    static float elapsedTime = 0.0f;
    elapsedTime += 1.0f / 60.0f; // Increment by 1/60 seconds

    // Apply animation only if within the total animation time
    if (elapsedTime <= totalTime) {
        // Step the Box2D worlds
        for (auto world : worlds) {
            world->Step(1.0f / 60.0f, 10, 5); // Step the world
        }

        QImage *pChip = new QImage(":/images/cardImages/pokerChip.png");
        // convert Qimage to Qpixmap
        QPixmap pixmap = QPixmap::fromImage(*pChip);
        //QPixmap pixmap("/Users/manyanair/Downloads/pokerchip.png"); // TODO Adjust this path
        QPixmap scaledPixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio);

        // Calculate the initial Y position for all labels (top of the window)
        float initialY = 0;

        // Adjust the initial x position and spacing between labels
        float initialX = 10.0f; // Initial x position
        float labelSpacing = 100.0f; // Spacing between labels

        for (int i = 0; i < 9; ++i) {
            QLabel *label = nullptr;
            switch (i) {
            case 0: label = ui->label; break;
            case 1: label = ui->label_2; break;
            case 2: label = ui->label_3; break;
            case 3: label = ui->label_4; break;
            case 4: label = ui->label_5; break;
            case 5: label = ui->label_6; break;
            case 6: label = ui->label_7; break;
            case 7: label = ui->label_8; break;
            case 8: label = ui->label_9; break;
            default: break;
            }

            if (label) {
                // Get the Box2D body position for the current label
                b2Vec2 position = worlds[i]->GetBodyList()->GetPosition();

                // Map the Box2D Y position to QLabel Y position
                float posY = initialY + (this->height() - scaledPixmap.height()) - position.y * 100; // Adjusted for Box2D scale

                // Map the Box2D X position to QLabel X position
                float posX = initialX + labelSpacing * i; // Adjusted for spacing

                // Calculate the final Y position based on the window's height
                float finalY = this->height() - scaledPixmap.height();
                // Smoothly interpolate the Y position towards the final position
                posY += (finalY - posY) * (elapsedTime / totalTime);

                label->setPixmap(scaledPixmap);
                label->move(posX, posY);
            }
        }
    } else {
        // Animation finished, stop the timer
        timer->stop();
    }
}
