#ifndef PIVO_INPUT_MODULE_FEATURES_H
#define PIVO_INPUT_MODULE_FEATURES_H

enum InputModuleFeatures
{
    IMF_FLAT_PROFILE = 0,
    IMF_CALL_GRAPH = 1,
    MAX_IMF
};

#define IMF_SET uint64_t
#define IMF_CREATE(a) a = 0
#define IMF_ADD(a,f) a = a | (1 << f)
#define IMF_REM(a,f) a = a & ~(1 << f)
#define IMF_ISSET(a,f) ((a & (1 << f)) != 0)

#endif
