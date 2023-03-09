#include <iostream>
#include <fstream>
#include <bitset>
#include <memory>
#include <vector>

// Input header for the input structure from linux
#include <linux/input.h>

#include "keylogger.hpp"

struct input
{
    uint64_t seconds, microseconds;
    uint16_t type, code;
    uint32_t value;
    void print();
};

void input::print()
{

    std::cout << "At time " << seconds << "." << microseconds << ":\n" <<
                 "KeyCode: " << code << "\n" <<
                 "Type: " << type << "\n" <<
                 "Value: " << value << "\n" <<
                 "----------------------------\n";

}


std::vector<std::shared_ptr<std::bitset<8>>> slice(std::vector<std::shared_ptr<std::bitset<8>>> &fullVect, int start, int end)
{
    auto first = fullVect.begin() + start;
    auto last = fullVect.begin() + end;

    std::vector<std::shared_ptr<std::bitset<8>>> output(first,last);

    return output;

}

std::vector<std::shared_ptr<std::bitset<8>>> littleEndianToBigEndian(std::vector<std::shared_ptr<std::bitset<8>>> &littleEnd)
{
    std::vector<std::shared_ptr<std::bitset<8>>> out;

    for (size_t i = 0; i < littleEnd.size(); ++i)
    {
        out.push_back(littleEnd.at(littleEnd.size()-1-i));
    }
    return out;

}

void print64(std::vector<std::shared_ptr<std::bitset<8>>> &t)
{
    std::cout << std::bitset<64>((t.at(0))->to_string()+
                                (t.at(1))->to_string()+
                                (t.at(2))->to_string()+
                                (t.at(3))->to_string()+
                                (t.at(4))->to_string()+
                                (t.at(5))->to_string()+
                                (t.at(6))->to_string()+
                                (t.at(7))->to_string()).to_ullong() << std::endl;
}

void print32(std::vector<std::shared_ptr<std::bitset<8>>> &t)
{
    std::cout << std::bitset<64>((t.at(0))->to_string()+
                                (t.at(1))->to_string()+
                                (t.at(2))->to_string()+
                                (t.at(3))->to_string()
                                ).to_ullong() << std::endl;
}

void print16(std::vector<std::shared_ptr<std::bitset<8>>> &t)
{
    std::cout << std::bitset<64>((t.at(0))->to_string()+
                                (t.at(1))->to_string()).to_ullong() << std::endl;
}

uint64_t merge64(std::vector<std::shared_ptr<std::bitset<8>>> &t)
{
    return std::bitset<64>((t.at(0))->to_string()+
                                (t.at(1))->to_string()+
                                (t.at(2))->to_string()+
                                (t.at(3))->to_string()+
                                (t.at(4))->to_string()+
                                (t.at(5))->to_string()+
                                (t.at(6))->to_string()+
                                (t.at(7))->to_string()).to_ullong();
}

uint32_t merge32(std::vector<std::shared_ptr<std::bitset<8>>> &t)
{
    return std::bitset<64>((t.at(0))->to_string()+
                                (t.at(1))->to_string()+
                                (t.at(2))->to_string()+
                                (t.at(3))->to_string()
                                ).to_ullong();
}

uint16_t merge16(std::vector<std::shared_ptr<std::bitset<8>>> &t)
{
    return std::bitset<64>((t.at(0))->to_string()+
                                (t.at(1))->to_string()).to_ullong();
}

input parse(std::vector<std::shared_ptr<std::bitset<8>>> &incomingData)
{
    input answer;
    std::vector<std::shared_ptr<std::bitset<8>>> temp = slice(incomingData,0,8);
    std::vector<std::shared_ptr<std::bitset<8>>> tempSeconds = littleEndianToBigEndian(temp);

    answer.seconds = merge64(tempSeconds);

    std::vector<std::shared_ptr<std::bitset<8>>> temp1 = slice(incomingData,8,16);
    std::vector<std::shared_ptr<std::bitset<8>>> tempMicroSeconds = littleEndianToBigEndian(temp1);

    answer.microseconds = merge64(tempMicroSeconds);

    std::vector<std::shared_ptr<std::bitset<8>>> temp2 = slice(incomingData,16,18);
    std::vector<std::shared_ptr<std::bitset<8>>> type = littleEndianToBigEndian(temp2);

    answer.type = merge16(type);

    std::vector<std::shared_ptr<std::bitset<8>>> temp3 = slice(incomingData,18,20);
    std::vector<std::shared_ptr<std::bitset<8>>> code = littleEndianToBigEndian(temp3);

    answer.code = merge16(code);

    std::vector<std::shared_ptr<std::bitset<8>>> temp4 = slice(incomingData,20,24);
    std::vector<std::shared_ptr<std::bitset<8>>> value = littleEndianToBigEndian(temp4);

    answer.value = merge32(value);

    return answer;

}

int main(int argc, char const *argv[])
{

    std::ifstream fd("/workspaces/input/event2", std::ios::in | std::ios::binary);

    // Check if the file was opened correctly or not.
    if (!fd)
    {
        std::cout << "The file was not opened" << std::endl;
        exit(1);
    }

    char data[24];
    while (true)
    {
    try
    {
        if (fd.is_open() && !fd.eof())
        {
            fd.read(data, 24);
        } else
        {
            continue;
        }

        std::vector<std::shared_ptr<std::bitset<8>>> dataIn;

        // Parsing the input data into the bits that they are. The VM is in littel endian, so it has to be swapped later.
        for (size_t i = 0; i < 24; ++i)
        {
            std::shared_ptr<std::bitset<8>> temp = std::make_shared<std::bitset<8>>(std::bitset<8>(data[i]));
            dataIn.push_back(temp);
            // std::cout << *temp << std::endl;
        }
        input tempInput = parse(dataIn);
        if (tempInput.value == 1)
        {
            tempInput.print();
        }
        

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    }
    return 0;
}
