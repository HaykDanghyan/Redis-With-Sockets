#ifndef DATASAVE_H
#define DATASAVE_H

#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

class DataSave {
public:
    DataSave();
public:
    void toFile(const json&, const std::string&);
    // void toFileHash(const json&);
    // void toFileList(const json&);
private:
    std::fstream mWrite;
};

#endif // DATASAVE_H