#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"

class game
{
private:
    int personCount; // count that the left hand (or center if no split) totals to
    int dealerCount; // dealers count
    int personSplitCount; // count that the right hand totals to
    Deck gameDeck; // the given deck
public:
    game(Deck gameDeck, player person, player dealer);
    /// checks whether the dealer or the player has lost
    void checkState(player currentPlayer);
    /// adds card to player
    void hit(player person);
    /// splits a persons hand
    void split(player person);
    /// doubles the current bet
    void doubleBet();
    /// disables further actions on players current hand
    void stand(player person);
    /// decides who won or loss
    void endResult();
    /// clears person and dealer hand, as well as, shuffles the deck
    void resetGame(player person, player dealer);
    /// initial checker for whether the dealer or the player or both got a blackjack
    void checkBlackJack(player person, player dealer);
};

#endif // GAME_H
