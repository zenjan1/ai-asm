/*
 * aiasm-aarch64/modules/date/src/main.c
 * date WASM module for AI-ASM AArch64 v33.0
 *
 * Usage: date [+FORMAT]
 * Format specifiers: %Y year, %m month, %d day, %H hour, %M minute, %S second, %s epoch
 * Default format: "Mon DD HH:MM:SS YYYY"
 */

__attribute__((import_module("host"), import_name("get_tick")))
extern unsigned long long host_get_tick(void);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int offset, unsigned int len);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* -------------------------------------------------------------------------- */
/* Constants                                                                  */
/* -------------------------------------------------------------------------- */

/* QEMU virt machine CNTFRQ_EL0 = 62.5 MHz */
#define TICK_FREQUENCY  62500000ULL

/* Epoch base for boot time (2026-01-01 00:00:00 UTC) */
#define BOOT_EPOCH_SECS 1767225600ULL

/* -------------------------------------------------------------------------- */
/* String helpers                                                             */
/* -------------------------------------------------------------------------- */

static unsigned int my_strlen(const char *s)
{
    unsigned int len = 0;
    while (s[len]) len++;
    return len;
}

static void my_utoa(unsigned int n, char *buf)
{
    char tmp[16];
    int i = 0;
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (n > 0) {
        tmp[i++] = (char)('0' + (n % 10));
        n /= 10;
    }
    int j = 0;
    while (i > 0)
        buf[j++] = tmp[--i];
    buf[j] = '\0';
}

/* -------------------------------------------------------------------------- */
/* Memory (bump allocator, same pattern as echo module)                       */
/* -------------------------------------------------------------------------- */

static unsigned int heap_pos = 65536;

static unsigned int alloc(unsigned int n)
{
    unsigned int ptr = heap_pos;
    heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u;
    return ptr;
}

static void copy_to_mem(unsigned int dst, const char *src, unsigned int len)
{
    char *d = (char *)dst;
    for (unsigned int i = 0; i < len; i++)
        d[i] = src[i];
}

static void print_str(const char *s)
{
    unsigned int len = my_strlen(s);
    unsigned int off = alloc(len);
    copy_to_mem(off, s, len);
    host_print(off, len);
}

/* -------------------------------------------------------------------------- */
/* Time computation from Unix seconds                                         */
/* -------------------------------------------------------------------------- */

static unsigned int is_leap(unsigned int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static unsigned int days_in_month(unsigned int year, unsigned int month)
{
    static const unsigned char dim[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 1 && is_leap(year)) return 29;
    return dim[month];
}

/* Compute year, month, day, hour, minute, second from Unix seconds */
static void compute_time(unsigned long long total_secs,
                         unsigned int *out_year, unsigned int *out_month,
                         unsigned int *out_day, unsigned int *out_hour,
                         unsigned int *out_min, unsigned int *out_sec,
                         unsigned int *out_wday)
{
    unsigned int year = 1970;
    unsigned long long rem = total_secs;

    *out_sec = (unsigned int)(rem % 60);
    rem /= 60;
    *out_min = (unsigned int)(rem % 60);
    rem /= 60;
    *out_hour = (unsigned int)(rem % 24);
    unsigned long long days = rem / 24;

    /* Day of week: 1970-01-01 = Thursday (4) */
    *out_wday = (unsigned int)((days + 4) % 7);

    while (1) {
        unsigned int ydays = is_leap(year) ? 366 : 365;
        if (days < ydays) break;
        days -= ydays;
        year++;
    }
    *out_year = year;

    unsigned int month = 0;
    while (month < 12) {
        unsigned int md = days_in_month(year, month);
        if (days < md) break;
        days -= md;
        month++;
    }
    *out_month = month + 1;
    *out_day = (unsigned int)days + 1;
}

/* -------------------------------------------------------------------------- */
/* Format output                                                              */
/* -------------------------------------------------------------------------- */

static void append_num(char *buf, unsigned int *pos, unsigned int n, unsigned int width)
{
    char tmp[16];
    my_utoa(n, tmp);
    unsigned int len = my_strlen(tmp);

    /* Zero-pad to width */
    while (len < width) {
        buf[(*pos)++] = '0';
        len++;
    }
    unsigned int i = 0;
    while (tmp[i])
        buf[(*pos)++] = tmp[i++];
}

static void format_date(char *buf, unsigned int *pos, const char *fmt,
                        unsigned int year, unsigned int month, unsigned int day,
                        unsigned int hour, unsigned int min, unsigned int sec,
                        unsigned long long total_secs)
{
    static const char *month_names[] = {
        "Jan","Feb","Mar","Apr","May","Jun",
        "Jul","Aug","Sep","Oct","Nov","Dec"
    };
    static const char *day_names[] = {
        "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
    };

    unsigned int i = 0;
    while (fmt[i]) {
        if (fmt[i] == '%') {
            i++;
            switch (fmt[i]) {
                case 'Y': append_num(buf, pos, year, 4); break;
                case 'm': append_num(buf, pos, month, 2); break;
                case 'd': append_num(buf, pos, day, 2); break;
                case 'H': append_num(buf, pos, hour, 2); break;
                case 'M': append_num(buf, pos, min, 2); break;
                case 'S': append_num(buf, pos, sec, 2); break;
                case 's': {
                    char tmp[32];
                    my_utoa((unsigned int)(total_secs & 0xFFFFFFFF), tmp);
                    unsigned int j = 0;
                    while (tmp[j]) buf[(*pos)++] = tmp[j++];
                    break;
                }
                case 'b': {
                    const char *mn = (month >= 1 && month <= 12)
                        ? month_names[month - 1] : "???";
                    unsigned int j = 0;
                    while (mn[j]) buf[(*pos)++] = mn[j++];
                    break;
                }
                case 'a': {
                    /* We need wday for this - compute it */
                    unsigned long long days_from_epoch = total_secs / 86400;
                    unsigned int wday = (unsigned int)((days_from_epoch + 4) % 7);
                    const char *dn = day_names[wday];
                    unsigned int j = 0;
                    while (dn[j]) buf[(*pos)++] = dn[j++];
                    break;
                }
                case '%': buf[(*pos)++] = '%'; break;
                default: buf[(*pos)++] = '%'; buf[(*pos)++] = fmt[i]; break;
            }
            if (fmt[i]) i++;
        } else {
            buf[(*pos)++] = fmt[i++];
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Entry point                                                                */
/* -------------------------------------------------------------------------- */

__attribute__((export_name("_start")))
void _start(void)
{
    /* Get ticks from hardware counter */
    unsigned long long ticks = host_get_tick();

    /* Convert to seconds since boot */
    unsigned long long secs_since_boot = ticks / TICK_FREQUENCY;

    /* Total Unix seconds */
    unsigned long long total_secs = BOOT_EPOCH_SECS + secs_since_boot;

    /* Compute calendar time */
    unsigned int year, month, day, hour, min, sec, wday;
    compute_time(total_secs, &year, &month, &day, &hour, &min, &sec, &wday);

    /* Get argv to check for format string */
    unsigned int argv_off = alloc(256);
    int argv_len = host_get_argv(argv_off, 255);
    if (argv_len < 0) argv_len = 0;

    const char *argv = (const char *)argv_off;
    const char *fmt = (const char *)0;

    /* Skip "date" argument, look for "+FORMAT" */
    int i = 0;
    while (i < argv_len && argv[i] != ' ') i++;
    while (i < argv_len && argv[i] == ' ') i++;
    if (i < argv_len && argv[i] == '+') {
        i++;  /* skip '+' */
        fmt = &argv[i];
    }

    /* Default format: "Day Mon DD HH:MM:SS YYYY" */
    if (!fmt) {
        static const char *day_names[] = {
            "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
        };
        static const char *month_names[] = {
            "Jan","Feb","Mar","Apr","May","Jun",
            "Jul","Aug","Sep","Oct","Nov","Dec"
        };

        char buf[64];
        unsigned int pos = 0;

        /* Day name */
        const char *dn = day_names[wday];
        while (*dn) buf[pos++] = *dn++;
        buf[pos++] = ' ';

        /* Month name */
        const char *mn = month_names[month - 1];
        while (*mn) buf[pos++] = *mn++;
        buf[pos++] = ' ';

        /* Day */
        append_num(buf, &pos, day, 2);
        buf[pos++] = ' ';

        /* HH:MM:SS */
        append_num(buf, &pos, hour, 2);
        buf[pos++] = ':';
        append_num(buf, &pos, min, 2);
        buf[pos++] = ':';
        append_num(buf, &pos, sec, 2);
        buf[pos++] = ' ';

        /* Year */
        append_num(buf, &pos, year, 4);

        buf[pos++] = '\n';

        unsigned int off = alloc(pos);
        copy_to_mem(off, buf, pos);
        host_print(off, pos);
    } else {
        /* Custom format */
        char buf[128];
        unsigned int pos = 0;
        format_date(buf, &pos, fmt, year, month, day, hour, min, sec, total_secs);
        buf[pos++] = '\n';

        unsigned int off = alloc(pos);
        copy_to_mem(off, buf, pos);
        host_print(off, pos);
    }

    host_exit(0);
}
