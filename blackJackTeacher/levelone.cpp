#include "levelone.h"
#include "ui_levelone.h"

LevelOne::LevelOne(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LevelOne)
{
    ui->setupUi(this);
}

LevelOne::~LevelOne()
{
    delete ui;
}
