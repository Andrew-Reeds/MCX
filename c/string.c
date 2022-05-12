#include "../string.h"

listDefineDefaultName(char, string);
listDefineVaListIntName(char, string);
listDefineCompare(string);

setDefineName(char, string);
setDefineVaListIntName(char, string);

static list(string) splitB(string str, char c, bool keep) {
    list(string) res = {0};
    u j = 0;
    for (u i = 0; i < str.len; i++)
        if (str.items[i] == c) {
            if (keep || i != j)
                stringListAdd(&res, stringGetRange(str, j, i - j));
            j = i + 1;
        }
    if (keep || str.len != j)
        stringListAdd(&res, stringGetRange(str, j, str.len - j));
    return res;
}
list(string) split(string str, char c) {
    return splitB(str, c, true);
}
list(string) splitR(string str, char c) {
    return splitB(str, c, false);
}
static list(string) splitSB(string str, string s, bool keep) {
    list(string) res = {0};
    if (s.len == 0) {
        stringListAdd(&res, str);
        return res;
    }
    u j = 0;
    for (u i = 0; i < str.len; i++)
        if (stringRangeCompare(str, i, s.len, s)) {
            if (keep || i != j)
                stringListAdd(&res, stringGetRange(str, j, i - j));
            i += s.len - 1;
            j = i + 1;
        }
    if (keep || j != str.len)
        stringListAdd(&res, stringGetRange(str, j, str.len - j));
    return res;
}
list(string) splitS(string str, string s) {
    return splitSB(str, s, true);
}
list(string) splitSR(string str, string s) {
    return splitSB(str, s, false);
}
static list(string) splitCSB(string str, set(char)* cs, bool keep) {
    list(string) res = {0};
    u j = 0;
    for (u i = 0; i < str.len; i++)
        if (charSetContains(cs, str.items[i])) {
            if (keep || i != j)
                stringListAdd(&res, stringGetRange(str, j, j - i));
            j = i + 1;
        }
    if (keep || str.len != j)
        stringListAdd(&res, stringGetRange(str, j, str.len - j));
    return res;
}

string* strTrim(string* str) {
    static set(char)* whitespace = NULL;
    if (!whitespace) whitespace = charAggregateFromArray(" \n\t", 3);
    return strTrimCS(str, whitespace);
}
string* strTrimC(string* str, char c) {
    u i = 0; for (; i < str->len && str->items[i] == c; i++);
    stringRemoveRange(str, 0, i);
    return str;
}
string* strTrimCS(string* str, set(char)* cs) {
    u i = 0; for (; i < str->len && charSetContains(cs, str->items[i]); i++);
    stringRemoveRange(str, 0, i);
    return str;
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
