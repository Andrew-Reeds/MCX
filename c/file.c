#include "../file.h"

void init(FILE) {
    illegalPath = charSetAdd(charAggregateFromArray("<>:\"/\\|?*", 9), charRangeNew(0, 31));
}

set(char)* illegalPath = NULL;
bool isPathLegal(string path) {
    bool res = path.len <= PATH_MAX;
    for (u i = 0; i < path.len && res; i++)
        res = !charSetContains(illegalPath, path.items[i]);
    return res;
}
