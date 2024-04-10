#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "levelone.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    levelOneWindow = new LevelOne(nullptr,this);

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_levelOne_clicked()
{
    levelOneWindow->show();
    this->hide();
}

