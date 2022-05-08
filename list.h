#ifndef MCX_LIST_H
#define MCX_LIST_H
#include <stdbool.h>
#include <stdarg.h>
#include "mcx.h"

#define list(type) type##List
#define listDeclareName(type, name)                             \
    typedef struct {                                            \
        u len;                                                  \
        u cap;                                                  \
        type* items;                                            \
    } name;                                                     \
    static inline name* name##New() {                           \
        return new(name);                                       \
    }                                                           \
    name name##FromArray(type* items, u count);                 \
    static inline name name##Clone(name original) {             \
        if (original.cap == 0) {                                \
            if (original.len == 0) return original;             \
            else return name##FromArray(original.items, original.len); \
        }                                                       \
        name res = { original.len, original.cap, (type*)malloc(original.cap * sizeof(type)) }; \
        memcpy(res.items, original.items, original.len);        \
        return res;                                             \
    }                                                           \
    void name##Add(name* list, type item);                      \
    void name##AddRange(name* list, name other);                \
    name name##With(name list, type item);                      \
    name name##WithRange(name list, name other);                \
    void name##Insert(name* list, type item, u index);          \
    void name##InsertRange(name* list, name other, u index);    \
    void name##Remove(name* list, u index);                     \
    void name##RemoveRange(name* list, u index, u count);       \
    name name##GetRange(name list, u index, u count);
#define listDeclareVaListName(type, name)       \
    name name##FromVaList(u count, ...);
#define listDeclareCompareName(type, name)                              \
    listDeclareName(type, name);                                        \
    bool name##Contains(name list, type item);                          \
    int name##RangeCompare(name list, u offset, u count, name other);   \
    static inline int name##Compare(name list, name other) {            \
        return name##RangeCompare(list, 0, list.len, other);            \
    }                                                                   \
    static inline bool name##Equals(name list, name other) {            \
        return list.len == other.len && name##Compare(list, other) == 0; \
    }                                                                   \
    static inline bool name##StartsWith(name list, name other) {        \
        return other.len <= list.len && name##RangeCompare(list, 0, other.len, other) == 0; \
    }                                                                   \
    static inline bool name##EndsWith(name list, name other) {          \
        return other.len <= list.len && name##RangeCompare(list, list.len - other.len, other.len, other) == 0; \
    }                                                                   \
    void name##RemoveAll(name* list, type item);                        \
    void name##ReplaceAll(name* list, name original, name replacement); \
    u name##Pos(name list, type item);                                  \
    u name##LastPos(name list, type item);
#define listDeclareDefaultName(type, name)                       \
    listDeclareCompareName(type, name);                          \
    static inline int type##Compare(type left, type right) {     \
        return (left > right) - (left < right);                  \
    }
#define listDeclare(type) listDeclareName(type, type##List)
#define listDeclareVaList(type) listDeclareVaListName(type, type##List)
#define listDeclareDefault(type) listDeclareDefaultName(type, type##List)
#define listDeclareCompare(type) listDeclareCompareName(type, type##List)

#define listDefineName(type, name)                                      \
    name name##FromArray(type* items, u count) {                        \
        name res = { count, 0, NULL };                                  \
        if (res.len == 0)                                               \
            return res;                                                 \
        res.cap = 16;                                                   \
        while (res.len > res.cap)                                       \
            res.cap <<= 1;                                              \
        res.items = (type*)malloc(res.cap * sizeof(type));              \
        memcpy(res.items, items, res.len);                              \
        return res;                                                     \
    }                                                                   \
    void name##Add(name* list, type item) {                             \
        if (list->cap == 0)                                             \
            listReset(*list, type);               \
        list->len++;                                                    \
        if (list->len > list->cap) {                                    \
            list->cap <<= 1;                                            \
            list->items = (type*)realloc(list->items, list->cap * sizeof(type)); \
        }                                                               \
        list->items[list->len - 1] = item;                              \
    }                                                                   \
    void name##AddRange(name* list, name other) {                       \
        if (other.len == 0)                                             \
            return;                                                     \
        if (list->cap == 0)                                             \
            listReset(*list, type);               \
        list->len += other.len;                                         \
        if (list->len > list->cap) {                                    \
            while (list->len > list->cap)                               \
                list->cap <<= 1;                                        \
            list->items = (type*)realloc(list->items, list->cap * sizeof(type)); \
        }                                                               \
        memcpy((void*)((ptr)list->items + (list->len - other.len) * sizeof(type)), \
               (void*)other.items,                                      \
               other.len * sizeof(type));                               \
    }                                                                   \
    name name##With(name list, type item) {                             \
        name res = { list.len, list.cap, clone(list.items, type, list.cap) }; \
        name##Add(&res, item);                                          \
        return res;                                                     \
    }                                                                   \
    name name##WithRange(name list, name other) {                       \
        name res = { list.len, list.cap, clone(list.items, type, list.cap) }; \
        name##AddRange(&res, other);                                    \
        return res;                                                     \
    }                                                                   \
    void name##Insert(name* list, type item, u index) {                 \
        if (list->cap == 0)                                             \
            listReset(*list, type);               \
        list->len++;                                                    \
        if (list->len > list->cap) {                                    \
            list->cap <<= 1;                                            \
            list->items = (type*)realloc(list->items, list->cap * sizeof(type)); \
        }                                                               \
        memcpy((void*)((ptr)list->items + (index + 1) * sizeof(type)),  \
               (void*)((ptr)list->items + index * sizeof(type)),        \
               list->len - index - 1);                                  \
        list->items[index] = item;                                      \
    }                                                                   \
    void name##InsertRange(name* list, name other, u index) {           \
        if (other.len == 0)                                             \
            return;                                                     \
        if (list->cap == 0)                                             \
            listReset(*list, type);                                     \
        list->len += other.len;                                         \
        if (list->len > list->cap) {                                    \
            while (list->len > list->cap)                               \
                list->cap <<= 1;                                        \
            list->items = (type*)realloc(list->items, list->cap * sizeof(type)); \
        }                                                               \
        memcpy((void*)((ptr)list->items + (index + other.len) * sizeof(type)), \
               (void*)((ptr)list->items + index * sizeof(type)),        \
               list->len - index - other.len);                          \
        memcpy((void*)((ptr)list->items + index * sizeof(type)),        \
               other.items, other.len);                                 \
    }                                                                   \
    void name##Remove(name* list, u index) {                            \
        memcpy((void*)((ptr)(list->items) + index * sizeof(type)),      \
               (void*)((ptr)(list->items) + (index + 1) * sizeof(type)), \
               list->len - index - 1);                                  \
        list->len--;                                                    \
        if (list->len < list->cap >> 1 && list->cap > 16) {             \
            while (list->len < list->cap >> 1 && list->cap > 16)        \
                list->cap >>=1;                                         \
            list->items = (type*)realloc(list->items, list->cap * sizeof(type)); \
        }                                                               \
    }                                                                   \
    void name##RemoveRange(name* list, u index, u count) {              \
        memcpy((void*)((ptr)(list->items) + index * sizeof(type)),      \
               (void*)((ptr)(list->items) + (index + count) * sizeof(type)), \
               list->len - index - count);                              \
        list->len -= count;                                             \
        if (list->len < list->cap >> 1 && list->cap > 16) {             \
            while (list->len < list->cap >> 1 && list->cap > 16)        \
                list->cap >>=1;                                         \
            list->items = (type*)realloc(list->items, list->cap * sizeof(type)); \
        }                                                               \
    }                                                                   \
    name name##GetRange(name list, u index, u count) {                  \
        if (count == 0) {                                               \
            name res = { 0 };                                           \
            return res;                                                 \
        }                                                               \
        name res = { count, 16, NULL };                                 \
        while (res.len > res.cap)                                       \
            res.cap <<= 1;                                              \
        res.items = (type*)malloc(res.cap * sizeof(type));              \
        memcpy(res.items, &list.items[index], count);                   \
        return res;                                                     \
    }
#define listDefineVaListName(type, name)                    \
    name name##FromVaList(u count, ...) {                   \
        va_list args;                                       \
        va_start(args, count);                              \
        name res = { count, 16, NULL };                     \
        while (count > res.cap)                             \
            res.cap <<= 1;                                  \
        res.items = (type*)malloc(res.cap * sizeof(type));  \
        for (u i = 0; i < count; i++)                       \
            res.items[i] = va_arg(args, type);              \
        va_end(args);                                       \
        return res;                                         \
    }
#define listDefineVaListIntName(type, name)                 \
    name name##FromVaList(u count, ...) {                   \
        va_list args;                                       \
        va_start(args, count);                              \
        name res = { count, 16, NULL };                     \
        while (count > res.cap)                             \
            res.cap <<= 1;                                  \
        res.items = (type*)malloc(res.cap * sizeof(type));  \
        for (u i = 0; i < count; i++)                       \
            res.items[i] = va_arg(args, int);               \
        va_end(args);                                       \
        return res;                                         \
    }
#define listDefineCompareName(type, name)                               \
    listDefineName(type, name);                                         \
    bool name##Contains(name list, type item) {                         \
        for (u i = 0; i < list.len; i++)                                \
            if (type##Compare(list.items[i], item) == 0)                \
                return true;                                            \
        return false;                                                   \
    }                                                                   \
    int name##RangeCompare(name list, u offset, u count, name other) {  \
        if (count + offset > list.len)                                  \
            count = list.len - offset;                                  \
        u i = 0;                                                        \
        for ( ; i < count && i < other.len && i + offset < list.len; i++) { \
            int c = type##Compare(list.items[i + offset], other.items[i]); \
            if (c != 0)                                                 \
                return c;                                               \
        }                                                               \
        return (count > other.len) - (count < other.len);               \
    }                                                                   \
    void name##RemoveAll(name* list, type item) {                       \
        for (u i = list->len; i > 0; i--)                               \
            if (type##Compare(list->items[i - 1], item) == 0)           \
                name##Remove(list, i - 1);                              \
    }                                                                   \
    void name##ReplaceAll(name* list, name original, name replacement) { \
        if (original.len == 0 || list->len < original.len)              \
            return;                                                     \
        uList indexes = {0};                                            \
        for (u i = 0; i <= list->len - original.len; i++)               \
            if (name##RangeCompare(*list, i, original.len, original) == 0) { \
                uListAdd(&indexes, i);                                  \
                i += original.len - 1;                                  \
            }                                                           \
        for (u i = 0; i < indexes.len; i++) {                           \
            name##RemoveRange(list, indexes.items[indexes.len - 1 - i], original.len); \
            name##InsertRange(list, replacement, indexes.items[indexes.len - 1 - i]); \
        }                                                               \
    }                                                                   \
    u name##Pos(name list, type item) {                                 \
        u i = 0;                                                        \
        for (; i < list.len && type##Compare(list.items[i], item) != 0; i++); \
        return i;                                                       \
    }                                                                   \
    u name##LastPos(name list, type item) {                             \
        u i = list.len;                                                 \
        for (; i > 0 && type##Compare(list.items[i - 1], item) != 0; i--); \
        if (i == 0)                                                     \
            return list.len;                                            \
        return i - 1;                                                   \
    }
#define listDefineDefaultName(type, name) listDefineCompareName(type, name)
#define listDefine(type) listDefineName(type, type##List)
#define listDefineVaList(type) listDefineVaListName(type, type##List)
#define listDefineVaListInt(type) listDefineVaListIntName(type, type##List)
#define listDefineCompare(type) listDefineCompareName(type, type##List)
#define listDefineDefault(type) listDefineDefaultName(type, type##List)
#define listReset(list, type) {                   \
        (list).len = 0;                           \
        (list).cap = 16;                          \
        (list).items = malloc(16 * sizeof(type)); \
    }

listDeclareDefault(u);
listDeclareVaList(u);

#endif // MCX_LIST_H
