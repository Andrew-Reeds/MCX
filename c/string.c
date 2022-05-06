#include "../string.h"

listDefineDefaultName(char, string);
listDefineVaListIntName(char, string);
listDefineCompare(string);

setDefineName(char, string);
setDefineVaListIntName(char, string);

list(string) split(string str, char c) {
    list(string) res = {0};
    u s = 0;
    for (u i = 0; i < str.len; i++)
        if (str.items[i] == c) {
            stringListAdd(&res, stringGetRange(str, s, i - s));
            s = i + 1;
        }
    stringListAdd(&res, stringGetRange(str, s, str.len - s));
    return res;
}
list(string) splitS(string str, string s) {
    list(string) res = {0};
    u j = 0;
    for (u i = 0; i < str.len; i++)
        if (stringRangeCompare(str, i, s.len, s)) {
            stringListAdd(&res, stringGetRange(str, j, i - j));
            i += s.len;
            j = i + 1;
        }
    stringListAdd(&res, stringGetRange(str, j, str.len - j));
    return res;
}

char getEscChar(char c) {
    switch (c) {
        case 'a':
            return '\a';
            break;
        case 'b':
            return '\b';
            break;
        case 'f':
            return '\f';
            break;
        case 'n':
            return '\n';
            break;
        case 'r':
            return '\r';
            break;
        case 't':
            return '\t';
            break;
        case 'v':
            return '\v';
            break;
        case '\'':
            return '\'';
            break;
        case '"':
            return '\"';
            break;
        case '\\':
            return '\\';
            break;
        case '?':
            return '\?';
            break;
        default:
            return 0;
    }
}
