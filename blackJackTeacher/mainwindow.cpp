#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "levelone.h"
#include "model.h"

MainWindow::MainWindow(QWidget *parent, Model* model)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupConnections();
    setupWindowDisplay();
    this->model = model;
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_levelOne_clicked(){
    this->levelOneWindow = new LevelOne(nullptr,this);
    levelOneWindow->show();
    this->hide();
}

void MainWindow::setupConnections(){
    //connect(&levelOneWindow,&levelOneWindow::hit,&model,model::hitSlot);
}
void::MainWindow::setupWindowDisplay(){
    QPixmap bkgnd(":/images/BlackjackWelcome.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    ui->welcomeText->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%) ; color : white; }");
    ui->levelOne->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");
    ui->levelTwo->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");
    ui->levelThree->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");
    ui->levelFour->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");

    this->setPalette(palette);
}

