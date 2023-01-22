#include "../Header/Helper.h"

Helper::Helper()
: setFile{"DB/object.json"}
, hashFile{"DB/Hash.json"}
, listFile{"DB/list.json"} 
{
    this->init();
}

void Helper::init() {
    findCommand["get"] = KeyWord::GET;
    findCommand["hget"] = KeyWord::HGET;

    findCommand["set"] = KeyWord::SET;
    findCommand["hset"] = KeyWord::HSET;

    findCommand["lpush"] = KeyWord::LPUSH;
    findCommand["rpush"] = KeyWord::RPUSH;
    findCommand["lrange"] = KeyWord::LRANGE;

    fileJson[KeyWord::SET] = setFile;
    fileJson[KeyWord::HSET] = hashFile;
    fileJson[KeyWord::LPUSH] = listFile;
    fileJson[KeyWord::RPUSH] = listFile;
}

KeyWord Helper::getKey(const std::string& src) {
    if (!static_cast<int>(findCommand[src])) { 
        throw std::invalid_argument("Syntax error");
    }
    return findCommand[src];
}


std::string Helper::getFile(const KeyWord& key) {
    return fileJson[key];
}

std::string Helper::toLowerCase(const std::string& input_str) {
    int size = input_str.size();
    std::string result{};
    for (int i = 0; i < size; i++) {
        result += tolower(input_str[i]);
    }
    return result;
}

std::vector<std::string> Helper::parser(const std::string& forSplit, const std::string& spliter) {
    std::vector<std::string> result = {};
	std::string forPush = {};
	for (int i = 0; i < forSplit.size(); ++i)
	{
		if (spliter.find(forSplit[i]) != std::string::npos)
		{   
            if (forPush != "")
            {
			    result.push_back(forPush);
			    forPush.clear();
			    continue;
            }
            continue;
		}
		forPush.push_back(forSplit[i]);
	}
    if (forPush.empty()) {
        return result;
    }
	result.push_back(forPush);
	return result;
}


void Helper::checkStartEnd(int& start, int& end, const int size) {
    if (start > end && end > 0) {
        throw std::invalid_argument("(empty array)");
    }
    
    if (start >= size || start < 0) {
        throw std::invalid_argument("(empty array)");
    }
    if (end < 0 && (std::abs(end) > size)) {
        throw std::invalid_argument("(empty array)");
    }

    if (size < end) {
        end = size - 1;
    }
}