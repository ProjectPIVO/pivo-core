#ifndef PIVO_UNIT_IDENTIFIERS_H
#define PIVO_UNIT_IDENTIFIERS_H

#define NO_CLASS -1

struct ClassEntry
{
    // Class/struct name
    const char* name;
};

struct FunctionEntry
{
    // Function/method name
    const char* name;
    // If function (method) belongs to class, this is the index to class table
    int32_t classId;
};

#endif
