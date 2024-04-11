#include "deck.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>
#include <QUrl>


Card::Card(Rank rank, Suit suit, QImage& image): rank(rank), suit(suit), image(image){}

Deck::Deck(){
    shuffle();
}

void Deck::shuffle(){
    for(int i=0; i<4; i++) {
        for(int j=2; j<=14; j++) {
            Rank rank;
            Suit suit;
            QImage image;
            QString filename = "";

            if(i == 0){
                suit = heart;
                filename += "heart";
            }
            else if(i == 1){
                suit = diamond;
                filename += "diamond";
            }
            else if(i == 2){
                suit = club;
                filename += "club";
            }
            else{
                suit = spade;
                filename += "spade";
            }

            if(j == 2){
                rank = two;
                filename += "Two";
            }
            else if(j == 3){
                rank = three;
                filename += "Three";
            }
            else if(j == 4){
                rank = four;
                filename += "Four";
            }
            else if(j == 5){
                rank = five;
                filename += "Five";
            }
            else if(j == 6){
                rank = six;
                filename += "Six";
            }
            else if(j == 7){
                rank = seven;
                filename += "Seven";
            }
            else if(j == 8){
                rank = eight;
                filename += "Eight";
            }
            else if(j == 9){
                rank = nine;
                filename += "Nine";
            }
            else if(j == 10){
                rank = ten;
                filename += "Ten";
            }
            else if(j == 11){
                rank = jack;
                filename += "Jack";
            }
            else if(j == 12){
                rank = queen;
                filename += "Queen";
            }
            else if(j == 13){
                rank = king;
                filename += "King";
            }
            else{
                rank = ace;
                filename += "Ace";
            }

            image.load(":/images/cardImages/" + filename + ".png");

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
    return ret;
}
