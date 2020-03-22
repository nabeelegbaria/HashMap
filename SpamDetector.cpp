#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "HashMap.hpp"

/**
 * This function exit the program and prints invalid input.
 */
void error()
{
    std::cout << "Invalid input" << std::endl;
    exit(EXIT_FAILURE);
}
/**
 * This function checks if the string content is number.
 * @param line
 * @return true if it is number, false otherwise.
 */
bool isparam(const std::string &line)
{
    char *p;
    strtol(line.c_str(), &p , 10);
    return *p == 0;
}
/**
 * This function checks if the file is empty.
 * @param argv
 * @return true if it is empty , false otherwise.
 */
bool isempty(char *argv[])
{
    std::ifstream file;
    file.open(argv[1], std::fstream::in);
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        int size = file.tellg();
        if (size != 0)
        {
            return false;
        }
    }
    return true;
}

/**
 * This function receives a string and make it lower case .
 * @param str
 */
void lowercase(std::string *str)
{
    std::for_each(str->begin(), str->end(), [](char &c)
    {
        c = ::tolower(c);
    });
}

/**
 * This function a count hash to update , string and a hashmap for strings
 * The function checks if the strings existed in the HashMap are also existed in the string
 * The function update the count hash according to how many times it is existed.
 * @param counthash
 * @param fullmsg
 * @param hash_map
 */
void countfrequence(HashMap<std::string, int> *counthash, const std::string *fullmsg, HashMap<std::string, std::string> const *hash_map)
{
    for (HashMap<std::string, std::string>::const_iterator it = hash_map->begin(); it != hash_map->end(); it++)
    {
        int counter = 0;
        std::string l = it->first;
        size_t index = fullmsg->find(it->first);
        if (index != std::string::npos)
        {
            while (index < fullmsg->length())
            {
                counter++;
                index = fullmsg->find(it->first, index + 1);
            }
        }
        counthash->operator[](it->first) = counter;
    }
}

/**
 * This function receives a HashMap and a Hash counter and a threshold
 * The function calculates the scoring for each string
 * if the score was bigger than the threshold a SPAM is printed , NOT_SPAM otherwise.
 * @param hash_map
 * @param count_hash
 * @param threeshold
 */
void isspam(HashMap<std::string, std::string> *hash_map, HashMap<std::string, int> *count_hash, int threshold)
{
    int result = 0;
    for (HashMap<std::string, std::string>::const_iterator it = hash_map->begin(); it != hash_map->end(); it++)
    {
        if(!isparam(hash_map->operator[](it->first)))
        {
            error();
        }
        result += stoi(hash_map->operator[](it->first)) * count_hash->operator[](it->first);
    }
    if (result < threshold)
    {
        std::cout << "NOT_SPAM" << std::endl;
    }
    else
    {
        std::cout << "SPAM" << std::endl;
    }
}
/**
 * main function which runs the code .
 * we receive two files and a threshold , then we calculate the score according to the given file
 * SPAM or NOT_SPAM will be printed .
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: SpamDetector <database path> <message path> <threshold>" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(!isparam(argv[3]) || atoi(argv[3]) < 0)
    {
        error();
    }
    std::ifstream file;
    std::string line;
    file.open(argv[1], std::fstream::in);
    if (!file.is_open())
    {
        error();
    }
    if (isempty(argv))
    {
        return 0;
    }
    auto *hash_map = new HashMap<std::string, std::string>;
    while (getline(file, line))
    {
        if (line.at(line.size() - 1) == '\r' || line.at(line.size() - 1) == '\n')
        {
            line.erase(line.size() - 1);
        }
        std::string keystr;
        std::string valstr;
        for (unsigned long i = 0; i < line.length(); ++i)
        {
            if (line.empty())
            {
                file.close();
                error();
            }
            if (line[i] == ',')
            {
                keystr = line.substr(0, i);
                if (keystr.length() == 0)
                {
                    file.close();
                    error();
                }
                lowercase(&keystr);
                valstr = line.substr(i + 1, line.length() - 1);
                if(!isparam(valstr))
                {
                    file.close();
                    error();
                }
                int num = stoi(valstr);
                if (num < 0)
                {
                    file.close();
                    error();
                }
                break;
            }
        }
        hash_map->insert(keystr, valstr);
    }
    file.close();
    file.open(argv[2], std::fstream::in);
    std::string fullmsg;
    while (getline(file, line))
    {
        if(line.empty())
        {
            continue;
        }
        if (line.at(line.size() - 1) == '\r' || line.at(line.size() - 1) == '\n')
        {
            line.erase(line.size() - 1);
        }
        fullmsg += line + " ";
    }
    lowercase(&fullmsg);
    auto *count_hash = new HashMap<std::string, int>;
    countfrequence(count_hash, &fullmsg, hash_map);
    if(!isparam(argv[3]))
    {
        file.close();
        error();
    }
    isspam(hash_map, count_hash, atoi(argv[3]));
    file.close();
}