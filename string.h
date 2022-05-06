#ifndef MCX_STRING_H
#define MCX_STRING_H
#include <stdio.h>
#include "list.h"
#include "set.h"

listDeclareDefaultName(char, string);
listDeclareVaListName(char, string);

setDeclareName(char, string);
setDeclareVaList(char);
listDeclareCompare(string);

static inline char* cptr(string str) {
    char* res = (char*)malloc((str.len + 1) * sizeof(char));
    memcpy(res, str.items, str.len);
    res[str.len] = 0;
    return res;
}
static inline string sstr(char* str) {
    string res = { strlen(str), 0, str };
    res.cap = res.len;
    return res;
}
static inline string str(char* str) {
    if (str == NULL) {
        string res = {0};
        return res;
    } else
        return stringFromArray(str, strlen(str));
}


list(string) split(string str, char c);
list(string) splitR(string str, char c);
list(string) splitS(string str, string s);
list(string) splitSR(string str, string s);

static inline string substring(string str, u index) {
    return stringGetRange(str, index, str.len - index);
}
static inline void concat(string* str, string other) {
    return stringAddRange(str, other);
}
static inline void catCptr(string* str, char* other) {
    return concat(str, sstr(other));
}

static inline string utos(u64 u) {
    string res = { 0, 64, (char*)malloc(64) };
    snprintf(res.items, 64, "%zu", u);
    res.len = strlen(res.items);
    return res;
}
static inline string itos(i64 i) {
    string res = { 0, 64, (char*)malloc(64) };
    snprintf(res.items, 64, "%zd", i);
    res.len = strlen(res.items);
    return res;
}

static inline char* ctcptr(char c) {
    char* res = malloc(2);
    res[0] = c;
    res[1] = 0;
    return res;
}

char getEscChar(char c);

#endif // MCX_STRING_H
