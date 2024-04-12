#include "model.h"
#include "game.h"

Model::Model(QObject *parent)
    : QObject{parent}
{

    Deck deck;
    playerOne = new Player(false);
    dealer = new Player(true);
    game = new Game(deck,*playerOne,*dealer);
}
void Model::hitSlot(){
    game->hit(playerOne);
    // gets the card most recently added to the player hand
    emit SendCardImage(playerOne->cardArray.back().image);
}

void Model::SetLevel(int level){
    currentLevel = level;
    initalDeal();
}
void Model::initalDeal(){
    emit addCardToPlayerHand(playerOne->cardArray.at(0).image);
    emit addCardToPlayerHand(playerOne->cardArray.at(1).image);
    emit addCardToDealerHand(dealer->cardArray.at(0).image);
    emit addCardToDealerHand(dealer->cardArray.at(1).image);
}
