#include "game.h"
#include "deck.h"
#include "player.h"

Game::Game(Deck gameDeck, Player person, Player dealer) {
    gameDeck = Deck();
    person = Player(false);
    dealer = Player(true);
    person.addCard(gameDeck.draw()); // add cards to player and dealer (emit these cards)
    dealer.addCard(gameDeck.draw());
    person.addCard(gameDeck.draw());
    dealer.addCard(gameDeck.draw()); // dont show the player this card
    checkBlackJack(person, dealer);
}

void Game::checkBlackJack(Player person, Player dealer){
    personCount = person.cardArray.at(0).rank + person.cardArray.at(1).rank; // get inital personCount
    dealerCount = dealer.cardArray.at(0).rank + dealer.cardArray.at(1).rank; // get inital dealerCount
    if(personCount == 21 && dealerCount != 21){
        //emit person win + 1.5, if bet 5, get 12.50 (remember when you bet you deduct 5)
    }
    else if(dealerCount == 21 && personCount != 21){
        // emit dealer win
    }
    else if(dealerCount == 21 && personCount == 21){
        // emit tie
    }
}

void Game::checkState(Player currentPlayer){
    if(currentPlayer.getIsDealer()){ // adds up the dealers count when they hit
        dealerCount = 0;
        int aceCount = 0;
        for(int i = 0; i < int(currentPlayer.cardArray.size()); i++){
            dealerCount += currentPlayer.cardArray.at(i).rank;
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
        if(dealerCount > 17 && dealerCount < 22){ // if a dealer is between a 17 and 21 stand
            stand(currentPlayer);
        }
        else{
            // emit dealer loss or something
        }
    }
    if(!currentPlayer.getIsDealer()){ // same as above
        personCount = 0;
        int aceCount = 0;
        for(int i = 0; i < int(currentPlayer.cardArray.size()); i++){
            personCount += currentPlayer.cardArray.at(i).rank;
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
            // emit something for a loss on that hand idk
        }
        if(int(currentPlayer.splitArray.size()) != 0){ // same as above but for a split hand
            personSplitCount = 0;
            aceCount = 0;
            for(int i = 0; i < int(currentPlayer.splitArray.size()); i++){
                personSplitCount += currentPlayer.splitArray.at(i).rank;
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
                // emit something for a loss on that hand idk
            }
        }
    }
}
void Game::endResult(){
    if(personCount < dealerCount){
        // emit player loss (left hand)
    }
    if(personCount == dealerCount){
        // emit tie (right hand)
    }
    if(personCount > dealerCount){
        // emit person win (left hand)
    }
    if(personSplitCount != 0){
        if(personSplitCount == dealerCount){
            // emit tie for the split hand (right hand)
        }
        if(personSplitCount < dealerCount){
            // emit loss for the split hand (right hand)
        }
        if(personSplitCount > dealerCount){
            // emit win for the split hand (right hand)
        }
    }
}
void Game::hit(Player currentPlayer){
    if(!currentPlayer.getState()){ // if the current player has not chose to stand
        currentPlayer.addCard(gameDeck.draw());
        checkState(currentPlayer);
        // emit signal to disable hit button for person and if its a dealer, disable the ability to hit
    }
}

void Game::split(Player person){
    if(person.cardArray.at(0).rank == person.cardArray.at(1).rank){
        person.addHand(gameDeck.draw(), gameDeck.draw());
        //disable split button because we only allow one split
    }
    // say something about the cards not being equal so you cannot split
}

void Game::stand(Player currentPlayer){
    currentPlayer.setState(true);
    // emit to disable stand button
}

void Game::doubleBet(){
    // if we work with bets we can make this method do something
}

void Game::resetGame(Player person, Player dealer){
    gameDeck.shuffle();
    person.resetPlayer();
    dealer.resetPlayer();
    personCount = 0;
    dealerCount = 0;
    personSplitCount = 0;
}
