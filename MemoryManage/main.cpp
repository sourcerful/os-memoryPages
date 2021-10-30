#include <iostream>
#include "ProcessProperties.hpp"

int main()
{
    using std::string;

    string reference_string = "7 0 1 2 0 3 0 4 2 3 0 3 2 1 2";

    ProcessProperties process;
        
    process.LFU(reference_string);
   

    return 0;
}