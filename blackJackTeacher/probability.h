#ifndef PROBABILITY_H
#define PROBABILITY_H

#include "deck.h"
#include <vector>
///
/// \brief The Probability class: Provides some methods for calculating useful metrics like the likelyhood of a bust.
/// \author Emma Eastland
///
class Probability
{
public:
    ///
    /// \brief Probability default constructor
    ///
    Probability();
    ///
    /// \brief probabilityOfDealerBust calculates the probability of the dealer busting on the next draw. Use the two argument version for when the player has not split
    /// \param dealersHand a vector of cards representing the dealer's hand
    /// \param playersHand a vector of cards representing the player's hand
    /// \return a double percentage representing the likelyhood that the next card will cause the value of the dealer's hand to exceed 21
    ///
    double probabilityOfDealerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand);
    ///
    /// \brief probabilityOfDealerBust calculates the probability of the dealer busting on the next draw. Use the three argument version for when the player has split
    /// \param dealersHand a vector of cards representing the dealer's hand
    /// \param playersHand1 the primary hand. This is only being tallied. We DO NOT calculate odds for this hand
    /// \param playersHand2 the secondary hand. This is only being tallied. We DO NOT calculate odds for this hand
    /// \return a double percentage representing the likelyhood that the next card will cause the value of the dealer's hand to exceed 21 (0-100)
    ///
    double probabilityOfDealerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand1, std::vector<Card>playersHand2);
    ///
    /// \brief probabilityOfPlayerBust calculates the probability of the player busting on their next draw. Use the two argument version for when the player has not split
    /// \param dealersHand the dealer's hand. We dont calculate any odds for this one
    /// \param playersHand a vector of cards representing the players hand
    /// \return a double percentage representing the likelyhood that the next card will cause the value of the player's hand to exceed 21 (0-100)
    ///
    double probabilityOfPlayerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand);
    ///
    /// \brief probabilityOfPlayerBust calculates the probability of the player busting on their next draw. Use the three argument version for when the player has split
    /// \param dealersHand the dealer's hand. We dont calculate any odds for this one
    /// \param playersHand1 the primary hand. a vector representing the player's hand that they are currently hitting on
    /// \param playersHand2 the secondary hand. This is only being tallied. We DO NOT calculate odds for this hand
    /// \return a double percentage representing the likelyhood that the next card will cause the value of the player's hand to exceed 21 (0-100)
    ///
    double probabilityOfPlayerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand1, std::vector<Card>playersHand2);
private:
    ///
    /// \brief probabilityOfBust Calculates the probability that the next dealt card will cause the first provided hand to go bust
    /// \param handToAnalyze the hand we are calculating odds on
    /// \param otherHand the hand we are tallying. We need to take a census of the cards left in the deck
    /// \return a double percentage representing the likelyhood that the next card will cause the value of the first hand to exceed 21 (0-1)
    ///
    double probabilityOfBust(std::vector<Card> handToAnalyze, std::vector<Card> otherHand);
    ///
    /// \brief probabilityOfBustSplit Calculates the probability that the next dealt card will cause the first provided hand to go bust
    /// \param handToAnalyze the hand we are calculating odds on
    /// \param otherHand the hand we are tallying. Taken in to provide census
    /// \param thirdHand another hand we are tallying. Taken in to provide census
    /// \return a double percentage representing the likelyhood that the next card will cause the value of the first hand to exceed 21 (0-1)
    ///
    double probabilityOfBustSplit(std::vector<Card> handToAnalyze, std::vector<Card> otherHand, std::vector<Card> thirdHand);
};

#endif // PROBABILITY_H
