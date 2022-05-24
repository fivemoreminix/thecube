#include "vm.h"

#include "stdio.h"

#define PROG_SIZE 15
static char PROGRAM[PROG_SIZE] = {0x07, 'A', 0x08, 0x0, 0x0, 0x0, 0x0, 0x06, 0x08, 0x0, 0x0, 0x0, 0x0, 0x05, 0x0A};

int main(int argc, char* argv[argc+1]) {
    VM* vm = vm_init(100);
    vm_set_program(vm, PROGRAM, PROG_SIZE, 0);

    OP_Err err = vm_run(vm);

    printf("\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", vm->mem[i]);
    }
    printf("\n");

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
