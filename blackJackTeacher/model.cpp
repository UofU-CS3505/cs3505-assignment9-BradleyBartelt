#include "model.h"
#include "game.h"
#include <QTimer>
#include <iostream>
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer, false)
{

}
void Model::standSlot(){
    if(game.stand(playerOne) == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit disableButtons(false);
        std::tuple<bool, int> gameTuple = game.hit(dealer);
        int waitTime = 1000;
        int count = 1;
        while(get<0>(gameTuple)){
            waitTime = abs(count * waitTime);
            if(dealer.getState()){
                // do stuff to represent end game
                break;
            }
            game.hit(dealer);
            QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
            count++;
        }
        if(!get<0>(gameTuple)){
            // emit dealer bust
        }
    }
}
void Model::hitSlot(){
    std::tuple<bool, int> gameTuple = game.hit(playerOne);
    if(get<0>(gameTuple) && get<1>(gameTuple) != 3)
        emit addCardToPlayerHand(playerOne.cardArray.back());
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 3){ //player gets a 21
        emit addCardToPlayerHand(playerOne.cardArray.back());
        emit disableButtons(false);
    }
    else{
        emit addCardToPlayerHand(playerOne.cardArray.back());
        // emit a loss
        emit disableButtons(false);
    }

}
void Model::SetLevel(int level){
    currentLevel = level;
    initalDeal();
}
void Model::initalDeal(){
    emit addCardToPlayerHand(playerOne.cardArray.at(0));
    emit addCardToDealerHand(dealer.cardArray.at(0), true);
    emit addCardToPlayerHand(playerOne.cardArray.at(1));
    emit addCardToDealerHand(dealer.cardArray.at(1), false);
    int checkBlackJack = game.checkBlackJack(playerOne,dealer);
    if(checkBlackJack == 1){
        emit disableButtons(false);
        // emit player win
    }
    else if(checkBlackJack == 2){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        // emit player loss
    }
    else if(checkBlackJack == 3){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        // emit tie
    }
}
