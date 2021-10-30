#ifndef TABLE_F
#define TABLE_F

#include <set>
#include <string>
#include <vector>
#include <map>
#include <queue>

class ProcessProperties
{
private:
    std::set<int> _tableCheck;
    std::vector<int> _table;
    int _size;
    int _workingSet; //how many pages currently
    int _iterations;
    int _focus;
    int _pageFault; 
public:
    ProcessProperties();
    bool Insert(const int, int*);
    void LIFO(std::string);
    void FIFO(std::string);
    void LRU(std::string);
    void LFU(std::string);
    //void MLRU();
    void showInfo();
    void toString();
    ~ProcessProperties();
};

typedef struct Swap
{
    std::map<int, int> swappedIn;
    std::map<int, int> swappedOut;
}Swap;

bool isInsideQueue(std::queue<int>& tableQueue, int val);
void removeFromQueue(std::queue<int>& tableQueue, int val);
int findMinPage(std::map<int, int>&, std::queue<int>&);
void removeDuplicates(std::string& s);
void simple_tokenizer(std::string s, std::vector<std::string>& vec, char delimiter);

#endif
