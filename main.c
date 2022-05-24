#include "vm.h"

#include "stdio.h"

#define PROG_SIZE 3
static char PROGRAM[PROG_SIZE] = {0x7, 'A', 0xA};

int main() {
    VM* vm = vm_init(100);
    vm_set_program(vm, PROGRAM, PROG_SIZE, 0);

    OP_Err err = vm_run(vm);

    if (err != OPERR_OK) {
        switch (err) {
            case OPERR_EOF:
                break;
            case OPERR_INVALID_OP:
                fprintf(stderr, "invalid opcode: %x\n", err);
                break;
            case OPERR_NOT_ENOUGH_ARGS:
                fprintf(stderr, "not enough args for opcode: %x\n", err);
                break;
            case OPERR_UNHANDLED_OP:
                fprintf(stderr, "operation not implemented\n");
                break;
            default:
                fprintf(stderr, "Unhandled error code: %x\n", err);

                vm_deinit(vm);
                return EXIT_FAILURE;
        }
    }

    vm_deinit(vm);
    return EXIT_SUCCESS;
}
