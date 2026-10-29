#include <string>
#include <fstream>
#include <sstream>
#include <DxLib.h>
#include "AsoUtility.h"

std::vector<std::string> AsoUtility::Split(std::string& line, char delimiter)
{
    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;

    while (getline(stream, field, delimiter))
    {
		result.push_back(field);
    }

    return result;
}