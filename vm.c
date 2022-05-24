#include "vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

uint64_t bytes_to_int(const char* bytes, uint8_t len) {
    uint64_t result = 0;
    for (uint8_t i = 0; i < len; i++) {
        unsigned char c = bytes[len-i-1];
        result |= (uint64_t)c << 8*i;
    }
    return result;
}

uint8_t arg_bytes_for_tag(OP_Tag tag) {
    switch (tag) {
        case OP_PUSH_BYTE: return 1;
        case OP_PUSH_I32: return 4;
        default: return 0;
    }
}

/// init_vm returns a new VM reference.
///
/// Caller responsible for deallocating VM with vm_deinit.
VM* vm_init(size_t mem_size) {
    VM* vm = (VM*)(calloc(1, sizeof(VM)));
    vm->mem = calloc(mem_size, sizeof(char));
    vm->mem_size = mem_size;
    return vm;
}

void vm_deinit(VM* self) {
    free(self->mem);
    free(self);
}

/// vm_set_program sets the program and instruction pointer of the VM.
///
/// Caller still owns program pointer and needs to free after vm_deinit.
void vm_set_program(VM* self, char* program, size_t program_len, size_t pc) {
    self->program = program;
    self->program_len = program_len;
    self->pc = pc;
}

int vm_next_char(VM* self, char* out_c) {
    if (self->pc >= self->program_len) {
        return 1;
    }
    *out_c = self->program[self->pc];
    self->pc += 1;
    return 0;
}

OP_Err vm_next_op(VM* self, OP* out_op) {
    size_t pc = self->pc; // Gets incremented by vm_next_char

    char c;
    if (vm_next_char(self, &c) != 0) {
        return OPERR_EOF;
    }

    if (c >= OP_INVALID) {
        return OPERR_INVALID_OP;
    }

    OP_Tag tag = (OP_Tag)c;
    size_t arg_size = arg_bytes_for_tag(tag);

    if (pc + arg_size >= self->program_len) {
        return OPERR_NOT_ENOUGH_ARGS;
    }

    *out_op = (OP){ .tag = tag, .arg = {0} };
    if (arg_size != 0) {
        memcpy(out_op->arg, self->program + pc + 1, arg_size);
        self->pc += arg_size;
    }

    return OPERR_OK;
}

int vm_stack_push(VM* self, const char* bytes, size_t count) {
    if (self->stack_len + count > STACK_SIZE) {
        return 1;
    }
    memcpy(self->stack + self->stack_len, bytes, count);
    self->stack_len += count;
    return 0;
}

int vm_stack_pop(VM* self, char* dst, size_t count) {
    if (self->stack_len < count) {
        return 1;
    }
    memcpy(dst, self->stack + self->stack_len - count, count);
    self->stack_len -= count;
    return 0;
}

OP_Err vm_run(VM *self) {
    char buf[4] = {0};
    uint32_t addr;
    OP op;
    OP_Err err;

    while ((err = vm_next_op(self, &op)) == OPERR_OK) {
        switch (op.tag) {
            case OP_NOP:
                break;
            case OP_LOD_BYTE:
                if (vm_stack_pop(self, buf, 4)) {
                    return OPERR_STACK_UNDERFULL;
                }
                addr = bytes_to_int(buf, 4);
                if (addr >= self->mem_size) {
                    return OPERR_READ_OOB;
                }
                if (vm_stack_push(self, &self->mem[addr], 1)) {
                    return OPERR_STACK_OVERFULL;
                }
                break;
            case OP_STO_BYTE:
                if (vm_stack_pop(self, buf, 4)) {
                    return OPERR_STACK_UNDERFULL;
                }
                addr = bytes_to_int(buf, 4);
                if (addr >= self->mem_size) {
                    return OPERR_WRITE_OOB;
                }
                if (vm_stack_pop(self, buf, 1)) {
                    return OPERR_STACK_UNDERFULL;
                }
                self->mem[addr] = buf[0];
                break;
            case OP_PUSH_BYTE:
                if (vm_stack_push(self, op.arg, 1)) {
                    return OPERR_STACK_OVERFULL;
                }
                break;
            case OP_PUSH_I32:
                if (vm_stack_push(self, op.arg, 4)) {
                    return OPERR_STACK_OVERFULL;
                }
                break;
            case OP_PRINT_CHAR: {
                if (vm_stack_pop(self, buf, 1)) {
                    return OPERR_STACK_UNDERFULL;
                }
                putchar(buf[0]);
                break;
            }
            case OP_INVALID:
            default:
                fprintf(stderr, "Unhandled opcode: %x\n", op.tag);

                return OPERR_UNHANDLED_OP;
        }
    }

    return err;
}
