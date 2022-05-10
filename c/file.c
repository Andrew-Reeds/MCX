#include "../file.h"
#include <unistd.h>

void init(FILE) {
    illegalPath = charSetAdd(charAggregateFromArray("<>:\"/\\|?*", 9), charRangeNew(0, 31));
}

string realPathR(string path, string relativeTo) {
    string cmd = str("realpath ");
    concat(&cmd, path);
    if (relativeTo.len != 0) concat3(&cmd, sstr(" --relative-to="), realPath(relativeTo));
    string res = runProcess(cmd);
    if (res.len > 0 && res.items[res.len - 1] == '\n') stringRemove(&res, res.len - 1);
    if (path.len > 0 && path.items[path.len - 1] == '/') stringAdd(&res, '/');
    return res;
}

set(char)* illegalPath = NULL;
bool isPathLegal(string path) {
    bool res = true;
    for (u i = 0; i < path.len && res; i++)
        res = !charSetContains(illegalPath, path.items[i]);
    return res;
}
bool fileExists(string path) {
    return access(cptr(realPath(path)), F_OK) == 0;
}

list(string) listFiles(string directory, PATHPROP kind) {
    if (directory.len == 0 || directory.items[directory.len - 1] != '/') directory = stringWith(directory, '/');
    string cmd = str("ls -p1 --color=no ");
    stringAddRange(&cmd, directory);
    if ((kind | P_DIR) != kind) stringAddRange(&cmd, sstr(" | grep -v / --color=never"));
    else if ((kind | P_REG) != kind) stringAddRange(&cmd, sstr(" | grep -s / --color=never"));
    list(string) res = splitR(runProcess(cmd), '\n');
    if (kind & P_FULL) for (u i = 0; i < res.len; i++) res.items[i] = stringWithRange(directory, res.items[i]);
    return res;
}
