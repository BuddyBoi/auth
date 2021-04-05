#pragma once
#include <regex>
#include <string.h>
#include <iostream>
#include <string>

std::string extract_string(std::string oStr, std::string sStr1, std::string sStr2)
{
    std::string str = oStr;
    unsigned first = str.find(sStr1);
    unsigned last = str.find(sStr2);
    std::string strNew = str.substr(first, last - first);
    strNew = strNew.substr(strNew.find_first_of('>') + 1, strNew.size());
    return strNew;
}

std::string str_remove_whitespace(std::string str)
{
    char arr_valid[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };
    std::string temp = "";
    for (char c : str)
    {
        for (char x : arr_valid)
        {
            if (x == c)
                temp += c;
        }
    }
    return temp;
}