/*
 * aiasm-aarch64/modules/sleep/src/main.c
 * sleep WASM module for AI-ASM AArch64 v39.0
 *
 * Usage: sleep SECONDS
 * Supports: integer (5), fractional (1.5)
 */

__attribute__((import_module("host"), import_name("sleep")))
extern void host_sleep(unsigned int ms);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get command line arguments */
    char buf[64];
    int argv_len = host_get_argv((unsigned int)buf, 63);
    if (argv_len < 0) argv_len = 0;

    /* Skip argv[0] ("sleep") */
    int i = 0;
    while (i < argv_len && buf[i] != ' ') i++;
    while (i < argv_len && buf[i] == ' ') i++;

    /* Parse seconds (integer.fraction) */
    unsigned int seconds = 0;
    unsigned int frac = 0;
    while (i < argv_len && buf[i] >= '0' && buf[i] <= '9') {
        seconds = seconds * 10 + (buf[i] - '0');
        i++;
    }
    if (i < argv_len && buf[i] == '.') {
        i++;
        /* Parse up to 3 decimal places as milliseconds */
        unsigned int multiplier = 100;
        int digits = 0;
        while (i < argv_len && buf[i] >= '0' && buf[i] <= '9' && digits < 3) {
            frac += (buf[i] - '0') * multiplier;
            multiplier /= 10;
            digits++;
            i++;
        }
    }

    unsigned int total_ms = seconds * 1000 + frac;
    host_sleep(total_ms);
    host_exit(0);
}
