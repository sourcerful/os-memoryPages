#ifndef TABLE_F
#define TABLE_F

#include <set>
#include <string>
#include <map>

class ProcessProperties
{
private:
    std::set<int> _table;
    int _size;
    int _workingSet; //how many pages currently
    int _iterations;
    int _focus;
    int _pageFault; 
public:
    ProcessProperties();
    bool Insert(const int);
    void LIFO(std::string);
    void FIFO(std::string);
    /*
    void LRU();
    void LFU();
    void MLRU();
    */
    void showInfo();
    void toString();
    ~ProcessProperties();
};

typedef struct Swap
{
    std::map<int, int> swappedIn;
    std::map<int, int> swappedOut;
}Swap;

void removeDuplicates(std::string& s);

#endif
