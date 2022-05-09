#ifndef MCX_FILE_H
#define MCX_FILE_H
#include "string.h"
#include "mcx.h"

void init(FILE);

static inline string readToEnd(FILE* f) {
    long pos = ftell(f);
    fseek(f, 0l, SEEK_END);
    long size = ftell(f) - pos;
    fseek(f, pos, SEEK_SET);
    char* res = malloc(size);
    fread(res, 1, size, f);
    return str(res);
}
static inline string readAllText(string path) {
    FILE* f = fopen(cptr(path), "r");
    string res = readToEnd(f);
    fclose(f);
    return res;
}
static inline string runProcess(string command) {
    FILE* f = popen(cptr(command), "r");
    char buf[1024];
    string res = {0};
    while (fgets(buf, sizeof(buf), f) != NULL) stringAddRange(&res, sstr(buf));
    if (WEXITSTATUS(pclose(f)) == 0) return res;
    reset(&res, string);
    return res;
}
static inline void writeAllText(string path, string text) {
    FILE* f = fopen(cptr(path), "w");
    fputs(cptr(text), f);
    fclose(f);
}

typedef enum {
   P_DIR  = 1,
   P_REG  = 2,
   P_FULL = 4,
} PATHPROP;
list(string) listFiles(string directory, PATHPROP kind);

static inline string realPath(string path) {
    string cmd = str("realpath ");
    string res = runProcess(*stringAddRange(&cmd, path));
    if (res.len > 0 && res.items[res.len - 1] == '\n') stringRemove(&res, res.len - 1);
    if (path.len > 0 && path.items[path.len - 1] == '/') stringAdd(&res, '/');
    return res;
}
bool fileExists(string path);
extern set(char)* illegalPath;
bool isPathLegal(string path);

#endif // MCX_FILE_H
