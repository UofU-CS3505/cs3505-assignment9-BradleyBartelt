#include "model.h"
#include "game.h"
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer, false)
{

}
void Model::standSlot(){
    emit disableButtons(false);
    // if(game.stand(playerOne) == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
    //     emit disableButtons(false);
    //     std::tuple<bool, QImage, int> gameTuple = game.hit(dealer);
    //     while(get<0>(gameTuple)){
    //         if(dealer.getState()){

    //         }
    //         //display card image after a delay
    //     }
    //     if(!get<0>(gameTuple)){
    //         // emit dealer bust
    //     }
    // }

}
void Model::hitSlot(){
    game.hit(playerOne);
    emit addCardToPlayerHand(playerOne.cardArray.back());
}
void Model::SetLevel(int level){
    currentLevel = level;
    levelScript.setScript(":/scripts/levelScripts/levelOneScript.txt");//Change this once we get something better figured out
    //game = Game(deck, playerOne, dealer, true);
    QString messagetype = levelScript.nextCommand(&scriptOutputDetails);
    messagetype = levelScript.nextCommand(&scriptOutputDetails);
    interpretCommand(messagetype);
    //initalDeal();

}
void Model::initalDeal(){
    emit addCardToPlayerHand(playerOne.cardArray.at(0));
    emit addCardToPlayerHand(playerOne.cardArray.at(1));
    emit addCardToDealerHand(dealer.cardArray.at(0), true);
    emit addCardToDealerHand(dealer.cardArray.at(1), false);
}
void Model::readyForNextLine()
{
    QString messageType = levelScript.nextCommand(&scriptOutputDetails);
    interpretCommand(messageType);
}

void Model::interpretCommand(QString messageType)
{
    //TODO: Switch case here?
    if(messageType == "error")
    {
        //EndLevel
        emit endLevel(true);
    }
    else if (messageType == "message")
    {
        //emit message to the view (it will show the popup)
        emit sendMessage(scriptOutputDetails);
    }
    else if (messageType == "deal")
    {
        //TODO: Figure out this logic
        //parse the content and deal the correct card

    }
    else if (messageType == "reveal")
    {
        //reveal hole
        emit revealHole();
    }
    else if (messageType == "finish")
    {
        //emit end level signal
        emit endLevel(false);
    }
    else if (messageType == "shuffle")
    {
        //Pull all cards back into the deck and shuffle
    }
    else
    {
        //This is an error state. We should be throwing here, or something
        emit endLevel(true);
    }
}
