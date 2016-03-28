#ifndef PIVO_OUTPUT_MODULE_FEATURES_H
#define PIVO_OUTPUT_MODULE_FEATURES_H

enum OutputModuleFeatures
{
    OMF_FLAT_PROFILE = 0,
    OMF_CALL_GRAPH = 1,
    OMF_CALL_TREE = 2,
    MAX_OMF
};

#define OMF_SET uint64_t
#define OMF_CREATE(a) a = 0
#define OMF_ADD(a,f) a = a | (1 << f)
#define OMF_REM(a,f) a = a & ~(1 << f)
#define OMF_ISSET(a,f) ((a & (1 << f)) != 0)

#endif
