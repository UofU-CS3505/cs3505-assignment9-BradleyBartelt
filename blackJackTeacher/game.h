#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"

class Game
{

private:
    Deck gameDeck; // the given deck
    int personHitCount = 0;
    bool isRigged;
public:
    int personCount; // count that the left hand (or center if no split) totals to
    int dealerCount; // dealers count
    int personSplitCount; // count that the right hand totals to
    Game(Deck deck, Player& person, Player& dealer, bool isRigged);

    Game(const Game& other);
    //destructor
    ~Game();
    //equal method
    Game& operator=(Game other);
    /// checks whether the dealer or the player has lost, false is player bust, true means that the player did not bust or the dealer bust
    std::tuple<bool,int> checkState(Player currentPlayer);
    /// adds card to player
    std::tuple<bool,int> hit(Player& person);
    /// splits a persons hand
    bool split(Player& person);
    /// doubles the current bet
    void doubleBet();
    /// disables further actions on players current hand
    int stand(Player& person);
    /// decides who won or loss, bool determines whether the dealer won or the player won, and the int determines which hand won
    std::tuple<bool, int> endResult();
    /// clears person and dealer hand, as well as, shuffles the deck
    void resetGame(Player& person, Player& dealer);
    /// initial checker for whether the dealer or the player or both got a blackjack
    int checkBlackJack(Player& person, Player& dealer);
    /// rigs Game
    std::tuple<bool,int> hit(Player& person, Card card);

};

#endif // GAME_H
