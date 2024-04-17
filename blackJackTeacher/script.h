#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>
#include <fstream>
#include <vector>
#include <QObject>
class Script
{
public:
    Script(std::string fileName);
    ~Script();
    ///
    /// \brief nextCommand Parses the next line of the script and returns a string representing the type of command
    /// \param outputDetails a string that holds the details of the output. Will be replaced with a blank string, a string with a display message or a string that can be parsed into pieces to deal a card
    /// \return a string representing the next command's type
    ///
    std::string nextCommand(std::string* outputDetails);
private:
    ///
    /// \brief script a file stream reading the script
    ///
    std::fstream script;
    ///
    /// \brief tokenize breaks a given string into a vector of tokens seperated by a ' ' as a delimiter
    /// \param providedString a line to be tokenized
    /// \return a vector inn which each entry is a word in the original string.
    ///
    std::vector<std::string> tokenize(std::string providedString);
};

#endif // SCRIPT_H