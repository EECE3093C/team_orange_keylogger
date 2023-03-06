#include <iostream>
#include <fstream>

#include "keylogger.hpp"

int main(int argc, char const *argv[])
{
    std::cout << "Hello World" << std::endl;

    std::ifstream fd("/workspaces/input/event2");

    // Check if the file was opened correctly or not.
    if (!fd)
    {
        std::cout << "The file was not opened" << std::endl;
        exit(1);
    }

    std::string data;
    while (std::getline(fd, data))
    {
        std::cout << data << std::endl;
    }
    

    return 0;
}
