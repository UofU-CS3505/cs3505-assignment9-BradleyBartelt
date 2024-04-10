#include "deck.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>


Card::Card(Rank rank, Suit suit, QImage image): rank(rank), suit(suit), image(image){}

Deck::Deck(){
    shuffle();
}

void Deck::shuffle(){
    for(int i=0; i<4; i++) {
        for(int j=2; j<14; j++) {
            Rank rank;
            Suit suit;
            QImage image;

            if(i == 0)
                suit = heart;
            else if(i == 1)
                suit = diamond;
            else if(i == 2)
                suit = club;
            else
                suit = spade;

            if(j == 2)
                rank = two;
            else if(j == 3)
                rank = three;
            else if(j == 4)
                rank = four;
            else if(j == 5)
                rank = five;
            else if(j == 6)
                rank = six;
            else if(j == 7)
                rank = seven;
            else if(j == 8)
                rank = eight;
            else if(j == 9)
                rank = nine;
            else if(j == 10)
                rank = ten;
            else if(j == 11)
                rank = jack;
            else if(j == 12)
                rank = queen;
            else if(j == 13)
                rank = king;
            else
                rank = ace;

            image = QImage(); //we need to add actual image of card here

            cards.push_back(Card(rank, suit, image));
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rd(seed);

    std::shuffle(std::begin(cards), std::end(cards), rd);
}

Card Deck::draw(){
    Card ret = cards.back();
    cards.pop_back();
    std::cout << ret.rank << std::endl;
    return ret;
}
