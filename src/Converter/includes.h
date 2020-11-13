#include "/SelfWrittenConverter/my_conv.c"
#include "/SelfWrittenConverter/my_conv.h"
#include "/ThirdPartyWrittenConverter/qdbmp.c"
#include "/ThirdPartyWrittenConverter/qdbmp.h"
#include "/Comparer/"
#include "/Comparer/"

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define