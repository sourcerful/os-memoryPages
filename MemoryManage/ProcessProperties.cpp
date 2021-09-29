#include "ProcessProperties.hpp"
#include <iostream>
#include <stack>
#include <queue>
#include <cstdio>

Swap* swaps;

ProcessProperties::ProcessProperties()
{
    _size = 3;
    _workingSet = 0;
    _focus = 0;
    _iterations = 0;
    _pageFault = 0;
}
bool ProcessProperties::Insert(int page_val)
{
    bool inserted = _table.insert(page_val).second; //inserted or not
    if (inserted)
        _pageFault++;
    _iterations++;
    return inserted;
}
void ProcessProperties::LIFO(std::string referenceString)
{
    std::stack<int> tableStack;
    swaps = new Swap;

    removeDuplicates(referenceString);

    for (int i = 0; i < referenceString.size(); i++)
    {
        _focus = referenceString[i] - '0';

        if (_workingSet != _size)
        {
            if (this->Insert(_focus))
            {
                tableStack.push(_focus);
                swaps->swappedIn[_focus]++;
                _workingSet++;
            }
        }
        else
        {
            if (this->Insert(_focus))
            {
                _table.erase(tableStack.top());
                swaps->swappedOut[tableStack.top()]++;
                tableStack.pop();
                tableStack.push(_focus);
                swaps->swappedIn[_focus]++;
            }
        }

        this->toString();
    }

    this->showInfo();
    delete swaps;
}
void ProcessProperties::FIFO(std::string referenceString)
{

    std::queue<int> tableQueue;
    swaps = new Swap;

    removeDuplicates(referenceString);

    for (int i = 0; i < referenceString.size(); i++)
    {
        _focus = referenceString[i] - '0';

        if (_workingSet != _size)
        {
            if (this->Insert(_focus))
            {
                tableQueue.push(_focus);
                swaps->swappedIn[_focus]++;
                _workingSet++;
            }
        }
        else
        {
            if (this->Insert(_focus))
            {
                _table.erase(tableQueue.front());
                swaps->swappedOut[tableQueue.front()]++;
                tableQueue.pop();
                tableQueue.push(_focus);
                swaps->swappedIn[_focus]++;
            }
        }
        this->toString();
    }

    this->showInfo();
    delete swaps;
        
}
void ProcessProperties::showInfo()
{
    std::cout << "Page faults: " << _pageFault << std::endl;
    std::cout << "miss ratio: ";
    printf("%.2f\n", (double)_pageFault / _iterations);
    std::map<int, int>::iterator iter;
    std::cout << "number of swap in:\n";
    for (iter = swaps->swappedIn.begin(); iter != swaps->swappedIn.end(); iter++)
    {
        std::cout << (*iter).first << ": " << (*iter).second << std::endl;
    }

    std::cout << "number of swap out:\n";

    for (iter = swaps->swappedOut.begin(); iter != swaps->swappedOut.end(); iter++)
    {
        std::cout << (*iter).first << ": " << (*iter).second << std::endl;
    }
}
void removeDuplicates(std::string& s)
{
    size_t n = s.length();
    std::string str = "";
    // We don't need to do anything for
    // empty string.
    if (n == 0)
        return;

    // Traversing string
    for (int i = 0; i < n - 1; i++) 
    {
        //checking if s[i] is not same as s[i+1] then add it into str
        if (s[i] != s[i + 1]) 
        {
            str += s[i];
        }
    }
    //Since the last character will not be inserted in the loop we add it at the end
    str.push_back(s[n - 1]);
    s = str;
}
void ProcessProperties::toString()
{
    std::set<int>::iterator iter = _table.begin();
    std::cout << "|-----|  iterations: " << _iterations << std::endl;
    for (int i = 0; i < _size; i++)
    {
        if (i < _workingSet)
        {
            if(*iter == _focus)
                std::cout << "| " << '[' << *iter << ']' << " |" << std::endl;
            else
                std::cout << "|  " << *iter << "  |" << std::endl;
            iter++;
        }
        else
            std::cout << "|  " << '-' << "  |" << std::endl;
        std::cout << "|-----|" << std::endl;
    }
    std::cout << std::endl;
}
ProcessProperties::~ProcessProperties()
{
    ;
}
