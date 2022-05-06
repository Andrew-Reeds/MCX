#ifndef MCX_ASSERT_H
#define MCX_ASSERT_H

#define assert(condition, message...) if (!(condition)) fprintf(stderr, message)

#endif // MCX_ASSERT_H
