#include "model.h"
#include "game.h"
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer)
{

}
void Model::hitSlot(){
    game.hit(playerOne);
    emit SendCardImage(playerOne.cardArray.back().image);
}

void Model::SetLevel(int level){
    currentLevel = level;
    initalDeal();
}
void Model::initalDeal(){
    emit addCardToPlayerHand(playerOne->cardArray.at(0).image);
    emit addCardToPlayerHand(playerOne->cardArray.at(1).image);
    QImage cardBack;
    cardBack.load(":/images/cardImages/cardBack.png");
    emit addCardToDealerHand(cardBack);
    emit addCardToDealerHand(dealer->cardArray.at(1).image);
}
