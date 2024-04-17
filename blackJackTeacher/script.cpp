#include "script.h"
#include <vector>
#include <sstream>

Script::Script(std::string fileName)
{
    //Try to open the provided file name
    script.open(fileName, std::ios::in);
    //Check to see if it opened. If not throw error
    if(!script.is_open())
    {
        //Get upset and throw
    }
}
std::string Script::nextCommand(std::string* outputDetails)
{
    //Read the next line from the file
    std::string nextLine;
    if(getline(script, nextLine))
    {
        // By the contract we made for the scripts, starting with a forward slash represents a message to display to the view
        if(nextLine.at(0) == '/')
        {
            *outputDetails = nextLine.substr(2);
            return "message";
        }
        // Otherwise, all other commands should start with an asterisk.
        else if(nextLine.at(0) == '*')
        {
            nextLine = nextLine.substr(2);
            std::vector<std::string> tokens = tokenize(nextLine);
            if(tokens[0] == "deal")
            {
                //Deal card tokens[3] to player tokens[1], hand tokens[2]
                *outputDetails = tokens[1] + tokens[2] + tokens[3];
                return "deal";
            }
            if(tokens[0] == "shuffle")
            {
                *outputDetails = "";
                //Pull all cards back into the deck and prepare for the next deal
                return "shuffle";
            }
            if(tokens[0] == "reveal")
            {
                *outputDetails = "";
                //flip the display of the dealers hole card, so the user can see
                return "reveal";
            }
        }
        else
        {
            //We should be throwing at this point. If we are not starting with a '/' or '*'
            //We are not following the contract. If it is empty, we should close this script otherwise they tried to open something that they shouldn't
            if(nextLine.empty())
            {
                *outputDetails = "";
                script.close();
                return "finish";
            }
            return "error";
        }

    }
    else
    {
        *outputDetails = "";
        //We made it to the end of the file. End the Module, close the file, and return to home screen
        script.close();
        return "finish";
    }
    return "error";
}

std::vector<std::string> Script::tokenize(std::string providedString)
{
    std::vector<std::string> tokens;
    std::stringstream stream(providedString);
    std::string token;
    while(getline(stream, token, ' '))
    {
        tokens.push_back(token);
    }
    return tokens;
}
