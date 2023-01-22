#include "../Header/DataSave.h"
#include <iostream>
DataSave::DataSave() {}


void DataSave::toFile(const json& array, const std::string& fileName) {
    mWrite.open(fileName, std::ios::out);
    if (!mWrite.is_open()) {
        throw std::runtime_error("Can't open object.json");
    }
    mWrite << array.dump(4);
    mWrite.close();
}