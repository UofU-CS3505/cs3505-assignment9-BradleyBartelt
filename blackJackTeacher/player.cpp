#include "player.h"
#include "deck.h"

player::player(bool isPlayerDealer){
    isDealer = isPlayerDealer;
}
void player::addCard(Card topCard){
    if(currentHand == 0){
        cardArray.push_back(topCard);
    }
    else{
        splitArray.push_back(topCard);
    }

}
void player::addHand(Card topCard, Card cardAfter){
    splitArray.push_back(cardArray.back()); // becomes hand on the right of player
    cardArray.pop_back();
    splitArray.push_back(topCard); // adds card to hand on the right
    cardArray.push_back(cardAfter); // adds card to hand on the left
    currentHand = 1; // sets current hand to the splitArray
}
void player::setState(bool state){
    if(currentHand == 0)
         choseStand[0] = state;
    else
        choseStand[1] = state;
}
void player::resetPlayer(){
    choseStand[0] = false;
    choseStand[1] = false;
    splitArray.clear();
    cardArray.clear();
    currentHand = 0;
}
bool player::getState(){
    if(currentHand == 0)
        return choseStand[0];
    return choseStand[1];
}
bool player::getIsDealer(){
    return isDealer;
}
