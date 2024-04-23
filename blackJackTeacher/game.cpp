#include "game.h"
#include "deck.h"
#include "player.h"
#include <iostream>

Game::Game(Deck& deck, Player& person, Player& dealer, bool rigged) {
    gameDeck = deck;
    isRigged = rigged;
    if(!isRigged){
        isRigged = false;
        person.addCard(gameDeck.draw()); // add cards to player and dealer (emit these cards)
        dealer.addCard(gameDeck.draw());
        person.addCard(gameDeck.draw());
        dealer.addCard(gameDeck.draw()); // dont show the player this card
    }
    else{
        isRigged = true;
    }
    checkState(dealer);
    checkState(person);
}
Game::Game(const Game& otherGame){
    gameDeck = otherGame.gameDeck;
    personCount = otherGame.personCount;
    dealerCount = otherGame.dealerCount;
    personSplitCount = otherGame.personSplitCount;
    isRigged = otherGame.isRigged;
}
Game& Game::operator=(Game otherGame){
    std::swap(gameDeck,otherGame.gameDeck);
    std::swap(personCount,otherGame.personCount);
    std::swap(dealerCount,otherGame.dealerCount);
    std::swap(personSplitCount,otherGame.personSplitCount);
    std::swap(isRigged, otherGame.isRigged);
    return *this;
}
Game::~Game(){}
Deck Game::getDeck(){
    return gameDeck;
}
int Game::checkBlackJack(Player& person, Player& dealer){
    personCount = person.cardArray.at(0).value + person.cardArray.at(1).value; // get inital personCount
    dealerCount = dealer.cardArray.at(0).value + dealer.cardArray.at(1).value; // get inital dealerCount
    if(personCount == 21 && dealerCount != 21){
        //emit person win + 1.5, if bet 5, get 12.50 (remember when you bet you deduct 5)
        return 1;
    }
    else if(dealerCount == 21 && personCount != 21){
        // emit dealer win
        return 2;
    }
    else if(dealerCount == 21 && personCount == 21){
        // return tie
        return 3;
    }
    return 4;
}

std::tuple<bool,int> Game::checkState(Player currentPlayer){
    if(currentPlayer.getIsDealer()){ // adds up the dealers count when they hit
        dealerCount = 0;
        int aceCount = 0;
        for(int i = 0; i < int(currentPlayer.cardArray.size()); i++){
            dealerCount += currentPlayer.cardArray.at(i).value;
            if(currentPlayer.cardArray.at(i).rank == ace){
                aceCount += 1;
            }
        }
        while(aceCount > 0){ // if an ace is present, this logic decides whether its a 1 or 11
            if(dealerCount > 21){
                dealerCount -= 10;
                aceCount -= 1;
            }
            else{
                break;
            }
        }
        if(dealerCount >= 17 && dealerCount < 22){ // if a dealer is between a 17 and 21 stand
            stand(currentPlayer);
            return std::tuple<bool,int>(true,0);
        }
        if(dealerCount > 21){
            return std::tuple<bool,int>(false,0);
        }
        else{
            return std::tuple<bool,int>(true,0);
        }
    }
    if(!currentPlayer.getIsDealer()){ // same as above
        personCount = 0;
        int aceCount = 0;
        for(int i = 0; i < int(currentPlayer.cardArray.size()); i++){
            personCount += currentPlayer.cardArray.at(i).value;
            if(currentPlayer.cardArray.at(i).rank == ace){
                aceCount += 1;
            }
        }
        while(aceCount > 0){
            if(personCount > 21){
                personCount -= 10;
                aceCount -= 1;
            }
            else{
                break;
            }
        }
        if(personCount > 21){ // if the player goes above a 21 they lose
            return std::tuple<bool,int>(false,0);
        }
        if(int(currentPlayer.splitArray.size()) != 0){ // same as above but for a split hand
            personSplitCount = 0;
            aceCount = 0;
            for(int i = 0; i < int(currentPlayer.splitArray.size()); i++){
                personSplitCount += currentPlayer.splitArray.at(i).value;
                if(currentPlayer.splitArray.at(i).rank == ace){
                    aceCount += 1;
                }
            }
            while(aceCount > 0){
                if(personSplitCount > 21){
                    personSplitCount -= 10;
                    aceCount -= 1;
                }
                else{
                    break;
                }
            }
            if(personSplitCount > 21){
                return std::tuple<bool,int>(false,1);
            }
        }
    }
    return std::tuple<bool,int>(true, currentPlayer.currentHand);
}

QString Game::endResult(){
    if(dealerCount > 21){
        return "win";
    }
    if(personCount < dealerCount){
        return "loss";
    }
    if(personCount == dealerCount){
        return "tie";
    }
    if(personCount > dealerCount){
        return "win";
    }
    if(personSplitCount != 0){ // if a split exists
        if(personSplitCount == dealerCount){
            return "splitTie";
        }
        if(personSplitCount < dealerCount){
            return "splitLoss";
        }
        if(personSplitCount > dealerCount){
            return "splitWin";
        }
    }
    return "";
}
std::tuple<bool,int> Game::hit(Player& currentPlayer){
    currentPlayer.addCard(gameDeck.draw());
    std::tuple<bool, int> playerState = checkState(currentPlayer);
    if(get<0>(playerState) && get<1>(playerState) == 0 && personCount == 21){ // if its true that means the game can continue
        return std::tuple<bool, int>(true,3);
    }
    if(get<0>(playerState) && get<1>(playerState) == 1 && personSplitCount == 21){ // if its true that means the game can continue
        return std::tuple<bool, int>(true,4);
    }
    if(get<0>(playerState) && get<1>(playerState) == 0){ // if its true that means the game can continue
        return std::tuple<bool, int>(true,currentPlayer.currentHand);
    }
    else if(!get<0>(playerState) && get<1>(playerState) == 0){ // if its false the player has lost
        return std::tuple<bool, int>(false, currentPlayer.currentHand);
    }
    if(get<0>(playerState) && get<1>(playerState) == 1){
        return std::tuple<bool, int>(true, currentPlayer.currentHand);
    }
    else{
        int tempVal = currentPlayer.currentHand;
        currentPlayer.currentHand = 0;
        return std::tuple<bool, int>(false, tempVal);
    }
}

std::tuple<bool,int> Game::hit(Player& currentPlayer, Card card){
    if(currentPlayer.getIsDealer() && dealerCount > 16){
        stand(currentPlayer);
        return std::tuple<bool, int>(true, 0);
    }
    currentPlayer.addCard(gameDeck.draw(card.rank,card.suit));
    std::tuple<bool, int> playerState = checkState(currentPlayer);
    if(get<0>(playerState) && get<1>(playerState) == 0 && personCount == 21){ // if its true that means the game can continue
        stand(currentPlayer);
        return std::tuple<bool, int>(true,3);
    }
    if(get<0>(playerState) && get<1>(playerState) == 0){ // if its true that means the game can continue
        return std::tuple<bool, int>(true,currentPlayer.currentHand);
    }
    else if(!get<0>(playerState) && get<1>(playerState) == 0){ // if its false the player has lost
        return std::tuple<bool, int>(false, currentPlayer.currentHand);
    }
    if(get<0>(playerState) && get<1>(playerState) == 1){
        return std::tuple<bool, int>(true, currentPlayer.currentHand);
    }
    else{
        int tempVal = currentPlayer.currentHand;
        currentPlayer.currentHand = 0;
        return std::tuple<bool, int>(false, tempVal);
    }
    // emit signal to disable hit button for person and if its a dealer, disable the ability to hit
}

bool Game::split(Player& person){
    person.addHand(gameDeck.draw(), gameDeck.draw());
    return true;
}

int Game::stand(Player& currentPlayer){
    std::cout << currentPlayer.currentHand << std::endl;
    currentPlayer.setState(true);
    if(currentPlayer.currentHand == 0)
        return 0;
    currentPlayer.currentHand -= 1;
    return 1;
}

void Game::doubleBet(){
    // if we work with bets we can make this method do something
}

