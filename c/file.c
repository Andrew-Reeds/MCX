#include "../file.h"
#include <unistd.h>

void init(FILE) {
    illegalPath = charSetAdd(charAggregateFromArray("<>:\"/\\|?*", 9), charRangeNew(0, 31));
}

set(char)* illegalPath = NULL;
bool isPathLegal(string path) {
    bool res = true;
    for (u i = 0; i < path.len && res; i++)
        res = !charSetContains(illegalPath, path.items[i]);
    return res;
}
bool fileExists(string path) {
    return access(cptr(path), F_OK) == 0;
}
