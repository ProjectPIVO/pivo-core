#ifndef PIVO_CLIOPTS_H
#define PIVO_CLIOPTS_H

enum CommandLineOptionType
{
    CLIOPTYPE_STRING = 0,
    CLIOPTYPE_INT,
    CLIOPTYPE_BOOL,
    MAX_CLIOPTYPE
};

union CommandLineOptionUnion
{
    char* asString;
    int64_t asInt;
    // bool asBool; // we will use asInt value 0 / 1 to recognize false / true boolean value
};

struct CommandLineOptionValue
{
    CommandLineOptionType  type;
    CommandLineOptionUnion value;
};

#endif
