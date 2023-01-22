#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <list>

#include "KeyWords.h"
#include "DataSave.h"
#include "DataRead.h"



class Helper {
public:
    Helper();

public:
    std::string toLowerCase(const std::string&);
    std::vector<std::string> parser(const std::string& str, const std::string& spliter);
    KeyWord getKey(const std::string&);
    std::string getFile(const KeyWord&);
    void checkStartEnd(int& start, int& end, const int size);

private:
    void init();

private:
    std::map<std::string, KeyWord> findCommand;
    std::map<KeyWord, std::string> fileJson;

private:
    std::string setFile;
    std::string hashFile;
    std::string listFile;

};

#endif // HELPER_H