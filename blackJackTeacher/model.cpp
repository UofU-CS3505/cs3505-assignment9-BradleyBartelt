#include "model.h"
#include "game.h"
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer, false)
{

}
void Model::hitSlot(){
    game.hit(playerOne);
    emit addCardToPlayerHand(playerOne.cardArray.back());
}
void Model::standSlot(){
    game.stand(playerOne);
    std::tuple<int,int> result;
    result = game.endResult();
    // emit playerStood();
}
void Model::SetLevel(int level){
    currentLevel = level;
    initalDeal();
}
void Model::initalDeal(){
    emit addCardToPlayerHand(playerOne.cardArray.at(0));
    emit addCardToPlayerHand(playerOne.cardArray.at(1));
    emit addCardToDealerHand(dealer.cardArray.at(0),true);
    emit addCardToDealerHand(dealer.cardArray.at(1),false);
}

