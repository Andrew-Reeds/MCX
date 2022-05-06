#ifndef MCX_ASSERT_H
#define MCX_ASSERT_H

#define assert(condition, message, ...) if (condition) fprintf(message, __VA_ARGS__)

#endif // MCX_ASSERT_H
