#include "../Header/MyRedis.h"


MyRedis::MyRedis() 
: mHelp{new Helper}
, mSave{new DataSave}
, mRead{new DataRead}
, CommandCount{"set", "hset", "lpush", "rpush"}
, Command(CommandCount.size())
, whiteSpace{' '}
, CommandI{0}
{
    this->init();
    int size = CommandCount.size();

    for (auto it : CommandCount) {
        KeyWord key = mHelp->getKey(it);
        mRead->fromFile(*localDB[key], mHelp->getFile(key));
    }
}

MyRedis::~MyRedis() {
    for (auto it : Command) {
        if (!it.empty()) {
            KeyWord key = mHelp->getKey(it);
            mSave->toFile(*localDB[key], mHelp->getFile(key));
        }
    }
    delete mHelp;
    delete mSave;
    delete mRead;
}

void MyRedis::init() {
    GetMap[KeyWord::GET] = &MyRedis::get;
    GetMap[KeyWord::HGET] = &MyRedis::hget;


    SetMap[KeyWord::SET] = &MyRedis::set;
    SetMap[KeyWord::HSET] = &MyRedis::hset;
    SetMap[KeyWord::LPUSH] = &MyRedis::lpush;
    SetMap[KeyWord::RPUSH] = &MyRedis::rpush;
    SetMap[KeyWord::LRANGE] = &MyRedis::lrange;

    localDB[KeyWord::SET] = &array;
    localDB[KeyWord::HSET] = &hJson;
    localDB[KeyWord::LPUSH] = &list;
    localDB[KeyWord::RPUSH] = &list;


}

void MyRedis::execute(std::string& line) {
    if (line.empty()) { return; }
    auto CommandParsed = mHelp->parser(line, whiteSpace);
    std::string command = mHelp->toLowerCase(*CommandParsed.begin());
    KeyWord CommandForCall = mHelp->getKey(command);
    auto itContains = std::find(CommandCount.begin(), CommandCount.end(), command);

    if (itContains != CommandCount.end()) {
        auto exist = std::find(Command.begin(), Command.end(), command);
        if (exist == Command.end()) {
            Command.push_back(command);
        }
    }

    auto itSet = SetMap.find(CommandForCall); 
    if (itSet == SetMap.end()) {
        auto itGet = GetMap.find(CommandForCall);
        std::cout << (this->*GetMap[(itGet->first)]) 
                        (std::vector(CommandParsed.begin() + 1, CommandParsed.end())) << std::endl;
        return;
    }
    (this->*SetMap[itSet->first]) 
                (std::vector<std::string>(CommandParsed.begin() + 1, CommandParsed.end()));
}

void MyRedis::set(const std::vector<std::string>& rhs) {
    if (rhs.size() != ErrorHandling::SETERROR) {
        throw std::invalid_argument("Syntax Error!");
    }
    auto key = rhs.begin();
    auto value = rhs.begin() + 1;
    auto forSet = std::find_if(array.begin(), array.end(), 
    [&] (const json objec) {
        return objec.begin().key() == *key;
    });

    if (forSet != array.end()) {
        (*forSet).begin().value() = *value;
        return;
    }

    json js;
    js[*key] = *value;
    array.push_back(js);
}

void MyRedis::hset(const std::vector<std::string>& rhs) {
    if (rhs.size() < ErrorHandling::HSETERROR) {
        throw std::invalid_argument("Syntax Error!");
    }
    if (((rhs.size() - 1) % 2)) {
        throw std::invalid_argument("Wrong number of arguments for 'hset' command");
    }
    std::string mapName = *rhs.begin();

    json arrForpush;
    for (auto itHash = rhs.begin() + 1; itHash != rhs.end(); itHash += 2) {
        auto findKey = std::find_if(hJson[mapName].begin(), hJson[mapName].end(),
                    [&] (const json& object) { return object.begin().key() == *itHash; });
        if (findKey != hJson[mapName].end()) {
            (*findKey).begin().value() = *(itHash + 1);
            continue;
        }
        arrForpush[*itHash] = *(itHash + 1);
        hJson[mapName].push_back(arrForpush);
        arrForpush.clear();
    }   
}

std::string MyRedis::get(const std::vector<std::string>& rhs) {
    if (rhs.size() != ErrorHandling::GETERROR) {
            throw std::invalid_argument("Syntax Error!");
    }
    auto forGet = std::find_if(array.begin(), array.end(), 
        [&] (const json objec) {
            return objec.begin().key() == *rhs.begin();
        });
    if (forGet == array.end()) {
        throw std::invalid_argument("nill");
    }
    return (*forGet).begin().value();
}

std::string MyRedis::hget(const std::vector<std::string>&  rhs) {
    if (rhs.size() != ErrorHandling::HGETERROR) {
        throw std::invalid_argument("Syntax error!");
    }
    auto hMapName = rhs.begin();
    if (hJson.find(*hMapName) == hJson.end()) {
        throw std::invalid_argument("nill");
    }

    auto Key = hMapName + 1;
    auto findKey = std::find_if(hJson[*hMapName].begin(), hJson[*hMapName].end(),
                                [&] (const json& object) { return object.begin().key() == *Key; });
    if (findKey == hJson[*hMapName].end()) {
        throw std::invalid_argument("nill");
    }
    return (*findKey).begin().value();
}


void MyRedis::lpush(const std::vector<std::string>& rhs) {
    if (rhs.size() < ErrorHandling::LPUSHERROR) {
        throw std::invalid_argument("Syntax error!");
    }
    std::string listName = *rhs.begin();
    
    auto contains = list.find(listName);
    if (contains != list.end()) {
        for (auto it = rhs.begin() + 1; it != rhs.end(); ++it) {
            list[listName].begin().value().insert(list[listName].begin().value().begin(), *it);
        } 
        return;
    }

    for (auto it = rhs.begin() + 1; it != rhs.end(); ++it) {
        mlList.push_front(*it);
    }
    list[listName].push_back(mlList);
}


void MyRedis::rpush(const std::vector<std::string>& rhs) {
    if (rhs.size() < ErrorHandling::LPUSHERROR) {
        throw std::invalid_argument("Syntax error!");
    }
    std::string listName = *rhs.begin();
    
    auto contains = list.find(listName);
    if (contains != list.end()) {
        for (auto push = rhs.begin() + 1; push != rhs.end(); ++push) {
            list[listName].begin().value().push_back(*push);
        } 
        return;
    }

    for (auto it = rhs.begin() + 1; it != rhs.end(); ++it) {
        mrList.push_back(*it);
    }
    list[listName].push_back(mrList);
}


void MyRedis::lrange(const std::vector<std::string>& rhs) {
    if (rhs.size() != ErrorHandling::LRANGEERROR) {
        throw std::invalid_argument("Syntax error!");
    }
    std::string listName = *rhs.begin();
    int start = std::stoi(*(rhs.begin() + 1));
    int end = std::stoi(*(rhs.end() - 1));

    auto selected = list.find(listName);
    if (selected == list.end()) {
        throw std::invalid_argument("nill");
    }

    int size = list[listName].begin().value().size();

    mHelp->checkStartEnd(start, end, size);

    auto startIt = list[listName].begin().value().begin() + start;
    auto endIt = list[listName].begin().value().begin();
    
    if (end >= 0) {
        endIt += end;
    } else {
        endIt = list[listName].begin().value().end() + end;
    }
    
    int count = 1;
    while (startIt <= endIt) {
        std::cout << count++ << ") " << *startIt++ << std::endl;
    }
}