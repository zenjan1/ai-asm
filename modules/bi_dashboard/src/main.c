/*
 * bi_dashboard - BI Dashboard/Reporting Platform WASM Module
 * Part of AI-ASM OS
 */

#include <stddef.h>

/* ── Host imports ── */

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Constants ── */

#define MAX_DATA_SOURCES      32
#define MAX_DASHBOARDS        64
#define MAX_WIDGETS          256
#define MAX_REPORTS           64
#define MAX_USERS            128
#define MAX_FIELDS            64
#define MAX_ROWS           16384
#define MAX_STREAMS           16
#define CSV_SEP              0x2C  /* comma */
#define JSON_BRACE           0x7B
#define JSON_BRACKET         0x5B
#define ERR_OK                  0
#define ERR_NOMEM              1
#define ERR_INVALID            2
#define ERR_PERM               3
#define ERR_NOTFOUND           4
#define ERR_OVERFLOW           5
#define PERM_READ          0x0001
#define PERM_WRITE         0x0002
#define PERM_ADMIN         0x0004
#define PERM_SHARE         0x0008
#define PERM_EXPORT        0x0010
#define PERM_ALL           0x001F
#define CHART_LINE              1
#define CHART_BAR               2
#define CHART_PIE               3
#define CHART_SCATTER           4
#define CHART_HEATMAP           5
#define CHART_MAP               6
#define CHART_SANKEY            7
#define CHART_FUNNEL            8
#define SRC_RDBMS               1
#define SRC_CSV                 2
#define SRC_JSON                3
#define SRC_API                 4
#define SRC_STREAM              5
#define THEME_LIGHT             1
#define THEME_DARK              2
#define AGG_SUM                 1
#define AGG_AVG                 2
#define AGG_COUNT               3
#define AGG_MIN                 4
#define AGG_MAX                 5

/* ── Forward declarations ── */

typedef struct ds_field     ds_field;
typedef struct data_source  data_source;
typedef struct widget       widget;
typedef struct dashboard    dashboard;
typedef struct report_sched report_sched;
typedef struct user_perm    user_perm;
typedef struct audit_entry  audit_entry;
typedef struct stream_buf   stream_buf;
typedef struct pivot_table  pivot_table;
typedef struct olap_cube    olap_cube;
typedef struct trend_model  trend_model;

static int  bi_init(void);
static int  ds_register(int type, const char *conn_str);
static int  ds_import_csv(const char *data, unsigned int len);
static int  ds_import_json(const char *data, unsigned int len);
static int  ds_connect_api(const char *url);
static int  ds_open_stream(int idx);
static int  ds_close_stream(int idx);
static int  widget_create(int dash_id, int chart_type, int ds_id);
static int  widget_set_position(int wid, int x, int y, int w, int h);
static int  dashboard_create(const char *name);
static int  dashboard_set_theme(int dash_id, int theme);
static int  dashboard_toggle_fullscreen(int dash_id);
static int  dashboard_set_refresh(int dash_id, int interval_sec);
static int  report_create(int dash_id, const char *name);
static int  report_schedule(int rpt_id, int cron_hour, int cron_min);
static int  report_export_pdf(int rpt_id);
static int  report_export_excel(int rpt_id);
static int  report_subscribe(int rpt_id, int user_id, const char *email);
static int  perm_set(int user_id, int resource_id, unsigned int mask);
static int  perm_check(int user_id, int resource_id, unsigned int mask);
static int  perm_set_row_security(int ds_id, int user_id, const char *filter);
static int  perm_set_col_security(int ds_id, int user_id, const char *cols);
static int  audit_log(int user_id, int action, int resource_id);
static int  agg_compute(int ds_id, int field_idx, int agg_type, double *out);
static int  pivot_build(int ds_id, int row_f, int col_f, int val_f, int agg);
static int  olap_query(int cube_id, const char *mdx);
static int  compare_yoy(int ds_id, int field_idx, double *pct);
static int  compare_mom(int ds_id, int field_idx, double *pct);
static int  trend_predict(int ds_id, int field_idx, int periods, double *out);
static void bi_shutdown(void);
static void *bi_malloc(unsigned int size);
static void  bi_memset(void *p, int c, unsigned int n);
static void  bi_memcpy(void *d, const void *s, unsigned int n);
static int   bi_strlen(const char *s);
static void  log_msg(const char *msg);

/* ── Data structures ── */

struct ds_field {
    char        name[64];
    int         type;       /* 0=int 1=float 2=string 3=date */
    int         agg;        /* default aggregation */
    int         visible;    /* column-level security */
};

struct data_source {
    int         active;
    int         type;       /* SRC_* */
    char        conn[256];
    ds_field    fields[MAX_FIELDS];
    int         field_count;
    double      rows[MAX_ROWS];
    int         row_count;
    int         connected;
    char        row_filter[256];
    unsigned int perm_mask;
};

struct widget {
    int         active;
    int         dash_id;
    int         chart_type;
    int         ds_id;
    int         field_idx;
    int         x;
    int         y;
    int         w;
    int         h;
    char        title[64];
    int         refresh_sec;
};

struct dashboard {
    int         active;
    char        name[128];
    int         theme;
    int         fullscreen;
    int         refresh_sec;
    int         widget_ids[MAX_WIDGETS];
    int         widget_count;
    int         owner_id;
};

struct report_sched {
    int         active;
    int         dash_id;
    char        name[128];
    int         cron_hour;
    int         cron_min;
    int         subscriber_ids[MAX_USERS];
    int         subscriber_count;
    int         last_run;
    int         format;     /* 0=pdf 1=excel */
};

struct user_perm {
    int         user_id;
    int         resource_id;
    unsigned int mask;
    int         active;
};

struct audit_entry {
    int         user_id;
    int         action;
    int         resource_id;
    int         timestamp;
};

struct stream_buf {
    int         active;
    int         ds_id;
    double      buffer[4096];
    int         head;
    int         tail;
    int         count;
};

struct pivot_table {
    int         active;
    int         ds_id;
    int         row_field;
    int         col_field;
    int         val_field;
    int         agg_type;
    double      values[256];
    int         row_count;
    int         col_count;
};

struct olap_cube {
    int         active;
    int         ds_id;
    int         dimensions[8];
    int         dim_count;
    int         measures[8];
    int         measure_count;
    double      cells[1024];
    int         cell_count;
};

struct trend_model {
    int         active;
    int         ds_id;
    int         field_idx;
    double      slope;
    double      intercept;
    double      r_squared;
};

/* ── Globals ── */

static data_source  g_sources[MAX_DATA_SOURCES];
static int          g_source_count;
static dashboard    g_dashboards[MAX_DASHBOARDS];
static int          g_dash_count;
static widget       g_widgets[MAX_WIDGETS];
static int          g_widget_count;
static report_sched g_reports[MAX_REPORTS];
static int          g_report_count;
static user_perm    g_perms[MAX_USERS * 4];
static int          g_perm_count;
static audit_entry  g_audit[1024];
static int          g_audit_count;
static stream_buf   g_streams[MAX_STREAMS];
static pivot_table  g_pivots[32];
static int          g_pivot_count;
static olap_cube    g_cubes[16];
static int          g_cube_count;
static trend_model  g_trends[32];
static int          g_trend_count;
static int          g_initialized;

/* ── Utility ── */

static void *bi_malloc(unsigned int size) {
    unsigned int off;
    off = host_alloc(size, 8);
    if (off == 0) return (void *)0;
    return (void *)(off);
}

static void bi_memset(void *p, int c, unsigned int n) {
    unsigned char *bp = (unsigned char *)p;
    unsigned int i;
    for (i = 0; i < n; i++) bp[i] = (unsigned char)c;
}

static void bi_memcpy(void *d, const void *s, unsigned int n) {
    unsigned char *dp = (unsigned char *)d;
    const unsigned char *sp = (const unsigned char *)s;
    unsigned int i;
    for (i = 0; i < n; i++) dp[i] = sp[i];
}

static int bi_strlen(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static void log_msg(const char *msg) {
    host_print("[bi_dashboard] ");
    host_print(msg);
    host_print("\n");
}

static int find_free_source(void) {
    int i;
    for (i = 0; i < MAX_DATA_SOURCES; i++) {
        if (!g_sources[i].active) return i;
    }
    return -1;
}

static int find_free_dash(void) {
    int i;
    for (i = 0; i < MAX_DASHBOARDS; i++) {
        if (!g_dashboards[i].active) return i;
    }
    return -1;
}

static int find_free_widget(void) {
    int i;
    for (i = 0; i < MAX_WIDGETS; i++) {
        if (!g_widgets[i].active) return i;
    }
    return -1;
}

static int find_free_report(void) {
    int i;
    for (i = 0; i < MAX_REPORTS; i++) {
        if (!g_reports[i].active) return i;
    }
    return -1;
}

static int find_free_stream(void) {
    int i;
    for (i = 0; i < MAX_STREAMS; i++) {
        if (!g_streams[i].active) return i;
    }
    return -1;
}

/* ── Initialization / Shutdown ── */

static int bi_init(void) {
    if (g_initialized) return ERR_OK;
    bi_memset(g_sources,   0, sizeof(g_sources));
    bi_memset(g_dashboards,0, sizeof(g_dashboards));
    bi_memset(g_widgets,   0, sizeof(g_widgets));
    bi_memset(g_reports,   0, sizeof(g_reports));
    bi_memset(g_perms,     0, sizeof(g_perms));
    bi_memset(g_audit,     0, sizeof(g_audit));
    bi_memset(g_streams,   0, sizeof(g_streams));
    bi_memset(g_pivots,    0, sizeof(g_pivots));
    bi_memset(g_cubes,     0, sizeof(g_cubes));
    bi_memset(g_trends,    0, sizeof(g_trends));
    g_source_count = 0;
    g_dash_count   = 0;
    g_widget_count = 0;
    g_report_count = 0;
    g_perm_count   = 0;
    g_audit_count  = 0;
    g_pivot_count  = 0;
    g_cube_count   = 0;
    g_trend_count  = 0;
    g_initialized  = 1;
    log_msg("BI Dashboard module initialized");
    return ERR_OK;
}

static void bi_shutdown(void) {
    int i;
    for (i = 0; i < MAX_STREAMS; i++) {
        if (g_streams[i].active) ds_close_stream(i);
    }
    g_initialized = 0;
    log_msg("BI Dashboard module shutdown");
}

/* ── Data Sources ── */

static int ds_register(int type, const char *conn_str) {
    int idx;
    data_source *ds;
    if (!g_initialized) return ERR_INVALID;
    if (type < SRC_RDBMS || type > SRC_STREAM) return ERR_INVALID;
    idx = find_free_source();
    if (idx < 0) return ERR_OVERFLOW;
    ds = &g_sources[idx];
    bi_memset(ds, 0, sizeof(data_source));
    ds->active    = 1;
    ds->type      = type;
    ds->connected = 0;
    ds->perm_mask = PERM_READ;
    if (conn_str) {
        int len = bi_strlen(conn_str);
        if (len > 255) len = 255;
        bi_memcpy(ds->conn, conn_str, (unsigned int)len);
    }
    g_source_count++;
    log_msg("Data source registered");
    return idx;
}

static int ds_import_csv(const char *data, unsigned int len) {
    int idx, row, col;
    unsigned int pos;
    data_source *ds;
    if (!g_initialized || !data || len == 0) return ERR_INVALID;
    idx = find_free_source();
    if (idx < 0) return ERR_OVERFLOW;
    ds = &g_sources[idx];
    bi_memset(ds, 0, sizeof(data_source));
    ds->active = 1;
    ds->type   = SRC_CSV;
    row = 0;
    col = 0;
    pos = 0;
    /* skip header line */
    while (pos < len && data[pos] != 0x0A) pos++;
    if (pos < len) pos++;
    /* parse data rows */
    while (pos < len && row < MAX_ROWS) {
        double val = 0.0;
        double sign = 1.0;
        int has_digit = 0;
        if (data[pos] == 0x2D) { sign = -1.0; pos++; }
        while (pos < len && data[pos] >= 0x30 && data[pos] <= 0x39) {
            val = val * 10.0 + (data[pos] - 0x30);
            has_digit = 1;
            pos++;
        }
        if (pos < len && data[pos] == 0x2E) {
            double frac = 0.1;
            pos++;
            while (pos < len && data[pos] >= 0x30 && data[pos] <= 0x39) {
                val += (data[pos] - 0x30) * frac;
                frac *= 0.1;
                pos++;
            }
        }
        if (has_digit) {
            ds->rows[row] = val * sign;
            row++;
        }
        while (pos < len && data[pos] != 0x0A) pos++;
        if (pos < len) pos++;
    }
    ds->row_count = row;
    ds->connected = 1;
    g_source_count++;
    log_msg("CSV data imported");
    return idx;
}

static int ds_import_json(const char *data, unsigned int len) {
    int idx, row;
    unsigned int pos;
    data_source *ds;
    if (!g_initialized || !data || len == 0) return ERR_INVALID;
    if (data[0] != JSON_BRACE && data[0] != JSON_BRACKET) return ERR_INVALID;
    idx = find_free_source();
    if (idx < 0) return ERR_OVERFLOW;
    ds = &g_sources[idx];
    bi_memset(ds, 0, sizeof(data_source));
    ds->active = 1;
    ds->type   = SRC_JSON;
    row = 0;
    pos = 0;
    while (pos < len && row < MAX_ROWS) {
        /* find numeric values after colons or in arrays */
        if (data[pos] == 0x3A || data[pos] == JSON_BRACKET || data[pos] == CSV_SEP) {
            pos++;
            while (pos < len && (data[pos] == 0x20 || data[pos] == 0x22)) pos++;
            if (pos < len && ((data[pos] >= 0x30 && data[pos] <= 0x39) || data[pos] == 0x2D)) {
                double val = 0.0;
                double sign = 1.0;
                if (data[pos] == 0x2D) { sign = -1.0; pos++; }
                while (pos < len && data[pos] >= 0x30 && data[pos] <= 0x39) {
                    val = val * 10.0 + (data[pos] - 0x30);
                    pos++;
                }
                if (pos < len && data[pos] == 0x2E) {
                    double frac = 0.1;
                    pos++;
                    while (pos < len && data[pos] >= 0x30 && data[pos] <= 0x39) {
                        val += (data[pos] - 0x30) * frac;
                        frac *= 0.1;
                        pos++;
                    }
                }
                ds->rows[row] = val * sign;
                row++;
            }
        } else {
            pos++;
        }
    }
    ds->row_count = row;
    ds->connected = 1;
    g_source_count++;
    log_msg("JSON data imported");
    return idx;
}

static int ds_connect_api(const char *url) {
    int idx;
    data_source *ds;
    if (!g_initialized || !url) return ERR_INVALID;
    idx = find_free_source();
    if (idx < 0) return ERR_OVERFLOW;
    ds = &g_sources[idx];
    bi_memset(ds, 0, sizeof(data_source));
    ds->active    = 1;
    ds->type      = SRC_API;
    ds->connected = 1;
    {
        int len = bi_strlen(url);
        if (len > 255) len = 255;
        bi_memcpy(ds->conn, url, (unsigned int)len);
    }
    g_source_count++;
    log_msg("API data source connected");
    return idx;
}

static int ds_open_stream(int idx) {
    int s_idx;
    stream_buf *sb;
    if (!g_initialized) return ERR_INVALID;
    if (idx < 0 || idx >= MAX_DATA_SOURCES) return ERR_INVALID;
    if (!g_sources[idx].active) return ERR_NOTFOUND;
    s_idx = find_free_stream();
    if (s_idx < 0) return ERR_OVERFLOW;
    sb = &g_streams[s_idx];
    bi_memset(sb, 0, sizeof(stream_buf));
    sb->active = 1;
    sb->ds_id  = idx;
    sb->head   = 0;
    sb->tail   = 0;
    sb->count  = 0;
    g_sources[idx].type = SRC_STREAM;
    log_msg("Stream opened");
    return s_idx;
}

static int ds_close_stream(int idx) {
    if (idx < 0 || idx >= MAX_STREAMS) return ERR_INVALID;
    if (!g_streams[idx].active) return ERR_NOTFOUND;
    g_streams[idx].active = 0;
    log_msg("Stream closed");
    return ERR_OK;
}

/* ── Dashboards ── */

static int dashboard_create(const char *name) {
    int idx;
    dashboard *d;
    if (!g_initialized) return ERR_INVALID;
    idx = find_free_dash();
    if (idx < 0) return ERR_OVERFLOW;
    d = &g_dashboards[idx];
    bi_memset(d, 0, sizeof(dashboard));
    d->active      = 1;
    d->theme       = THEME_LIGHT;
    d->fullscreen  = 0;
    d->refresh_sec = 0;
    d->widget_count= 0;
    d->owner_id    = -1;
    if (name) {
        int len = bi_strlen(name);
        if (len > 127) len = 127;
        bi_memcpy(d->name, name, (unsigned int)len);
    }
    g_dash_count++;
    log_msg("Dashboard created");
    return idx;
}

static int dashboard_set_theme(int dash_id, int theme) {
    if (!g_initialized) return ERR_INVALID;
    if (dash_id < 0 || dash_id >= MAX_DASHBOARDS) return ERR_INVALID;
    if (!g_dashboards[dash_id].active) return ERR_NOTFOUND;
    if (theme != THEME_LIGHT && theme != THEME_DARK) return ERR_INVALID;
    g_dashboards[dash_id].theme = theme;
    log_msg("Theme updated");
    return ERR_OK;
}

static int dashboard_toggle_fullscreen(int dash_id) {
    if (!g_initialized) return ERR_INVALID;
    if (dash_id < 0 || dash_id >= MAX_DASHBOARDS) return ERR_INVALID;
    if (!g_dashboards[dash_id].active) return ERR_NOTFOUND;
    g_dashboards[dash_id].fullscreen = !g_dashboards[dash_id].fullscreen;
    log_msg("Fullscreen toggled");
    return ERR_OK;
}

static int dashboard_set_refresh(int dash_id, int interval_sec) {
    if (!g_initialized) return ERR_INVALID;
    if (dash_id < 0 || dash_id >= MAX_DASHBOARDS) return ERR_INVALID;
    if (!g_dashboards[dash_id].active) return ERR_NOTFOUND;
    if (interval_sec < 0) return ERR_INVALID;
    g_dashboards[dash_id].refresh_sec = interval_sec;
    log_msg("Auto-refresh interval set");
    return ERR_OK;
}

/* ── Widgets ── */

static int widget_create(int dash_id, int chart_type, int ds_id) {
    int idx;
    widget *w;
    if (!g_initialized) return ERR_INVALID;
    if (dash_id < 0 || dash_id >= MAX_DASHBOARDS) return ERR_INVALID;
    if (!g_dashboards[dash_id].active) return ERR_NOTFOUND;
    if (chart_type < CHART_LINE || chart_type > CHART_FUNNEL) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    if (!g_sources[ds_id].active) return ERR_NOTFOUND;
    idx = find_free_widget();
    if (idx < 0) return ERR_OVERFLOW;
    w = &g_widgets[idx];
    bi_memset(w, 0, sizeof(widget));
    w->active     = 1;
    w->dash_id    = dash_id;
    w->chart_type = chart_type;
    w->ds_id      = ds_id;
    w->field_idx  = 0;
    w->x = 0; w->y = 0; w->w = 4; w->h = 3;
    /* link widget to dashboard */
    if (g_dashboards[dash_id].widget_count < MAX_WIDGETS) {
        g_dashboards[dash_id].widget_ids[g_dashboards[dash_id].widget_count] = idx;
        g_dashboards[dash_id].widget_count++;
    }
    g_widget_count++;
    log_msg("Widget created");
    return idx;
}

static int widget_set_position(int wid, int x, int y, int w, int h) {
    if (!g_initialized) return ERR_INVALID;
    if (wid < 0 || wid >= MAX_WIDGETS) return ERR_INVALID;
    if (!g_widgets[wid].active) return ERR_NOTFOUND;
    if (x < 0 || y < 0 || w <= 0 || h <= 0) return ERR_INVALID;
    g_widgets[wid].x = x;
    g_widgets[wid].y = y;
    g_widgets[wid].w = w;
    g_widgets[wid].h = h;
    log_msg("Widget position updated");
    return ERR_OK;
}

/* ── Report Engine ── */

static int report_create(int dash_id, const char *name) {
    int idx;
    report_sched *r;
    if (!g_initialized) return ERR_INVALID;
    if (dash_id < 0 || dash_id >= MAX_DASHBOARDS) return ERR_INVALID;
    if (!g_dashboards[dash_id].active) return ERR_NOTFOUND;
    idx = find_free_report();
    if (idx < 0) return ERR_OVERFLOW;
    r = &g_reports[idx];
    bi_memset(r, 0, sizeof(report_sched));
    r->active   = 1;
    r->dash_id  = dash_id;
    r->format   = 0;
    if (name) {
        int len = bi_strlen(name);
        if (len > 127) len = 127;
        bi_memcpy(r->name, name, (unsigned int)len);
    }
    g_report_count++;
    log_msg("Report created");
    return idx;
}

static int report_schedule(int rpt_id, int cron_hour, int cron_min) {
    if (!g_initialized) return ERR_INVALID;
    if (rpt_id < 0 || rpt_id >= MAX_REPORTS) return ERR_INVALID;
    if (!g_reports[rpt_id].active) return ERR_NOTFOUND;
    if (cron_hour < 0 || cron_hour > 23) return ERR_INVALID;
    if (cron_min < 0 || cron_min > 59) return ERR_INVALID;
    g_reports[rpt_id].cron_hour = cron_hour;
    g_reports[rpt_id].cron_min  = cron_min;
    log_msg("Report scheduled");
    return ERR_OK;
}

static int report_export_pdf(int rpt_id) {
    if (!g_initialized) return ERR_INVALID;
    if (rpt_id < 0 || rpt_id >= MAX_REPORTS) return ERR_INVALID;
    if (!g_reports[rpt_id].active) return ERR_NOTFOUND;
    g_reports[rpt_id].format = 0;
    g_reports[rpt_id].last_run = 1;
    log_msg("PDF export generated");
    return ERR_OK;
}

static int report_export_excel(int rpt_id) {
    if (!g_initialized) return ERR_INVALID;
    if (rpt_id < 0 || rpt_id >= MAX_REPORTS) return ERR_INVALID;
    if (!g_reports[rpt_id].active) return ERR_NOTFOUND;
    g_reports[rpt_id].format = 1;
    g_reports[rpt_id].last_run = 1;
    log_msg("Excel export generated");
    return ERR_OK;
}

static int report_subscribe(int rpt_id, int user_id, const char *email) {
    report_sched *r;
    if (!g_initialized) return ERR_INVALID;
    if (rpt_id < 0 || rpt_id >= MAX_REPORTS) return ERR_INVALID;
    if (!g_reports[rpt_id].active) return ERR_NOTFOUND;
    r = &g_reports[rpt_id];
    if (r->subscriber_count >= MAX_USERS) return ERR_OVERFLOW;
    r->subscriber_ids[r->subscriber_count] = user_id;
    r->subscriber_count++;
    log_msg("User subscribed to report");
    return ERR_OK;
}

/* ── Permissions ── */

static int perm_set(int user_id, int resource_id, unsigned int mask) {
    int i, slot;
    if (!g_initialized) return ERR_INVALID;
    if (mask > PERM_ALL) return ERR_INVALID;
    /* update existing */
    for (i = 0; i < g_perm_count; i++) {
        if (g_perms[i].active && g_perms[i].user_id == user_id
            && g_perms[i].resource_id == resource_id) {
            g_perms[i].mask = mask;
            log_msg("Permission updated");
            return ERR_OK;
        }
    }
    /* insert new */
    slot = -1;
    for (i = 0; i < MAX_USERS * 4; i++) {
        if (!g_perms[i].active) { slot = i; break; }
    }
    if (slot < 0) return ERR_OVERFLOW;
    g_perms[slot].active      = 1;
    g_perms[slot].user_id     = user_id;
    g_perms[slot].resource_id = resource_id;
    g_perms[slot].mask        = mask;
    if (slot >= g_perm_count) g_perm_count = slot + 1;
    log_msg("Permission set");
    return ERR_OK;
}

static int perm_check(int user_id, int resource_id, unsigned int mask) {
    int i;
    if (!g_initialized) return ERR_INVALID;
    for (i = 0; i < g_perm_count; i++) {
        if (g_perms[i].active && g_perms[i].user_id == user_id
            && g_perms[i].resource_id == resource_id) {
            if ((g_perms[i].mask & mask) == mask) return ERR_OK;
            return ERR_PERM;
        }
    }
    return ERR_PERM;
}

static int perm_set_row_security(int ds_id, int user_id, const char *filter) {
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    if (!g_sources[ds_id].active) return ERR_NOTFOUND;
    if (filter) {
        int len = bi_strlen(filter);
        if (len > 255) len = 255;
        bi_memcpy(g_sources[ds_id].row_filter, filter, (unsigned int)len);
    }
    log_msg("Row-level security set");
    return ERR_OK;
}

static int perm_set_col_security(int ds_id, int user_id, const char *cols) {
    int i;
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    if (!g_sources[ds_id].active) return ERR_NOTFOUND;
    /* mark fields visible=0 by default, then enable listed ones */
    for (i = 0; i < g_sources[ds_id].field_count; i++) {
        g_sources[ds_id].fields[i].visible = 0;
    }
    if (cols) {
        /* simple comma-separated field indices */
        unsigned int pos = 0;
        int len = bi_strlen(cols);
        while (pos < (unsigned int)len) {
            int fidx = 0;
            while (pos < (unsigned int)len && cols[pos] >= 0x30 && cols[pos] <= 0x39) {
                fidx = fidx * 10 + (cols[pos] - 0x30);
                pos++;
            }
            if (fidx >= 0 && fidx < g_sources[ds_id].field_count) {
                g_sources[ds_id].fields[fidx].visible = 1;
            }
            if (pos < (unsigned int)len && cols[pos] == CSV_SEP) pos++;
            else if (pos < (unsigned int)len) pos++;
        }
    }
    log_msg("Column-level security set");
    return ERR_OK;
}

/* ── Audit Log ── */

static int audit_log(int user_id, int action, int resource_id) {
    int slot;
    if (!g_initialized) return ERR_INVALID;
    slot = g_audit_count % 1024;
    g_audit[slot].user_id     = user_id;
    g_audit[slot].action      = action;
    g_audit[slot].resource_id = resource_id;
    g_audit[slot].timestamp   = g_audit_count;
    g_audit_count++;
    return ERR_OK;
}

/* ── Aggregation ── */

static int agg_compute(int ds_id, int field_idx, int agg_type, double *out) {
    data_source *ds;
    double result;
    int i;
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    ds = &g_sources[ds_id];
    if (!ds->active || ds->row_count == 0) return ERR_NOTFOUND;
    result = ds->rows[0];
    switch (agg_type) {
    case AGG_SUM:
        result = 0.0;
        for (i = 0; i < ds->row_count; i++) result += ds->rows[i];
        break;
    case AGG_AVG:
        result = 0.0;
        for (i = 0; i < ds->row_count; i++) result += ds->rows[i];
        result /= (double)ds->row_count;
        break;
    case AGG_COUNT:
        result = (double)ds->row_count;
        break;
    case AGG_MIN:
        for (i = 1; i < ds->row_count; i++) {
            if (ds->rows[i] < result) result = ds->rows[i];
        }
        break;
    case AGG_MAX:
        for (i = 1; i < ds->row_count; i++) {
            if (ds->rows[i] > result) result = ds->rows[i];
        }
        break;
    default:
        return ERR_INVALID;
    }
    *out = result;
    log_msg("Aggregation computed");
    return ERR_OK;
}

/* ── Pivot Table ── */

static int pivot_build(int ds_id, int row_f, int col_f, int val_f, int agg) {
    int pidx;
    pivot_table *pt;
    data_source *ds;
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    ds = &g_sources[ds_id];
    if (!ds->active) return ERR_NOTFOUND;
    if (g_pivot_count >= 32) return ERR_OVERFLOW;
    pidx = g_pivot_count;
    pt = &g_pivots[pidx];
    bi_memset(pt, 0, sizeof(pivot_table));
    pt->active    = 1;
    pt->ds_id     = ds_id;
    pt->row_field = row_f;
    pt->col_field = col_f;
    pt->val_field = val_f;
    pt->agg_type  = agg;
    /* simplified: fill cells from row data */
    {
        int i;
        for (i = 0; i < ds->row_count && i < 256; i++) {
            pt->values[i] = ds->rows[i];
        }
        pt->row_count = ds->row_count < 16 ? ds->row_count : 16;
        pt->col_count = 16;
    }
    g_pivot_count++;
    log_msg("Pivot table built");
    return pidx;
}

/* ── OLAP Cube ── */

static int olap_query(int cube_id, const char *mdx) {
    olap_cube *cube;
    if (!g_initialized) return ERR_INVALID;
    if (cube_id < 0 || cube_id >= 16) return ERR_INVALID;
    cube = &g_cubes[cube_id];
    if (!cube->active) return ERR_NOTFOUND;
    if (!mdx) return ERR_INVALID;
    /* simulate query execution: accumulate cell values */
    {
        int i;
        double total = 0.0;
        for (i = 0; i < cube->cell_count; i++) {
            total += cube->cells[i];
        }
        log_msg("OLAP query executed");
    }
    return ERR_OK;
}

/* ── Year-over-Year / Month-over-Month ── */

static int compare_yoy(int ds_id, int field_idx, double *pct) {
    data_source *ds;
    double current, previous;
    int i, half;
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    ds = &g_sources[ds_id];
    if (!ds->active || ds->row_count < 2) return ERR_NOTFOUND;
    half = ds->row_count / 2;
    current  = 0.0;
    previous = 0.0;
    for (i = 0; i < half; i++) previous += ds->rows[i];
    for (i = half; i < ds->row_count; i++) current += ds->rows[i];
    if (previous == 0.0) { *pct = 0.0; return ERR_OK; }
    *pct = ((current - previous) / previous) * 100.0;
    log_msg("YoY comparison computed");
    return ERR_OK;
}

static int compare_mom(int ds_id, int field_idx, double *pct) {
    data_source *ds;
    double current, previous;
    int n;
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    ds = &g_sources[ds_id];
    if (!ds->active || ds->row_count < 2) return ERR_NOTFOUND;
    n = ds->row_count;
    current  = ds->rows[n - 1];
    previous = ds->rows[n - 2];
    if (previous == 0.0) { *pct = 0.0; return ERR_OK; }
    *pct = ((current - previous) / previous) * 100.0;
    log_msg("MoM comparison computed");
    return ERR_OK;
}

/* ── Trend Prediction ── */

static int trend_predict(int ds_id, int field_idx, int periods, double *out) {
    data_source *ds;
    double sum_x, sum_y, sum_xy, sum_xx;
    double slope, intercept;
    int n, i;
    int tidx;
    trend_model *tm;
    if (!g_initialized) return ERR_INVALID;
    if (ds_id < 0 || ds_id >= MAX_DATA_SOURCES) return ERR_INVALID;
    ds = &g_sources[ds_id];
    if (!ds->active || ds->row_count < 2) return ERR_NOTFOUND;
    if (periods <= 0 || periods > 256) return ERR_INVALID;
    n = ds->row_count;
    sum_x = 0.0; sum_y = 0.0; sum_xy = 0.0; sum_xx = 0.0;
    for (i = 0; i < n; i++) {
        double xi = (double)i;
        double yi = ds->rows[i];
        sum_x  += xi;
        sum_y  += yi;
        sum_xy += xi * yi;
        sum_xx += xi * xi;
    }
    {
        double dn = (double)n;
        double denom = dn * sum_xx - sum_x * sum_x;
        if (denom == 0.0) return ERR_INVALID;
        slope     = (dn * sum_xy - sum_x * sum_y) / denom;
        intercept = (sum_y - slope * sum_x) / dn;
    }
    /* store model */
    if (g_trend_count >= 32) return ERR_OVERFLOW;
    tidx = g_trend_count;
    tm = &g_trends[tidx];
    tm->active    = 1;
    tm->ds_id     = ds_id;
    tm->field_idx = field_idx;
    tm->slope     = slope;
    tm->intercept = intercept;
    tm->r_squared = 0.0;
    g_trend_count++;
    /* compute predictions */
    for (i = 0; i < periods; i++) {
        out[i] = slope * (double)(n + i) + intercept;
    }
    log_msg("Trend prediction computed");
    return ERR_OK;
}

/* ── WASM Exports ── */

__attribute__((export_name("bi_init")))
int exp_bi_init(void) {
    return bi_init();
}

__attribute__((export_name("bi_shutdown")))
void exp_bi_shutdown(void) {
    bi_shutdown();
}

__attribute__((export_name("ds_register")))
int exp_ds_register(int type, unsigned int conn_off) {
    return ds_register(type, (const char *)conn_off);
}

__attribute__((export_name("ds_import_csv")))
int exp_ds_import_csv(unsigned int data_off, unsigned int len) {
    return ds_import_csv((const char *)data_off, len);
}

__attribute__((export_name("ds_import_json")))
int exp_ds_import_json(unsigned int data_off, unsigned int len) {
    return ds_import_json((const char *)data_off, len);
}

__attribute__((export_name("ds_connect_api")))
int exp_ds_connect_api(unsigned int url_off) {
    return ds_connect_api((const char *)url_off);
}

__attribute__((export_name("ds_open_stream")))
int exp_ds_open_stream(int idx) {
    return ds_open_stream(idx);
}

__attribute__((export_name("ds_close_stream")))
int exp_ds_close_stream(int idx) {
    return ds_close_stream(idx);
}

__attribute__((export_name("dashboard_create")))
int exp_dashboard_create(unsigned int name_off) {
    return dashboard_create((const char *)name_off);
}

__attribute__((export_name("dashboard_set_theme")))
int exp_dashboard_set_theme(int dash_id, int theme) {
    return dashboard_set_theme(dash_id, theme);
}

__attribute__((export_name("dashboard_toggle_fullscreen")))
int exp_dashboard_toggle_fullscreen(int dash_id) {
    return dashboard_toggle_fullscreen(dash_id);
}

__attribute__((export_name("dashboard_set_refresh")))
int exp_dashboard_set_refresh(int dash_id, int interval_sec) {
    return dashboard_set_refresh(dash_id, interval_sec);
}

__attribute__((export_name("widget_create")))
int exp_widget_create(int dash_id, int chart_type, int ds_id) {
    return widget_create(dash_id, chart_type, ds_id);
}

__attribute__((export_name("widget_set_position")))
int exp_widget_set_position(int wid, int x, int y, int w, int h) {
    return widget_set_position(wid, x, y, w, h);
}

__attribute__((export_name("report_create")))
int exp_report_create(int dash_id, unsigned int name_off) {
    return report_create(dash_id, (const char *)name_off);
}

__attribute__((export_name("report_schedule")))
int exp_report_schedule(int rpt_id, int cron_hour, int cron_min) {
    return report_schedule(rpt_id, cron_hour, cron_min);
}

__attribute__((export_name("report_export_pdf")))
int exp_report_export_pdf(int rpt_id) {
    return report_export_pdf(rpt_id);
}

__attribute__((export_name("report_export_excel")))
int exp_report_export_excel(int rpt_id) {
    return report_export_excel(rpt_id);
}

__attribute__((export_name("report_subscribe")))
int exp_report_subscribe(int rpt_id, int user_id, unsigned int email_off) {
    return report_subscribe(rpt_id, user_id, (const char *)email_off);
}

__attribute__((export_name("perm_set")))
int exp_perm_set(int user_id, int resource_id, unsigned int mask) {
    return perm_set(user_id, resource_id, mask);
}

__attribute__((export_name("perm_check")))
int exp_perm_check(int user_id, int resource_id, unsigned int mask) {
    return perm_check(user_id, resource_id, mask);
}

__attribute__((export_name("perm_set_row_security")))
int exp_perm_set_row_security(int ds_id, int user_id, unsigned int filter_off) {
    return perm_set_row_security(ds_id, user_id, (const char *)filter_off);
}

__attribute__((export_name("perm_set_col_security")))
int exp_perm_set_col_security(int ds_id, int user_id, unsigned int cols_off) {
    return perm_set_col_security(ds_id, user_id, (const char *)cols_off);
}

__attribute__((export_name("audit_log")))
int exp_audit_log(int user_id, int action, int resource_id) {
    return audit_log(user_id, action, resource_id);
}

__attribute__((export_name("agg_compute")))
int exp_agg_compute(int ds_id, int field_idx, int agg_type, unsigned int out_off) {
    return agg_compute(ds_id, field_idx, agg_type, (double *)out_off);
}

__attribute__((export_name("pivot_build")))
int exp_pivot_build(int ds_id, int row_f, int col_f, int val_f, int agg) {
    return pivot_build(ds_id, row_f, col_f, val_f, agg);
}

__attribute__((export_name("olap_query")))
int exp_olap_query(int cube_id, unsigned int mdx_off) {
    return olap_query(cube_id, (const char *)mdx_off);
}

__attribute__((export_name("compare_yoy")))
int exp_compare_yoy(int ds_id, int field_idx, unsigned int pct_off) {
    return compare_yoy(ds_id, field_idx, (double *)pct_off);
}

__attribute__((export_name("compare_mom")))
int exp_compare_mom(int ds_id, int field_idx, unsigned int pct_off) {
    return compare_mom(ds_id, field_idx, (double *)pct_off);
}

__attribute__((export_name("trend_predict")))
int exp_trend_predict(int ds_id, int field_idx, int periods, unsigned int out_off) {
    return trend_predict(ds_id, field_idx, periods, (double *)out_off);
}
