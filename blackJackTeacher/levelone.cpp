#include "levelone.h"
#include "qlabel.h"
#include "ui_levelone.h"

LevelOne::LevelOne(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LevelOne)
{
    ui->setupUi(this);
    QPalette palette;
    // this->setStyleSheet("background-color: green;");
}

LevelOne::~LevelOne()
{
    delete ui;
}

void LevelOne::on_hitButton_clicked()
{
    QHBoxLayout* layout = (QHBoxLayout*)(ui->handArea->widget()->layout());

}

