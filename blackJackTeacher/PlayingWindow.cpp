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
}

PlayingWindow::~PlayingWindow()
{
    delete ui;
}

void PlayingWindow::hitButtonClicked()
{
    QPushButton* newCard = new QPushButton( ui->handArea);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->handArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    emit hit();
}

void PlayingWindow::mainMenuClicked()
{
    mainMenu->show();
    delete this;
}

void PlayingWindow::SetUpConnections(Model& model){
    connect(ui->hitButton,&QPushButton::clicked,&model,&Model::hitSlot);
    connect(ui->hitButton,&QPushButton::clicked,this,&PlayingWindow::hitButtonClicked);
    connect(ui->mainMenu,&QPushButton::clicked,this,&PlayingWindow::mainMenuClicked);

}

