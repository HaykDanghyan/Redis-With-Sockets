#include "../Header/DataRead.h"

DataRead::DataRead() {}

void DataRead::fromFile(json& objArray, const std::string& fileName) {
    mRead.open(fileName.c_str());
    if (!mRead.is_open()) {
        throw std::runtime_error("Can't open DB");
    }
    std::string json_str((std::istreambuf_iterator<char>(mRead)), std::istreambuf_iterator<char>());

    if (json_str.empty()) { 
        mRead.close();
        return;
    }

    objArray = json::parse(json_str);
    mRead.close();
}