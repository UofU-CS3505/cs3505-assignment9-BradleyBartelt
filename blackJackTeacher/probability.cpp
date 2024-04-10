#include "probability.h"

Probability::Probability() {}

double Probability::probabilityOfDealerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand)
{
    return probabilityOfBust(dealersHand, playersHand)*100;
}

double Probability::probabilityOfDealerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand1, std::vector<Card>playersHand2)
{
    return probabilityOfBustSplit(dealersHand, playersHand1, playersHand2)*100;
}

double Probability::probabilityOfPlayerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand)
{
    return probabilityOfBust(playersHand, dealersHand)*100;
}

double Probability::probabilityOfPlayerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand1, std::vector<Card>playersHand2)
{
    return probabilityOfBustSplit(playersHand1, playersHand2, dealersHand)*100;
}
///BACKERS------------------------------------------

double Probability::probabilityOfBust(std::vector<Card> handToAnalyze, std::vector<Card> otherHand)
{
    //Track the number of cards remaining in the deck
    int cardsRemaining = 52;
    //Begin by assuming all cards are in the deck
    int inDeck[13];
    for(int i = 0; i<13;i++)
    {
        inDeck[i] = 4;
    }
    //Hold a tally of the handToAnalyze's cards
    int handSum = 0;
    for(Card card:handToAnalyze)
    {
        if(card.rank == ace)
        {
            inDeck[0] -= 1;
            handSum += 1;
        }
        else if(card.rank == jack)
        {
            inDeck[10] -= 1;
            handSum += 10;
        }
        else if(card.rank == queen)
        {
            inDeck[11] -= 1;
            handSum += 10;
        }
        else if(card.rank == king)
        {
            inDeck[12] -= 1;
            handSum += 10;
        }
        else
        {
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0
            handSum += card.rank;
        }
        cardsRemaining --;
    }

    //Remove the cards in the other hand from the pool of possible cards
    for(Card card:otherHand)
    {
        if(card.rank == ace)
            inDeck[0] -= 1;
        else if(card.rank == jack)
            inDeck[10] -= 1;
        else if(card.rank == queen)
            inDeck[11] -= 1;
        else if(card.rank == king)
            inDeck[12] -= 1;
        else
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

        cardsRemaining --;
    }

    int minimumToBust = 22- handSum;
    int cardsOverMin =0;
    for(int i = (minimumToBust-1);i<13;i++)//Have to subtract one because we start at 0
        cardsOverMin += inDeck[i];
    return (double)cardsOverMin/cardsRemaining;
}


double Probability::probabilityOfBustSplit(std::vector<Card> handToAnalyze, std::vector<Card> otherHand, std::vector<Card> thirdHand)
{
    //Track the number of cards remaining in the deck
    int cardsRemaining = 52;
    //Begin by assuming all cards are in the deck
    int inDeck[13];
    for(int i = 0; i<13;i++)
    {
        inDeck[i] = 4;
    }
    //Hold a tally of the handToAnalyze's cards
    int handSum = 0;
    for(Card card:handToAnalyze)
    {
        if(card.rank == ace)
        {
            inDeck[0] -= 1;
            handSum += 1;
        }
        else if(card.rank == jack)
        {
            inDeck[10] -= 1;
            handSum += 10;
        }
        else if(card.rank == queen)
        {
            inDeck[11] -= 1;
            handSum += 10;
        }
        else if(card.rank == king)
        {
            inDeck[12] -= 1;
            handSum += 10;
        }
        else
        {
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0
            handSum += card.rank;
        }
        cardsRemaining --;
    }

    //Remove the cards in the other hand from the pool of possible cards
    for(Card card:otherHand)
    {
        if(card.rank == ace)
            inDeck[0] -= 1;
        else if(card.rank == jack)
            inDeck[10] -= 1;
        else if(card.rank == queen)
            inDeck[11] -= 1;
        else if(card.rank == king)
            inDeck[12] -= 1;
        else
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

        cardsRemaining --;
    }

    //Remove the cards in the dealers hand from the pool of possible cards
    for(Card card:thirdHand)
    {
        if(card.rank == ace)
            inDeck[0] -= 1;
        else if(card.rank == jack)
            inDeck[10] -= 1;
        else if(card.rank == queen)
            inDeck[11] -= 1;
        else if(card.rank == king)
            inDeck[12] -= 1;
        else
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

        cardsRemaining --;
    }

    int minimumToBust = 22- handSum;
    int cardsOverMin =0;
    for(int i = (minimumToBust-1);i<13;i++)//Have to subtract one because we start at 0
        cardsOverMin += inDeck[i];
    return (double)cardsOverMin/cardsRemaining;
}
