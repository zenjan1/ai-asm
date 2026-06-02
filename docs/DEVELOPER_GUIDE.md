# AI-ASM OS Developer Guide (v28.0)

## WASM Module Development

### Quick Start

Create a new WASM module:

```c
/* modules/my_module/src/main.c */
#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    host_print("Hello, AI-ASM OS!\n");
    host_exit(0);
}
```

### Compilation

```bash
clang --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
  -Wl,--no-entry -Wl,--export=_start -Wl,--allow-undefined \
  modules/my_module/src/main.c -o modules/my_module/my_module.wasm
```

### Module Structure

```
modules/my_module/
├── src/
│   └── main.c          # Module source code
└── my_module.wasm      # Compiled WASM binary (gitignored)
```

### Standard Module Template

```c
/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strcpy(char *dst, const char *src) { while (*src) *dst++ = *src++; *dst = '\0'; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

/* Print utilities */
static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* CLI entry */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    if (help) { /* show help */ return; }
    if (test) { /* run test */ return; }
}
```

## Kernel Module Development

### Assembly Module Structure

```asm
/* kernel/my_module.asm */
/*
 * Provides:
 *   - feature_1
 *   - feature_2
 */

/* Constants */
.equ MY_CONST, 42

/* BSS section */
.bss
.align 4
my_state: .skip 4

/* Text section */
.text
.globl my_init
.globl my_func

my_init:
    stp x29, x30, [sp, -16]!
    mov x29, sp
    /* initialization */
    ldp x29, x30, [sp], 16
    ret

my_func:
    stp x29, x30, [sp, -16]!
    mov x29, sp
    /* implementation */
    ldp x29, x30, [sp], 16
    ret
```

### Register Convention

| Register | Purpose | Preserved |
|----------|---------|-----------|
| x0-x7 | Arguments/return values | No |
| x8 | Indirect result | No |
| x9-x15 | Caller-saved | No |
| x19-x28 | Callee-saved | Yes |
| x29 | Frame pointer | Yes |
| x30 | Link register | Yes |
| sp | Stack pointer | Yes |

### Adding a Syscall

1. Add syscall number in `kernel/syscall.asm`
2. Implement handler function
3. Add to syscall dispatch table
4. Update API documentation

## Agent Development

### Agent Structure

Agents communicate through message queues and shared memory.

```c
/* Create agent */
int agent_id = host_spawn("agent_module", argv);

/* Send message */
int result = host_msgq_send(agent_id, msg_ptr, msg_len);

/* Receive response */
int result = host_msgq_recv(agent_id, buf_ptr, buf_len);
```

## Testing

### Running Tests

```bash
# Unit tests
make test-unit

# Integration tests
make test-integration

# Security tests
make test-security

# All tests
make test
```

### Writing Tests

Tests are WASM modules with `-t` flag that print pass/fail results.

## Contribution Guidelines

1. Fork the repository
2. Create feature branch
3. Write tests first
4. Implement feature
5. Run full test suite
6. Submit pull request
