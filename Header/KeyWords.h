#ifndef KEYWORD_H
#define KEYWORD_H

enum class KeyWord {
    SET = 1,
    GET,
    KEY,
    HGET,
    HSET,
    LPUSH,
    RPUSH,
    LLEN,
    LRANGE
};

enum ErrorHandling {
    GETERROR = 1,
    SETERROR = 2,
    HGETERROR = 2,
    HSETERROR = 3,
    LPUSHERROR = 2,
    LRANGEERROR = 3,
    COMMANDCOUNT = 4
};

#endif // KEYWORD_H