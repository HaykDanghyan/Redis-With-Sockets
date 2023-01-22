#ifndef DATAREAD_H
#define DATAREAD_H


#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class DataRead {
public:
    DataRead();
public:
    void fromFile(json&, const std::string&);

private:
    std::fstream mRead;
};

#endif // DATAREAD_H