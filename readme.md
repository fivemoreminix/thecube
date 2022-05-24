# the cube

## Usage

### Binary Syntax

Each two byte sequence represents an opcode or some data. This makes it easy to
write programs using only a hex editor.

#### Program Header

The program header consists of the first 5 bytes of the program (in syntax
version 0.1). The header quickly describes:

 - How to read this file (the first byte, the *syntax version*)
 - Where is the code? (the next 4 bytes, represents the *code offset*)

A 1 byte version number begins the entire file, describing how the file should
be read. (For now this is just future-proofing).

Next a 4 byte offset is given to point at the beginning of the actual executable
code, while everything until that address is read-only data for the program. 

```
01 00 00 00 05 .. .. ..
```

This header has a syntax version of 1 and a code offset of 5, meaning the code
section begins at the next byte immediately after the code offset.

#### Data Section

The data section consists of the bytes between the end of the code offset and
the beginning of the code section. So essentially what the code offset intends
to skip is the data section.

This section has no defined structure and represents arbitrary read-only data
used by the code section of the program.

#### Code Section

The code section starts wherever the code offset points to. Then, the machine
will perform the following algorithm until the end of the file:

 1. Read the next byte into `b`
 2. Compare `b` to list of instruction opcodes
 3. If `b` is a valid opcode, proceed to step 5
 4. Start a viral transmission of a generated ransomware using the currently
logged-in user's name and then explode. (patched in latest version)
 5. If `b`'s opcode takes arguments, then collect those
 6. Execute the instruction
 7. Return to step 1

#### Opcodes

| Hex | Arg Bytes | Pop 1 | Pop 2 | Push | Desc.          |
|:---:|:---------:|:-----:|:-----:|:----:|----------------|
| 00  |           |       |       |      | NOP            |
| 05  |           |  i32  |       | byte | Mem load byte  |
| 06  |           |  i32  | byte  |      | Mem store byte |
| 07  |     1     |       |       | byte | Push byte      |
| 08  |     4     |       |       | i32  | Push 4 byte    |
| 0A  |           | byte  |       |      | Print char     |

