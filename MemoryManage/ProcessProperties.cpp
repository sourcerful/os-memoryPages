#include "ProcessProperties.hpp"
#include <iostream>
#include <stack>
#include <algorithm>
#include <cstdio>
#include <limits>

Swap* swaps;
bool maga = false;
int* globalPandemic;

ProcessProperties::ProcessProperties()
{
    _size = 3;
    _workingSet = 0;
    _focus = 0;
    _iterations = 0;
    _pageFault = 0;
}


bool ProcessProperties::Insert(int page_val, int *swapped = nullptr)
{
    bool inserted = _tableCheck.insert(page_val).second; //inserted or not 

    if (inserted)
    {
        maga = false;
        _pageFault++;
        if (swapped)
        {
            _tableCheck.erase(*swapped);
            auto iter = find(_table.begin(), _table.end(), *swapped);
            *iter = page_val;
        }
        else
        {
            _table.emplace_back(page_val);
        }
    }
    else
        maga = true;

    _iterations++;
    return inserted;
}
void ProcessProperties::LIFO(std::string referenceString)
{
    std::stack<int> tableStack;
    std::vector<std::string> pages;
    char delimiter = ' ';
    swaps = new Swap;
    _tableCheck.clear();
    _table.clear();

    simple_tokenizer(referenceString, pages, delimiter);

    for (int i = 0; i < pages.size(); i++)
    {
        _focus = std::stoi(pages[i]);

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
            if (this->Insert(_focus, &(tableStack.top())))
            {
                swaps->swappedOut[tableStack.top()]++;
                tableStack.pop();
                tableStack.push(_focus);
                swaps->swappedIn[_focus]++;
            }
        }
        std::cout << _focus << std::endl;
        this->toString();
    }

    this->showInfo();
    delete swaps;
}
void ProcessProperties::LRU(std::string referenceString)
{
    std::queue<int> last_elements;
    std::vector<std::string> pages;
    char delimiter = ' ';
    swaps = new Swap;
    _tableCheck.clear();
    _table.clear();

    simple_tokenizer(referenceString, pages, delimiter);

    for (int i = 0; i < pages.size(); i++)
    {
        _focus = std::stoi(pages[i]);

        if (_workingSet != _size)
        {
            if (this->Insert(_focus))
            {
                last_elements.push(_focus);
                swaps->swappedIn[_focus]++;
                _workingSet++;
            }
        }
        else
        {
            if (this->Insert(_focus, &(last_elements.front())))
            {
                swaps->swappedOut[last_elements.front()]++;
                last_elements.pop();
                last_elements.push(_focus);
                swaps->swappedIn[_focus]++;
            }
        }

        if (maga)
        {
            for (int i = 0; i < last_elements.size(); i++)
            {
                last_elements.push(last_elements.front());
                last_elements.pop();
                if (last_elements.front() == _focus)
                    break;
            }
            last_elements.pop();
            last_elements.push(_focus);
        }

        std::cout << _focus << std::endl;
        this->toString();
    }

    this->showInfo();
    delete swaps;
}
void ProcessProperties::FIFO(std::string referenceString)
{
    std::queue<int> tableQueue;
    std::vector<std::string> pages;
    char delimiter = ' ';
    swaps = new Swap;
    _tableCheck.clear();
    _table.clear();

    simple_tokenizer(referenceString, pages, delimiter);

    for (int i = 0; i < pages.size(); i++)
    {
        _focus = std::stoi(pages[i]);

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
            if (this->Insert(_focus, &(tableQueue.front())))
            {
                swaps->swappedOut[tableQueue.front()]++;
                tableQueue.pop();
                tableQueue.push(_focus);
                swaps->swappedIn[_focus]++;
            }
        }
        std::cout << _focus << std::endl;
        this->toString();
    }

    this->showInfo();
    delete swaps;
        
}

void ProcessProperties::LFU(std::string referenceString)
{
    std::queue<int> tableQueue;
    std::vector<std::string> pages;
    std::map<int, int> usage;
    int zibi;
    int* swappedOut_ptr = nullptr;
    char delimiter = ' ';
    swaps = new Swap;
    _tableCheck.clear();
    _table.clear();
    simple_tokenizer(referenceString, pages, delimiter);


    for (int i = 0; i < pages.size(); i++)
    {
        _focus = std::stoi(pages[i]);

        if (_workingSet != _size)
        {
            if (this->Insert(_focus))
            {
                if(!isInsideQueue(tableQueue, _focus))
                    tableQueue.push(_focus);
                swaps->swappedIn[_focus]++;
                _workingSet++;
            }
            usage[_focus]++;
        }
        else
        {
            zibi = findMinPage(usage, tableQueue);
            swappedOut_ptr = &zibi;

            if (this->Insert(_focus, swappedOut_ptr))
            {               
                if (!maga)
                {
                    usage[zibi] = 0;    
                    tableQueue.push(_focus);
                    removeFromQueue(tableQueue, zibi);
                    swaps->swappedOut[zibi]++;
                }
                swaps->swappedIn[_focus]++;
            }
            usage[_focus]++;
        }
        std::cout << _focus << std::endl;
        this->toString();
    }

    this->showInfo();
    delete swaps;

}
int findMinPage(std::map<int, int> &usageMap, std::queue<int> &pageQueue)
{
    //(*(usageMap.begin())).second
    int minVal = INT_MAX;
    int key = -1;
    int qHead = pageQueue.front();
    bool found = false;

    for (auto i = usageMap.begin(); i != usageMap.end(); i++)
    {
        if(i->second < minVal && i->second != 0)
            minVal = i->second;
    }

    if (usageMap[pageQueue.front()] == minVal)
    {
        key = pageQueue.front();
        found = true;
    }
    else
    {
        do
        {
            if (usageMap[pageQueue.front()] == minVal && key == -1)
            {
                key = pageQueue.front();
                found = true;
            }
            pageQueue.push(pageQueue.front());
            pageQueue.pop();
        } while (pageQueue.front() != qHead);
    }
    
    return key;

}
bool isInsideQueue(std::queue<int> &tableQueue, int val)
{
    if (tableQueue.empty())
        return false;
    int qHead = tableQueue.front();
    int temp;
    bool isInside = false;
    do
    {
        temp = tableQueue.front();
        tableQueue.pop();    
        if (temp == val)       
          isInside = true;
                  
        tableQueue.push(temp);
    } while (tableQueue.front() != qHead);
    
    return isInside;
}

void removeFromQueue(std::queue<int>& tableQueue, int val)
{
    if (tableQueue.empty())
        return;
    int qHead = tableQueue.front();
    int temp;
    if(tableQueue.front()==val)
        tableQueue.pop();
    else
    {
        do
        {
            temp = tableQueue.front();
            tableQueue.pop();
            if (temp != val)
                tableQueue.push(temp);

        } while (tableQueue.front() != qHead);
    }
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
        std::cout << (*iter).first << ": " << (*iter).second << std::endl; //or iter->first
    }

    std::cout << "number of swap out:\n";

    for (iter = swaps->swappedOut.begin(); iter != swaps->swappedOut.end(); iter++)
    {
        std::cout << (*iter).first << ": " << (*iter).second << std::endl;
    }
}
void removeDuplicates(std::string& s)
{
    using std::string;
    int new_size = 0;
    string::iterator iter;

    iter = std::unique(s.begin(), s.end());

    for (auto i = s.begin(); i < iter; i++)
        new_size++;

    s.resize(new_size);
    std::cout << s << std::endl;
}
void ProcessProperties::toString()
{
    std::vector<int>::iterator iter = _table.begin();
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
void simple_tokenizer(std::string s, std::vector<std::string> &vec, char delimiter)
{
    int currIndex = 0, i = 0;
    int startIndex = 0, endIndex = 0;

    while (i <= s.length())
    {
        if (s[i] == delimiter || i == s.length())
        {
            endIndex = i;
            std::string subStr = "";
            subStr.append(s, startIndex, endIndex - startIndex);
            vec.emplace_back(subStr);
            currIndex += 1;
            startIndex = endIndex + 1;
        }
        i++;
    }

    for (int i = 0; i < vec.size(); i++)
        std::cout << vec.at(i) << " ";
}
ProcessProperties::~ProcessProperties()
{
    ;
}
