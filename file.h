#ifndef MCX_FILE_H
#define MCX_FILE_H
#include "string.h"
#include "mcx.h"

string realPath(string path);
string realPathR(string path, string relativeTo);

bool fileExists(string path);
bool isPathLegal(string path);

static inline string readToEnd(FILE* f) {
    long pos = ftell(f);
    fseek(f, 0l, SEEK_END);
    long size = ftell(f) - pos;
    fseek(f, pos, SEEK_SET);
    char* res = malloc(size + 1);
    fread(res, 1, size, f);
    res[size] = 0;
    return str(res);
}
static inline string readAllText(string path) {
    FILE* f = fopen(cptr(realPath(path)), "r");
    string res = readToEnd(f);
    fclose(f);
    return res;
}
static inline string runProcess(string command) {
    FILE* f = popen(cptr(command), "r");
    char buf[1024];
    string res = {0};
    while (fgets(buf, sizeof(buf), f) != NULL) stringAddRange(&res, str(buf));
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

#endif // MCX_FILE_H
