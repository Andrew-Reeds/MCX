#ifndef FILE_H
#define FILE_H

#include "string.h"

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
    FILE* f = popen("objdump -d tmp.o", "r");
    string res = readToEnd(f);
    pclose(f);
    return res;
}
static inline void writeAllText(string path, string text) {
    FILE* f = fopen(cptr(path), "w");
    fputs(cptr(text), f);
    fclose(f);
}
static inline bool fileExists(string path) {
    return access(cptr(path), F_OK) == 0;
}
static inline string realPath(string path) {
    return str(realpath(cptr(path), NULL));
}

extern set(char)* illegalPath;
bool isPathLegal(string path);

#endif // FILE_H
