#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>

#include <vector>
#include <QObject>
#include <QFile>
class Script
{
public:
    Script();
    ~Script();
    ///
    /// \brief nextCommand Parses the next line of the script and returns a string representing the type of command
    /// \param outputDetails a string that holds the details of the output. Will be replaced with a blank string, a string with a display message or a string that can be parsed into pieces to deal a card
    /// \return a string representing the next command's type
    ///
    QString nextCommand(QString* outputDetails);

    Script& operator=(Script& other);

    void setScript(std::string filename);

private:
    ///
    /// \brief script a file stream reading the script
    ///
    std::vector<QString> lines;

    ///
    /// \brief index
    ///
    int lineNum = 0;
    ///
    /// \brief tokenize breaks a given string into a vector of tokens seperated by a ' ' as a delimiter
    /// \param providedString a line to be tokenized
    /// \return a vector inn which each entry is a word in the original string.
    ///
    std::vector<QString> tokenize(QString providedString);
};

#endif // SCRIPT_H
