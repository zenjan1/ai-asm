/*
 * aiasm-aarch64/kernel/libc_shim.c
 * Minimal freestanding C library shims for wasm3
 */

typedef unsigned long       size_t;
typedef unsigned long       uintptr_t;
typedef long                ptrdiff_t;
typedef long                ssize_t;
#define NULL ((void *)0)

/* -------------------------------------------------------------------------- */
/* String functions                                                           */
/* -------------------------------------------------------------------------- */

size_t strnlen(const char *s, size_t maxlen)
{
    size_t i = 0;
    while (i < maxlen && s[i] != '\0')
        i++;
    return i;
}

unsigned long strtoul(const char *s, char **endptr, int base)
{
    unsigned long result = 0;
    unsigned long digit;

    while (*s == ' ' || *s == '\t')
        s++;

    while (*s) {
        if (*s >= '0' && *s <= '9')
            digit = *s - '0';
        else if (*s >= 'a' && *s <= 'z')
            digit = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'Z')
            digit = *s - 'A' + 10;
        else
            break;

        if (digit >= (unsigned long)base)
            break;

        result = result * (unsigned long)base + digit;
        s++;
    }

    if (endptr)
        *endptr = (char *)s;
    return result;
}

unsigned long long strtoull(const char *s, char **endptr, int base)
{
    unsigned long long result = 0;
    unsigned long long digit;

    while (*s == ' ' || *s == '\t')
        s++;

    while (*s) {
        if (*s >= '0' && *s <= '9')
            digit = *s - '0';
        else if (*s >= 'a' && *s <= 'z')
            digit = *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'Z')
            digit = *s - 'A' + 10;
        else
            break;

        if (digit >= (unsigned long long)base)
            break;

        result = result * (unsigned long long)base + digit;
        s++;
    }

    if (endptr)
        *endptr = (char *)s;
    return result;
}

/* -------------------------------------------------------------------------- */
/* Timer: read CNTVCT_EL0 (ARM Generic Timer virtual count)                   */
/* -------------------------------------------------------------------------- */

static unsigned long long read_cntvct(void)
{
    unsigned long long val;
    __asm__ volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
}

unsigned long clock(void)
{
    return (unsigned long)(read_cntvct() & 0xFFFFFFFFUL);
}

/* -------------------------------------------------------------------------- */
/* PL011 UART output for stdio stubs (UART at 0x09000000)                     */
/* -------------------------------------------------------------------------- */

#define UART_BASE 0x09000000UL
#define UART_DR   (*(volatile unsigned int *)(UART_BASE + 0x000))
#define UART_FR   (*(volatile unsigned int *)(UART_BASE + 0x018))
#define UART_FR_TXFF (1 << 5)

static void uart_putc(char c)
{
    while (UART_FR & UART_FR_TXFF)
        ;
    UART_DR = (unsigned int)c;
}

/* -------------------------------------------------------------------------- */
/* stdio stubs — only pulled in by m3_api_libc.c which we don't invoke        */
/* -------------------------------------------------------------------------- */

typedef struct { int _fd; } FILE;
static FILE _stdin  = { 0 };
static FILE _stdout = { 1 };
static FILE _stderr = { 2 };

FILE *stdin  = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

int fputs(const char *s, FILE *f)
{
    while (*s)
        uart_putc(*s++);
    return 0;
}

int fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f)
{
    size_t total = size * nmemb;
    const char *p = ptr;
    for (size_t i = 0; i < total; i++)
        uart_putc(p[i]);
    return (int)nmemb;
}

int fputc(int c, FILE *f)
{
    uart_putc((char)c);
    return c;
}

int putc(int c, FILE *f)
{
    uart_putc((char)c);
    return c;
}

int fprintf(FILE *f, const char *fmt, ...)
{
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 's' || *fmt == 'd' || *fmt == 'x' || *fmt == 'p' ||
                *fmt == 'c' || *fmt == 'u' || *fmt == 'l' || *fmt == 'z' ||
                *fmt == 'L' || *fmt == 'h') {
                fmt++;
            }
        }
        uart_putc(*fmt++);
    }
    return 0;
}

int fflush(FILE *f)
{
    return 0;
}

FILE *fopen(const char *path, const char *mode)
{
    return NULL;
}
