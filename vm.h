#ifndef THECUBE_VM_H
#define THECUBE_VM_H

#include <stdlib.h>
#include <stdbool.h>

#define STACK_SIZE 256

typedef enum {
    OP_NOP = 0x0,
    OP_LOD_BYTE = 0x5,
    OP_STO_BYTE,
    OP_PUSH_BYTE,
    OP_PUSH_I32,
    OP_PRINT_CHAR = 0xA,
    OP_INVALID, // Must always be last value in enum
} OP_Tag;

inline uint8_t arg_bytes_for_tag(OP_Tag tag);

typedef struct {
    OP_Tag tag;
    char arg[4];
} OP;

typedef enum {
    OPERR_OK = 0,
    OPERR_EOF,
    OPERR_INVALID_OP,
    OPERR_NOT_ENOUGH_ARGS,
    OPERR_UNHANDLED_OP,
    OPERR_READ_OOB,
    OPERR_WRITE_OOB,
} OP_Err;

typedef struct {
    char* mem;
    size_t mem_size;
    char* program;
    size_t program_len;
    size_t pc; // instruction pointer
    char stack[STACK_SIZE];
    size_t stack_len;
} VM;

VM* vm_init(size_t mem_size);
void vm_deinit(VM* self);
void vm_set_program(VM* self, char* program, size_t program_len, size_t pc);
OP_Err vm_next_op(VM* self, OP* op_out);
int vm_stack_push(VM* self, const char* bytes, size_t count);
int vm_stack_pop(VM* self, char* dst, size_t count);
OP_Err vm_run(VM* self);

uint64_t bytes_to_int(const char* bytes, uint8_t len);


#endif //THECUBE_VM_H
