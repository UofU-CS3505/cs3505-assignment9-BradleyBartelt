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
void Model::standSlot(){
    emit disableButtons(false);
    // if(game.stand(playerOne) == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
    //     emit disableButtons(false);
    //     std::tuple<bool, QImage, int> gameTuple = game.hit(dealer);
    //     while(get<0>(gameTuple)){
    //         if(dealer.getState()){

    //         }
    //         //display card image after a delay
    //     }
    //     if(!get<0>(gameTuple)){
    //         // emit dealer bust
    //     }
    // }

}
void Model::hitSlot(){
    game.hit(playerOne);
    emit addCardToPlayerHand(playerOne.cardArray.back());
}
void Model::SetLevel(int level){
    currentLevel = level;
    initalDeal();
}
void Model::initalDeal(){
    emit addCardToPlayerHand(playerOne.cardArray.at(0));
    emit addCardToPlayerHand(playerOne.cardArray.at(1));
    emit addCardToDealerHand(dealer.cardArray.at(0), true);
    emit addCardToDealerHand(dealer.cardArray.at(1), false);
}
