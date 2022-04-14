#ifndef MCX_SET_H
#define MCX_SET_H
#include "list.h"

typedef enum {
    SCSADD,
    SCSSUB,
    SCSCRS,
} SETCOMBINATIONSTYLE;
#define set(type) type##Set
#define aggregate(type) type##Aggregate
#define range(type) type##Range
#define setDeclareName(type, listName)                                  \
    struct type##Set_s;                                                 \
    typedef struct type##Set_s set(type);                               \
    struct type##Set_s {                                                \
        bool (*contains)(set(type)* set, type item);                    \
    };                                                                  \
    typedef struct {                                                    \
        bool (*contains)(set(type)* set, type item);                    \
        listName items;                                                 \
    } aggregate(type);                                                  \
    typedef struct {                                                    \
        bool (*contains)(set(type)* set, type item);                    \
        type##Set* orgnl;                                               \
    } type##ComplementSet;                                              \
    typedef struct {                                                    \
        bool (*contains)(set(type)* set, type item);                    \
        type##Set* left;                                                \
        type##Set* right;                                               \
        SETCOMBINATIONSTYLE scs;                                        \
    } type##CombinedSet;                                                \
    bool type##AggregateContains(set(type)* set, type item);            \
    bool type##ComplementSetContains(set(type)* set, type item);        \
    bool type##CombinedSetContains(set(type)* set, type item);          \
    aggregate(type) type##AggregateDefault();                           \
    type##ComplementSet type##ComplementSetDefault();                   \
    type##CombinedSet type##CombinedSetDefault();                       \
    set(type)* type##AggregateNew(listName list);                       \
    set(type)* type##AggregateFromArray(type* items, u count);          \
    set(type)* type##ComplementSetNew(set(type)* set);                  \
    set(type)* type##CombinedSetNew(set(type)* left, set(type)* right, SETCOMBINATIONSTYLE style); \
    set(type)* type##SetAdd(set(type)* left, set(type)* right);         \
    set(type)* type##SetSubstract(set(type)* left, set(type)* right);   \
    set(type)* type##Cross(set(type)* left, set(type)* right);          \
    set(type)* type##SetComplement(set(type)* set);                     \
    bool type##SetContains(set(type)* set, type item)
#define setDeclareVaList(type)                          \
    set(type)* type##AggregateFromVaList(u count, ...);
#define setDeclareCompareName(type, listName)               \
    setDeclareName(type, listName);                         \
    typedef struct {                                        \
        bool (*contains)(set(type)* set, type item);        \
        bool inclMin;                                       \
        bool inclMax;                                       \
        type min;                                           \
        type max;                                           \
    } range(type);                                          \
    bool type##RangeContains(set(type)* set, type item);    \
    range(type) type##RangeDefault();                       \
    type##Set* type##RangeNew(type min, type max)
#define setDeclareDefaultName(type, listName)       \
    setDeclareCompareName(type, listName);          \
    bool type##LessThan(type left, type right);     \
    bool type##GreaterThan(type left, type right)
#define setDeclare(type) setDeclareName(type, type##List);
#define setDeclareCompare(type) setDeclareCompareName(type, type##List)
#define setDeclareDefault(type) setDeclareDefaultName(type, type##List)

#define setDefineName(type, listName)                                   \
    bool type##AggregateContains(set(type)* set, type item) {           \
        return listName##Contains(as(aggregate(type), set)->items, item); \
    }                                                                   \
    bool type##ComplementSetContains(set(type)* set, type item) {       \
        return !(*as(type##ComplementSet, set)->orgnl->contains)(as(type##ComplementSet, set)->orgnl, item); \
    }                                                                   \
    bool type##CombinedSetContains(set(type)* set, type item) {         \
        bool left = (*as(type##CombinedSet, set)->left->contains)(as(type##CombinedSet, set)->left, item); \
        bool right = (*as(type##CombinedSet, set)->right->contains)(as(type##CombinedSet, set)->right, item); \
        return (as(type##CombinedSet, set)->scs == SCSADD && (left || right)) || \
            (as(type##CombinedSet, set)->scs == SCSSUB && (left && !right)) || \
            (as(type##CombinedSet, set)->scs == SCSCRS && (left && right)); \
    }                                                                   \
    type##ComplementSet type##ComplementSetDefault() {                  \
        type##ComplementSet res = { &type##ComplementSetContains, NULL }; \
        return res;                                                     \
    }                                                                   \
    type##CombinedSet type##CombinedSetDefault() {                      \
        type##CombinedSet res = { &type##CombinedSetContains, NULL, NULL, SCSADD }; \
        return res;                                                     \
    }                                                                   \
    type##Set* type##AggregateNew(listName items) {                     \
        set(type)* res = as(set(type), new(aggregate(type)));           \
        as(aggregate(type), res)->items = items;                        \
        return res;                                                     \
    }                                                                   \
    type##Set* type##ComplementSetNew(set(type)* set) {                 \
        set(type)* res = as(set(type), new(type##ComplementSet));       \
        *as(type##ComplementSet, res) = type##ComplementSetDefault();   \
        as(type##ComplementSet, res)->orgnl = set;                      \
        return res;                                                     \
    }                                                                   \
    type##Set* type##CombinedSetNew(set(type)* left, set(type)* right, SETCOMBINATIONSTYLE style) { \
        set(type)* res = as(set(type), new(type##CombinedSet));         \
        *as(type##CombinedSet, res) = type##CombinedSetDefault();       \
        as(type##CombinedSet, res)->left = left;                        \
        as(type##CombinedSet, res)->right = right;                      \
        as(type##CombinedSet, res)->scs = style;                        \
        return res;                                                     \
    }                                                                   \
    set(type)* type##AggregateFromArray(type* items, u count) {         \
        return type##AggregateNew(listName##FromArray(items, count));   \
    }                                                                   \
    set(type)* type##SetAdd(set(type)* left, set(type)* right) {        \
        return type##CombinedSetNew(left, right, SCSADD);               \
    }                                                                   \
    set(type)* type##SetSubstract(set(type)* left, set(type)* right) {  \
        return type##CombinedSetNew(left, right, SCSSUB);               \
    }                                                                   \
    set(type)* type##Cross(set(type)* left, set(type)* right) {         \
        return type##CombinedSetNew(left, right, SCSCRS);               \
    }                                                                   \
    set(type)* type##SetComplement(set(type)* set) {                    \
        return type##ComplementSetNew(set);                             \
    }                                                                   \
    bool type##SetContains(set(type)* set, type item) {                 \
        return (*set->contains)(set, item);                             \
    }
#define setDefineVaList(type)                               \
    set(type)* type##AggregateFromVaList(u count, ...) {    \
        va_list args;                                       \
        va_start(args, count);                              \
        list(type) res = { count, 16, NULL };               \
        while (count > res.cap)                             \
            res.cap <<= 1;                                  \
        res.items = (type*)malloc(res.cap * sizeof(type));  \
        for (u i = 0; i < count; i++)                       \
            res.items[i] = va_arg(args, type);              \
        va_end(args);                                       \
        return type##AggregateNew(res);                     \
    }
#define setDefineVaListIntName(type, name)                  \
    set(type)* type##AggregateFromVaList(u count, ...) {    \
        va_list args;                                       \
        va_start(args, count);                              \
        name res = { count, 16, NULL };                     \
        while (count > res.cap)                             \
            res.cap <<= 1;                                  \
        res.items = (type*)malloc(res.cap * sizeof(type));  \
        for (u i = 0; i < count; i++)                       \
            res.items[i] = (type)va_arg(args, u);           \
        va_end(args);                                       \
        return type##AggregateNew(res);                     \
    }
#define setDefineCompareName(type, listName)                            \
    setDefineName(type, listName);                                      \
    bool type##RangeContains(set(type)* set, type item) {               \
        return (type##LessThan(as(range(type), set)->min, item) || as(range(type), set)->inclMin && as(range(type), set)->min == item) && \
            (type##GreaterThan(as(range(type), set)->max, item) || as(range(type), set)->inclMax && as(range(type), set)->max == item); \
    }                                                                   \
    range(type) type##RangeDefault() {                                  \
        range(type) res = { 0 };                                        \
        res.contains = &type##RangeContains;                            \
        res.inclMin = true;                                             \
        res.inclMax = true;                                             \
        return res;                                                     \
    }                                                                   \
    type##Set* type##RangeNew(type min, type max) {                     \
        set(type)* res = as(set(type), new(range(type)));               \
        *as(range(type), res) = type##RangeDefault();                   \
        as(range(type), res)->min = min;                                \
        as(range(type), res)->max = max;                                \
        as(range(type), res)->inclMin = true;                           \
        as(range(type), res)->inclMax = true;                           \
        return res;                                                     \
    }
#define setDefineDefaultName(type, listName)        \
    setDefineCompareName(type, listName);           \
    bool type##LessThan(type left, type right) {    \
        return left < right;                        \
    }                                               \
    bool type##GreaterThan(type left, type right) { \
        return left > right;                        \
    }
#define setDefine(type) setDefineName(type, type##List)
#define setDefineVaListInt(type) setDefineVaListIntName(type, type##List)
#define setDefineCompare(type) setDefineCompareName(type, type##List)
#define setDefineDefault(type) setDefineDefaultName(type, type##List)

#endif // MCX_SET_H
