#include "model.h"
#include "game.h"
#include <QTimer>
#include <iostream>
#include <ostream>
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer, false)
{
    probability = Probability();
}
void Model::standSlot(){
    // if a 1 is returned then the player has stood on their split hand, this makes it so the players
    // hits now only work on the left hand (represented by a 0)
    int num = game.stand(playerOne);
    if(num == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
        emit SendCardImage(dealer.cardArray.begin()->image); // flips dealer card
        emit updateDealerCount(QString(QString::number(game.dealerCount))); // updates the count
        emit disableButtons(false); // disables buttons so that the player cannot hit or stand or split while the dealer deals
        if(game.dealerCount > 16){
            endGame();
            return;
        }
        int waitTime = 1000;
        int count = 1;
        while(true){
            waitTime = abs(count * waitTime);
            if(isRigged){
                get<0>(game.hit(dealer, riggedCards[nextCard]));
                nextCard++;
            }
            else{
                get<0>(game.hit(dealer));
            }
            if(game.dealerCount > 16){
                // end game stuff
                QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
                emit updateDealerCount(QString(QString::number(game.dealerCount)));
                endGame();
                break;
            }
            QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
            QTimer::singleShot(waitTime, this,[=]{ emit updateDealerCount(QString(QString::number(game.dealerCount)));});
            count++;
        }
    }
    else{
        std::tuple<bool, int> stateCheck = game.checkState(playerOne); // checks if the main hand is a 21
        if(get<1>(stateCheck) == 3){
            standSlot();
        }
    }
}
void Model::endGame(){
    if(!isRigged){
        QTimer::singleShot(3000, this,[=]{ emit enableDealCards(true);});
        QTimer::singleShot(3000, this,[=]{ emit enableMainMenu(true);});
    }
    std::tuple endResult = game.endResult();
    if(get<0>(endResult) == "tie"){
        if(get<1>(endResult) == "splitTie"){
            emit changeEndGameMessage("You tied on both hands");
        }
        else if(get<1>(endResult) == "splitWin"){
            emit changeEndGameMessage("You tied on your main hand and won your split hand");
        }
        else if(get<1>(endResult) == "splitLoss"){
            emit displayEndGameMessage(true);
            emit changeEndGameMessage("You tied on your main hand and lost your split hand");
        }
        else{
            emit changeEndGameMessage("You tied");
        }
    }
    else if(get<0>(endResult) == "loss"){
        if(get<1>(endResult) == "splitTie"){
            emit changeEndGameMessage("You tied on your main hand and tied on your split hand");
        }
        else if(get<1>(endResult) == "splitWin"){
            emit changeEndGameMessage("You lost on your main hand and won your split hand");
        }
        else if(get<1>(endResult) == "splitLoss"){
            emit changeEndGameMessage("You lost on both hands");
        }
        else{
            emit changeEndGameMessage("You lost");
        }
    }
    else if(get<0>(endResult) == "win"){
        if(get<1>(endResult) == "splitTie"){
            emit changeEndGameMessage("You won on your main hand and tied on your split hand");
        }
        else if(get<1>(endResult) == "splitWin"){
            emit displayEndGameMessage(true);
            emit changeEndGameMessage("You won on both hands");
        }
        else if(get<1>(endResult) == "splitLoss"){
            emit displayEndGameMessage(true);
            emit changeEndGameMessage("You won on your main hand and lost your split hand");
        }
        else{
            emit changeEndGameMessage("You won");
        }
    }
    emit displayEndGameMessage(true);
}
void Model::hitSlot(){
    std::tuple<bool, int> gameTuple;
    if(isRigged){
        // pass in whatever card that needs to be rigged into the game
        gameTuple = game.hit(playerOne,riggedCards[nextCard]);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        nextCard++;
        readyForNextLine();
        emit sendProbabilities(probability.probabilityOfDealerBust(dealer.cardArray, playerOne.cardArray, true),
                               probability.probabilityOfDealerExceeding(dealer.cardArray, playerOne.cardArray));
    }
    else{
        gameTuple = game.hit(playerOne);
    }
    if(get<0>(gameTuple) && get<1>(gameTuple) == 0){
        emit addCardToPlayerHand(playerOne.cardArray.back(), false);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
    }
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 1){
        emit addCardToPlayerHand(playerOne.splitArray.back(), true);
    }
    else if(!get<0>(gameTuple) && get<1>(gameTuple) == 1){
        emit addCardToPlayerHand(playerOne.splitArray.back(), true);
    }
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 3){ //player gets a 21
        emit addCardToPlayerHand(playerOne.cardArray.back(), false);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        emit disableButtons(false);
        standSlot();
    }
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 4){ //players split hand gets a 21
        emit addCardToPlayerHand(playerOne.splitArray.back(), true);
        standSlot();
    }
    else{
        emit addCardToPlayerHand(playerOne.cardArray.back(), false);
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        endGame();
        emit disableButtons(false);
        emit enableDealCards(true);
    }

}
void Model::dealCards(){
    if(currentLevel < 4)
        isRigged = true;
    else
        isRigged = false;
    initialDeal();
    emit disableButtons(true);
    emit enableDealCards(false);
}
void Model::SetLevel(int level){
    currentLevel = level;

    if(currentLevel < 4)
    {
        isRigged = true;
        QString levelNum;
        switch(level){
            case(1):
            {
                levelNum = "One";
                break;
            }
            case(2):
            {
                levelNum = "Two";
                break;
            }
            default:
            {
                levelNum = "Three";
                break;
            }
        }
        emit sendReadingScript(isRigged);
        QString scriptName =":/scripts/levelScripts/level";
        scriptName.append(levelNum);
        levelScript.setScript(scriptName.append("Script.txt").toStdString());
        QString messagetype = levelScript.nextCommand(&scriptOutputDetails);
        interpretCommand(messagetype);
        emit sendLock("next\n");
    }
    else
        isRigged = false;

    initialDeal();
    //riggedCards.clear();
}
void Model::mainMenuSlot(){
    std::cout << "reset stuff" << std::endl;
    riggedCards.clear();
    deck.shuffle();
    levelScript.lineNum = 0;
    nextCard = 0;
    emit endLevel(true);
}
void Model::resetGame(){
    playerOne.resetPlayer();
    dealer.resetPlayer();
    deck.shuffle();
}
void Model::initialDeal(){
    emit displayEndGameMessage(false);
    if(isRigged){
        resetGame();
        Game riggedGame(deck, playerOne, dealer, true);
        game = riggedGame;
        // hit with specific cards in the order of person dealer person dealer
        for(int i = 0; i<4; i++)
        {
            if(i%2==0)
                game.hit(playerOne, riggedCards[nextCard]);
            else
                game.hit(dealer, riggedCards[nextCard]);
            nextCard++;
        }
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        int temp = game.dealerCount;
        temp = temp - dealer.cardArray.at(0).value;
        emit updateDealerCount(QString(QString::number(temp)));
    }
    else{
        resetGame();
        Game newGame(deck, playerOne, dealer, false);
        game = newGame;
        emit updatePlayerCount(QString(QString::number(game.personCount)));
        int temp = game.dealerCount;
        temp = temp - dealer.cardArray.at(0).value;
        emit updateDealerCount(QString(QString::number(temp)));
    }
    emit enableDealCards(false);
    emit addCardToPlayerHand(playerOne.cardArray.at(0), false);
    emit addCardToDealerHand(dealer.cardArray.at(0), true);
    emit addCardToPlayerHand(playerOne.cardArray.at(1), false);
    emit addCardToDealerHand(dealer.cardArray.at(1), false);
    int checkBlackJack = game.checkBlackJack(playerOne,dealer);
    if(checkBlackJack == 1){
        emit blackJackButtons(false);
        emit displayEndGameMessage(true);
        emit changeEndGameMessage("You won");
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        std::cout << "player BlackJack" << std::endl;
    }
    else if(checkBlackJack == 2){
        emit blackJackButtons(false);
        emit displayEndGameMessage(true);
        emit changeEndGameMessage("You lost");
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        std::cout << "dealer BlackJack" << std::endl;
    }
    else if(checkBlackJack == 3){
        emit blackJackButtons(false);
        emit displayEndGameMessage(true);
        emit changeEndGameMessage("You tied");
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        // emit tie
    }
    else if(playerOne.cardArray.at(0).value == playerOne.cardArray.at(1).value && isRigged == false && playerOne.cardArray.size() == 2){ emit enableSplit(true); }
    else{emit enableSplit(false);}
}
void Model::splitSlot(){
    game.split(playerOne);
    std::tuple<bool, int> stateCheck = game.checkState(playerOne); // checks if the split hand is a 21
    if(get<1>(stateCheck) == 4){
        standSlot();
    }
    emit updatePlayerCount(QString(QString::number(game.personCount)));
    emit addCardToPlayerHand(playerOne.splitArray.back(), true);
    emit addCardToPlayerHand(playerOne.cardArray.back(), false);
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
        std::vector<QString> tokens =levelScript.tokenize(scriptOutputDetails);
        if(tokens[tokens.size()-2] == "lock"){
            emit sendLock(tokens[tokens.size()-1]);
            scriptOutputDetails.clear();
            for(int i =0; i<tokens.size()-2; i++)
            {
                scriptOutputDetails.append(tokens[i]);
                scriptOutputDetails.append(" ");
            }
        }
        //emit message to the view (it will show the popup)
        emit sendMessage(scriptOutputDetails);
    }
    else if (messageType == "deal")
    {
        std::vector<QString> tokens = levelScript.tokenize(scriptOutputDetails);
        if(tokens[1] == "dealer")
        {
            //deal the next card in the card draw queue to dealer primary hand
            game.hit(dealer, riggedCards[nextCard]);
        }
        else if(tokens[2]== "hand")
        {
            //deal the next card in the card draw queue to the players primary hand
            game.hit(playerOne, riggedCards[nextCard]);

        }
        else if(tokens[2]== "split")
        {
            //deal the next card in the card draw queue to the players split hand
            game.hit(playerOne, riggedCards[nextCard]);
        }
        nextCard++;
    }
    else if (messageType == "reveal")
    {
        //reveal hole
        emit revealHole(dealer.cardArray[0].image);
    }
    else if (messageType == "addCards")
    {
        std::vector<QString> tokens = levelScript.tokenize(scriptOutputDetails);
        for(QString token:tokens)
        {
            Suit suit;
            if(token.at(0) == 'h')
            {
                suit = heart;
            }
            else if(token.at(0) == 'd')
            {
                suit = diamond;
            }
            else if(token.at(0) == 'c')
            {
                suit = club;
            }
            else
            {
                suit = spade;
            }
            QString rank = token.removeFirst();

            riggedCards.push_back(deck.draw(stringToRank(rank), suit));
        }

    }
    else if (messageType == "finish")
    {
        riggedCards.clear();
        //emit end level signal
        emit endLevel(false);
    }
    else if (messageType == "shuffle")
    {
        //Pull all cards back into the deck and shuffle
        emit sendClear();
        initialDeal();
    }
    else if (messageType == "lock")
    {
        emit sendLock(scriptOutputDetails);
    }
    else
    {
        //This is an error state. We should be throwing here, or something
        emit endLevel(true);
    }
}

Rank Model::stringToRank(QString text)
{
    if(text == "Ace")
        return ace;
    if(text == "Two")
        return two;
    if(text == "Three")
        return three;
    if(text == "Four")
        return four;
    if(text == "Five")
        return five;
    if(text == "Six")
        return six;
    if(text == "Seven")
        return seven;
    if(text == "Eight")
        return eight;
    if(text == "Nine")
        return nine;
    if(text == "Ten")
        return ten;
    if(text == "Jack")
        return jack;
    if(text == "Queen")
        return queen;
    if(text == "King"){
        return king;
    }
    else{
        text = text.removeLast();
        return stringToRank(text);
    }
}
