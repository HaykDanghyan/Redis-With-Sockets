#ifndef MYREDIS_H
#define MYREDIS_H

#include <nlohmann/json.hpp>
#include "Helper.h"

class MyRedis {
public:
    MyRedis();
    ~MyRedis();
public:
    void execute(std::string&);

public:

private:
    void init();

private:
    using Getter = std::string (MyRedis::*) (const std::vector<std::string>&);  
    using Setter = void (MyRedis::*) (const std::vector<std::string>&); 
    using json = nlohmann::json;
    std::map<KeyWord, Getter> GetMap; 
    std::map<KeyWord, Setter>  SetMap;

private:
    void set(const std::vector<std::string>&);
    void hset(const std::vector<std::string>&);
    void lpush(const std::vector<std::string>&);
    void rpush(const std::vector<std::string>&);
    void lrange(const std::vector<std::string>&);
    std::string get(const std::vector<std::string>&);
    std::string hget(const std::vector<std::string>&);

private:
    // std::map<std::string, json> hMap; 
    Helper* mHelp;
    DataSave* mSave;
    DataRead* mRead;
    json array;
    json hJson;
    json list;
    std::list<std::string> mlList;
    std::list<std::string> mrList;

private:
    std::map<KeyWord, json*> localDB;

private:
    std::vector<std::string> Command;
    std::vector<std::string> CommandCount;

    int CommandI;

private:
    const std::string whiteSpace;

};

#endif // MYREDIS_H