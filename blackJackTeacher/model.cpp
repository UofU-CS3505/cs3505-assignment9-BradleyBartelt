#include "model.h"
#include "game.h"

Model::Model(QObject *parent)
    : QObject{parent}
{

    Deck deck;
    playerOne = new Player(false);
    dealer = new Player(true);
    game = new Game(deck,playerOne,dealer);
}
void Model::hitSlot(){
    game->hit(playerOne);
}

void Model::SetLevel(int level){
    currentLevel = level;
}
