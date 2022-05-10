#include "../file.h"
#include <unistd.h>

string realPath(string path) {
    string res = {0};
    if (path.len == 0) return res;
    string cmd = str("realpath ");
    res = runProcess(*concat(&cmd, path));
    if (path.items[path.len - 1] == '/') res.items[res.len - 1] = '/';
    else stringRemove(&res, res.len - 1);
    return res;
}
string realPathR(string path, string relativeTo) {
    string here = realPath(str("."));
    chdir(cptr(relativeTo));
    string res = realPath(path);
    chdir(cptr(here));
    return  res;
}

bool isPathLegal(string path) {
    static set(char)* illegalPath = NULL;
    if (!illegalPath) illegalPath = charSetAdd(charAggregateFromArray("<>:\"/\\|?*", 9), charRangeNew(0, 31));
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
    if ((kind | P_DIR) != kind) stringAddRange(&cmd, str(" | grep -v / --color=never"));
    else if ((kind | P_REG) != kind) stringAddRange(&cmd, str(" | grep -s / --color=never"));
    list(string) res = splitR(runProcess(cmd), '\n');
    if (kind & P_FULL) for (u i = 0; i < res.len; i++) res.items[i] = stringWithRange(directory, res.items[i]);
    return res;
}
