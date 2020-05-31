#include "WordTree.hpp"
#include "rlutil.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

std::shared_ptr<WordTree> readDictionary(std::string filename)
{
    auto wordTree = std::make_shared<WordTree>();
    std::ifstream inFile = std::ifstream(filename, std::ios::in);

    while (!inFile.eof())
    {
        std::string word;
        std::getline(inFile, word);
        // Need to consume the carriage return character for some systems, if it exists
        if (!word.empty() && word[word.size() - 1] == '\r')
        {
            word.erase(word.end() - 1);
        }
        // Keep only if everything is an alphabetic character -- Have to send isalpha an unsigned char or
        // it will throw exception on negative values; e.g., characters with accent marks.
        if (std::all_of(word.begin(), word.end(), [](unsigned char c) { return std::isalpha(c); }))
        {
            std::transform(word.begin(), word.end(), word.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });
            wordTree->add(word);
        }
    }

    return wordTree;
}

int main()
{
    auto wordTree = readDictionary("dictionary.txt");

    int key = rlutil::getkey();
    std::string word = "";
    char c = static_cast<char>(key);
    word += c;

    while (key)
    {
        // clear the console
        rlutil::cls();

        // user input info
        for (unsigned i = 0; i < word.size(); i++)
        {
            if (word[i] == rlutil::KEY_BACKSPACE || word[i] == rlutil::KEY_DELETE)
            {
                word.clear();
                break;
            }
            rlutil::locate(i + 1, 1);
            rlutil::setChar(word[i]);
        }

        // predict display
        std::string predict = "--- prediction ---";
        for (unsigned i = 0; i < predict.size(); i++)
        {
            rlutil::locate(i + 1, 3);
            rlutil::setChar(predict[i]);
        }

        // prediction calculation and display
        rlutil::locate(1, 5);
        int howMany = rlutil::trows() - 4;
        auto words = wordTree->predict(word, howMany);
        for (unsigned i = 0; i < words.size(); i++)
        {
            std::string tmp = words[i];
            for (unsigned j = 0; j < tmp.size(); j++)
            {
                rlutil::locate(j + 1, 4 + i);
                rlutil::setChar(tmp[j]);
            }
        }

        // wait on next character...
        rlutil::locate(word.size() + 1, 1);
        key = rlutil::getkey();
        if ((key == rlutil::KEY_DELETE || key == rlutil::KEY_BACKSPACE) && word.size() != 0)
        {
            word.pop_back();
        }
        else
        {
            word += static_cast<char>(key);
        }
    }

    return 0;
}