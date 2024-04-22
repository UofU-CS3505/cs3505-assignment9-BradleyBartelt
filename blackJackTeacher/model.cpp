#include "model.h"
#include "game.h"
#include <QTimer>
#include <iostream>
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
    if(game.stand(playerOne) == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        emit disableButtons(false);
        if(game.dealerCount > 16){
            // end game stuff
            endGame();
            return;
        }
        int waitTime = 1000;
        int count = 1;
        while(true){
            waitTime = abs(count * waitTime);
            if(isRigged){
                // pass in card you want to rig
                get<0>(game.hit(dealer, riggedCards[nextCard]));
                nextCard++;
                if(game.dealerCount > 16){
                    QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
                    endGame();
                    break;
                }
            }
            else{
                get<0>(game.hit(dealer));
                if(game.dealerCount > 16){
                    // end game stuff
                    QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
                    endGame();
                    break;
                }
            }
            QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.cardArray.at(count + 1),false);});
            QTimer::singleShot(waitTime, this,[=]{ emit updateDealerCount(QString(QString::number(game.dealerCount)));});
            count++;
        }
    }
}
void Model::endGame(){
    QTimer::singleShot(3000, this,[=]{ emit enableDealCards(true);});
    QTimer::singleShot(3000, this,[=]{ emit enableMainMenu(true);});
    if(game.endResult() == "tie"){
        // emit main hand tie
    }
    else if(game.endResult() == "win"){
        std::cout << "end Win" << std::endl;

        emit winMessage(true);
    }
    else if(game.endResult() == "loss"){
        std::cout << "end loss" << std::endl;

        emit lossMessage(true);
    }
    else if(game.endResult() == "splitTie"){
        // emit split hand tie
    }
    else if(game.endResult() == "splitWin"){
        // emit split hand win
    }
    else{
        // emit split hand loss
    }
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
        emit updatePlayerCount(QString(QString::number(game.personCount)));
    }
    if(get<0>(gameTuple) && get<1>(gameTuple) != 3)
        emit addCardToPlayerHand(playerOne.cardArray.back());
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 3){ //player gets a 21
        emit addCardToPlayerHand(playerOne.cardArray.back());
        emit disableButtons(false);
        standSlot();
    }
    else{
        emit addCardToPlayerHand(playerOne.cardArray.back());
        emit lossMessage(true);
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
    emit lossMessage(false);
    emit winMessage(false);
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
    emit addCardToPlayerHand(playerOne.cardArray.at(0));
    emit addCardToDealerHand(dealer.cardArray.at(0), true);
    emit addCardToPlayerHand(playerOne.cardArray.at(1));
    emit addCardToDealerHand(dealer.cardArray.at(1), false);
    int checkBlackJack = game.checkBlackJack(playerOne,dealer);
    if(checkBlackJack == 1){
        emit disableButtons(false);
        emit winMessage(true);
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        std::cout << "player BlackJack" << std::endl;
        enableGameRestartButtons();
        emit winMessage(true);
    }
    else if(checkBlackJack == 2){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit lossMessage(true);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        emit lossMessage(true);
        enableGameRestartButtons();
        std::cout << "dealer BlackJack" << std::endl;
    }
    else if(checkBlackJack == 3){
        emit disableButtons(false);
        emit SendCardImage(dealer.cardArray.begin()->image);
        emit updateDealerCount(QString(QString::number(game.dealerCount)));
        enableGameRestartButtons();
        // emit tie
    }
    if(playerOne.cardArray.at(0).rank == playerOne.cardArray.at(1).rank && isRigged == false){ emit enableSplit(true); }
    else{emit enableSplit(false);}
}
void Model::enableGameRestartButtons(){
    emit enableDealCards(true);
    emit enableMainMenu(true);
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
