#include "WordTree.hpp"

#include <queue>

TreeNode::TreeNode() :
    m_endOfWord(false)
{
    for (unsigned i = 0; i < m_children.size(); i++)
    {
        m_children[i] = nullptr;
    }
}

WordTree::WordTree() :
    m_size(0),
    m_root(std::make_shared<TreeNode>())
{
}

void WordTree::add(std::string word)
{
    if (word.size() < 1)
    {
        return;
    }
    int asciiDiff = 97;
    std::shared_ptr<TreeNode> root = m_root;
    std::shared_ptr<TreeNode> node = m_root;
    for (unsigned c = 0; c < word.size(); c++)
    {
        if (std::isalpha(word[c]))
        {
            int charVal = std::tolower(word[c]) - asciiDiff;
            if (node->m_children[charVal] == nullptr)
            {
                std::shared_ptr<TreeNode> t = std::make_shared<TreeNode>();
                node->m_children[charVal] = t;
            }
            node = node->m_children[charVal];
        }
        else
        {
            return;
        }
    }
    if (node->m_endOfWord == false)
    {
        node->m_endOfWord = true;
        m_size += 1;
    }
    m_root = root;
}

bool WordTree::find(std::string word)
{
    if (word.size() < 1 || m_size < 1)
    {
        return false;
    }

    int asciiDiff = 97;

    std::shared_ptr<TreeNode> node = m_root;

    for (unsigned c = 0; c < word.size(); c++)
    {
        if (std::isalpha(word[c]))
        {
            int charVal = std::tolower(word[c]) - asciiDiff;
            if (node->m_children[charVal] == nullptr)
            {
                return false;
            }
            node = node->m_children[charVal];
        }
        else
        {
            return false;
        }
    }
    if (node->m_endOfWord)
    {
        return true;
    }
    return false;
}

std::size_t WordTree::size()
{
    return m_size;
}

std::vector<std::string> WordTree::predict(std::string partial, std::uint8_t howMany)
{
    std::vector<std::string> solution;
    if (partial.size() < 1)
    {
        return solution;
    }

    int asciiDiff = 97;
    // make sure partial is valid
    std::shared_ptr<TreeNode> node = m_root;
    for (unsigned c = 0; c < partial.size(); c++)
    {
        if (std::isalpha(partial[c]))
        {
            int charVal = std::tolower(partial[c]) - asciiDiff;
            if (node->m_children[charVal] == nullptr)
            {
                return solution;
            }
            node = node->m_children[charVal];
        }
        else
        {
            return solution;
        }
    }

    std::queue<std::string> queue;

    queue.push(partial);

    while (!queue.empty())
    {
        std::string word = queue.front();
        queue.pop();
        // check if current word on queue is a word.
        if (this->find(word) && word != partial)
        {
            solution.push_back(word);
        }
        // check if the size of solution is met.
        if (solution.size() == howMany)
        {
            return solution;
        }
        // find the node equivalent to the current word
        node = m_root;
        for (unsigned c = 0; c < word.size(); c++)
        {
            if (std::isalpha(word[c]))
            {
                int charVal = std::tolower(word[c]) - asciiDiff;
                if (node->m_children[charVal] == nullptr)
                {
                    return solution;
                }
                node = node->m_children[charVal];
            }
            else
            {
                return solution;
            }
        }
        // find all children at node and add them to the queue
        for (unsigned ch = 0; ch < node->m_children.size(); ch++)
        {
            if (node->m_children[ch] != nullptr)
            {
                std::string temp = word;
                char chr = static_cast<char>(ch + asciiDiff);
                temp += chr;
                queue.push(temp);
            }
        }
    }
    return solution;
}