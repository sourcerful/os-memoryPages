#include <iostream>
#include "ProcessProperties.hpp"

int main()
{
    std::string reference_string = "001112222333344333555333366333344333555333366778888";
    ProcessProperties process;

    process.FIFO(reference_string);

    return 0;
}