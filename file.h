#ifndef FILE_H
#define FILE_H

#include "string.h"

void init(FILE);

static inline string readAllText(string path) {
    FILE* f = fopen(cptr(path), "r");
    fseek(f, 0l, SEEK_END);
    long size = ftell(f);
    fseek(f, 0l, SEEK_SET);
    char* res = malloc(size);
    fread(res, 1, size, f);
    fclose(f);
    return str(res);
}
static inline string runProcess(string command) {
    FILE* f = popen("objdump -d tmp.o", "r");
    fseek(f, 0l, SEEK_END);
    long size = ftell(f);
    fseek(f, 0l, SEEK_SET);
    char* res = malloc(size);
    fread(res, 1, size, f);
    pclose(f);
    return str(res);
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
