#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _test_bytes_to_int() {
    uint64_t num = 0b1111000011110101;
    const char buf[4] = {0, 0, 0xF0, 0xF5};
    uint64_t result = bytes_to_int(buf, 4);

    if (num != result) {
        fprintf(stderr, "bytes_to_int: expected: %llx, got: %llx\n", num, result);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[argc+1]) {
    if (argc != 2) {
        fprintf(stderr, "test.c: expected one test name\n");
        return EXIT_FAILURE;
    }

    char* test = argv[1];

    if (strcmp(test, "bytes_to_int") == 0)
        return _test_bytes_to_int();
    else {
        fprintf(stderr, "test.c: invalid test name: %s\n", test);
        return EXIT_FAILURE;
    }
}
