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

list(string) listFiles(string directory, PATHPROP kind) {
    if (directory.len == 0 || directory.items[directory.len - 1] != '/') directory = stringWith(directory, '/');
    string cmd = str("ls -p1 --color=no ");
    stringAddRange(&cmd, directory);
    if ((kind | P_DIR) != kind) stringAddRange(&cmd, sstr(" | grep -v / --color=never"));
    else if ((kind | P_REG) != kind) stringAddRange(&cmd, sstr(" | grep -s / --color=never"));
    list(string) res = splitR(runProcess(cmd), '\n');
    if (kind & P_FULL) for (u i = 0; i < res.len; i++) stringInsertRange(&res.items[i], directory, 0);
    return res;
}
