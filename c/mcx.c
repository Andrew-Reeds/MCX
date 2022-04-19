#include "../mcx.h"
#include "../console.h"
#include "../file.h"

void init(MCX) {
    init(CONSOLE);
    init(FILE);
}
