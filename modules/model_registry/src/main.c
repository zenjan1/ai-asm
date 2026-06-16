#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);

__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);

__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* ── Constants ──────────────────────────────────────────────────────────── */
#define STAGE_NONE       0
#define STAGE_STAGING    1
#define STAGE_PRODUCTION 2
#define STAGE_ARCHIVED   3

#define FW_PYTORCH 0
#define FW_TF      1
#define FW_ONNX    2

#define METRIC_ACCURACY  0
#define METRIC_LOSS      1
#define METRIC_F1        2
#define METRIC_PRECISION 3
#define METRIC_RECALL    4
#define METRIC_AUC       5

#define APPROVAL_PENDING  0
#define APPROVAL_APPROVED 1
#define APPROVAL_REJECTED 2

#define STATUS_OK      0
#define STATUS_ERR    -1
#define STATUS_NOTFOUND -2

#define MAX_MODELS      64
#define MAX_VERSIONS    32
#define MAX_METRICS     16
#define MAX_ALIASES     16
#define MAX_DEPS        16
#define MAX_NAME_LEN    64
#define MAX_DESC_LEN   128

/* ── Struct Definitions ─────────────────────────────────────────────────── */

typedef struct {
    int type;
    int value_i;
} metric_val_t;

typedef struct {
    char name[MAX_NAME_LEN];
    char dataset[MAX_NAME_LEN];
    int  framework;
    int  num_params;
} lineage_t;

typedef struct {
    char version[MAX_NAME_LEN];
    int  stage;
    int  num_metrics;
    metric_val_t metrics[MAX_METRICS];
    lineage_t lineage;
    int  approved;
    int  download_count;
    int  cached;
    int  hot_loaded;
} model_version_t;

typedef struct {
    char name[MAX_NAME_LEN];
    char desc[MAX_DESC_LEN];
    int  num_versions;
    model_version_t versions[MAX_VERSIONS];
    int  active_version;
} model_t;

typedef struct {
    char alias[MAX_NAME_LEN];
    char version[MAX_NAME_LEN];
} alias_t;

typedef struct {
    char model_a[MAX_NAME_LEN];
    char model_b[MAX_NAME_LEN];
    int  metric_type;
    int  threshold_pct;
    int  traffic_split_a;
    int  traffic_split_b;
} ab_config_t;

typedef struct {
    int  request_id;
    char model[MAX_NAME_LEN];
    char version[MAX_NAME_LEN];
    int  status;
    char reviewer[MAX_NAME_LEN];
} approval_t;

typedef struct {
    char from_model[MAX_NAME_LEN];
    char to_model[MAX_NAME_LEN];
    int  compat;
} dependency_t;

typedef struct {
    int  num_models;
    model_t models[MAX_MODELS];
    int  num_aliases;
    alias_t aliases[MAX_ALIASES];
    int  num_ab_configs;
    ab_config_t ab_configs[MAX_ALIASES];
    int  num_approvals;
    approval_t approvals[MAX_VERSIONS];
    int  num_deps;
    dependency_t deps[MAX_DEPS];
    int  auto_approve_enabled;
    int  next_request_id;
} registry_t;

/* ── Forward Declarations ───────────────────────────────────────────────── */

/* String utilities */
static int  str_eq(const char *a, const char *b);
static int  str_len(const char *s);
static void str_cpy(char *dst, const char *src, int max);
static void str_cat(char *dst, const char *src, int max);
static void int_to_str(int val, char *buf, int max);
static void print_str(const char *s);
static void print_int(int v);
static void print_line(const char *s);
static void print_sep(void);

/* Memory utilities */
static void *mem_alloc(unsigned int size);
static void mem_zero(void *ptr, unsigned int size);

/* Registry init */
static void registry_init(registry_t *reg);

/* Model functions */
static int model_register(registry_t *reg, const char *name, const char *desc);
static int model_list(registry_t *reg);
static int model_delete(registry_t *reg, const char *name);
static model_t *model_find(registry_t *reg, const char *name);

/* Version functions */
static int version_create(registry_t *reg, const char *model, const char *ver);
static int version_list(registry_t *reg, const char *model);
static model_version_t *version_get(registry_t *reg, const char *model, const char *ver);
static int version_delete(registry_t *reg, const char *model, const char *ver);

/* Stage functions */
static int stage_transition(registry_t *reg, const char *model, const char *ver, int new_stage);
static const char *stage_name(int stage);

/* Alias functions */
static int alias_create(registry_t *reg, const char *alias, const char *ver);
static const char *alias_resolve(registry_t *reg, const char *alias);
static int alias_delete(registry_t *reg, const char *alias);

/* Metric functions */
static int metric_record(registry_t *reg, const char *model, const char *ver, int type, int value);
static int metric_get(registry_t *reg, const char *model, const char *ver, int type);
static int metric_list(registry_t *reg, const char *model, const char *ver);

/* Lineage functions */
static int lineage_track(registry_t *reg, const char *model, const char *ver, const char *dataset, int fw, int params);
static int lineage_dataset(registry_t *reg, const char *model, const char *ver);
static int lineage_params(registry_t *reg, const char *model, const char *ver);
static int lineage_framework(registry_t *reg, const char *model, const char *ver);

/* Compare functions */
static int compare_models(registry_t *reg, const char *model_a, const char *ver_a, const char *model_b, const char *ver_b);
static int compare_ab_config(registry_t *reg, const char *ma, const char *mb, int metric, int split_a, int split_b);
static int compare_benchmark(registry_t *reg, const char *model, const char *ver);

/* Approval functions */
static int approval_request(registry_t *reg, const char *model, const char *ver);
static int approval_approve(registry_t *reg, int req_id, const char *reviewer);
static int approval_reject(registry_t *reg, int req_id, const char *reviewer);
static int approval_auto_rule(registry_t *reg, int enabled);
static int approval_history(registry_t *reg);
static int approval_rollback(registry_t *reg, const char *model, const char *ver);

/* Download functions */
static int download_model(registry_t *reg, const char *model, const char *ver);
static int deploy_config(registry_t *reg, const char *model, const char *ver);
static int cache_model(registry_t *reg, const char *model, const char *ver);
static int hot_load(registry_t *reg, const char *model, const char *ver);
static int hot_update(registry_t *reg, const char *model, const char *old_ver, const char *new_ver);

/* Dependency functions */
static int dependency_add(registry_t *reg, const char *from, const char *to);
static int dependency_check_compat(registry_t *reg, const char *from, const char *to);
static int dependency_graph(registry_t *reg, const char *model);

/* Test suite */
static int test_suite(void);

/* Entry point */
void _start(void);

/* ── String Utilities ───────────────────────────────────────────────────── */

static int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

static int str_len(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static void str_cpy(char *dst, const char *src, int max) {
    int i;
    for (i = 0; i < max - 1 && src[i]; i++) {
        dst[i] = src[i];
    }
    dst[i] = '\0';
}

static void str_cat(char *dst, const char *src, int max) {
    int dlen = str_len(dst);
    int i;
    for (i = 0; i < max - dlen - 1 && src[i]; i++) {
        dst[dlen + i] = src[i];
    }
    dst[dlen + i] = '\0';
}

static void int_to_str(int val, char *buf, int max) {
    int i = 0;
    int neg = 0;
    if (val < 0) { neg = 1; val = -val; }
    if (val == 0) { buf[i++] = '0'; buf[i] = '\0'; return; }
    while (val > 0 && i < max - 1) {
        buf[i++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg && i < max - 1) buf[i++] = '-';
    buf[i] = '\0';
    /* reverse */
    int lo = 0, hi = i - 1;
    while (lo < hi) {
        char tmp = buf[lo];
        buf[lo] = buf[hi];
        buf[hi] = tmp;
        lo++; hi--;
    }
}

static void print_str(const char *s) { host_print(s); }

static void print_int(int v) {
    char buf[32];
    int_to_str(v, buf, 32);
    host_print(buf);
}

static void print_line(const char *s) {
    host_print(s);
    host_print("\n");
}

static void print_sep(void) {
    print_line("────────────────────────────────────────");
}

/* ── Memory Utilities ───────────────────────────────────────────────────── */

static void *mem_alloc(unsigned int size) {
    unsigned int ptr = host_alloc(size, 4);
    if (ptr == 0) return (void *)0;
    mem_zero((void *)ptr, size);
    return (void *)ptr;
}

static void mem_zero(void *ptr, unsigned int size) {
    unsigned char *p = (unsigned char *)ptr;
    unsigned int i;
    for (i = 0; i < size; i++) p[i] = 0;
}

/* ── Registry Init ──────────────────────────────────────────────────────── */

static void registry_init(registry_t *reg) {
    mem_zero(reg, sizeof(registry_t));
    reg->num_models = 0;
    reg->num_aliases = 0;
    reg->num_ab_configs = 0;
    reg->num_approvals = 0;
    reg->num_deps = 0;
    reg->auto_approve_enabled = 0;
    reg->next_request_id = 1000;
}

/* ── Model Functions ────────────────────────────────────────────────────── */

static model_t *model_find(registry_t *reg, const char *name) {
    int i;
    for (i = 0; i < reg->num_models; i++) {
        if (str_eq(reg->models[i].name, name)) return &reg->models[i];
    }
    return (model_t *)0;
}

static int model_register(registry_t *reg, const char *name, const char *desc) {
    if (model_find(reg, name) != (model_t *)0) {
        print_str("ERR: model already exists: "); print_line(name);
        return STATUS_ERR;
    }
    if (reg->num_models >= MAX_MODELS) {
        print_line("ERR: max models reached");
        return STATUS_ERR;
    }
    model_t *m = &reg->models[reg->num_models];
    str_cpy(m->name, name, MAX_NAME_LEN);
    str_cpy(m->desc, desc, MAX_DESC_LEN);
    m->num_versions = 0;
    m->active_version = -1;
    reg->num_models++;
    print_str("OK: registered model: "); print_line(name);
    return STATUS_OK;
}

static int model_list(registry_t *reg) {
    int i;
    print_line("[Model Registry]");
    if (reg->num_models == 0) {
        print_line("  (empty)");
        return STATUS_OK;
    }
    for (i = 0; i < reg->num_models; i++) {
        model_t *m = &reg->models[i];
        print_str("  ["); print_int(i); print_str("] ");
        print_str(m->name);
        print_str(" ("); print_int(m->num_versions); print_str(" versions)");
        print_str(" - "); print_line(m->desc);
    }
    return STATUS_OK;
}

static int model_delete(registry_t *reg, const char *name) {
    int i;
    for (i = 0; i < reg->num_models; i++) {
        if (str_eq(reg->models[i].name, name)) {
            int j;
            for (j = i; j < reg->num_models - 1; j++) {
                reg->models[j] = reg->models[j + 1];
            }
            reg->num_models--;
            print_str("OK: deleted model: "); print_line(name);
            return STATUS_OK;
        }
    }
    print_str("ERR: model not found: "); print_line(name);
    return STATUS_NOTFOUND;
}

/* ── Version Functions ──────────────────────────────────────────────────── */

static int version_create(registry_t *reg, const char *model, const char *ver) {
    model_t *m = model_find(reg, model);
    if (m == (model_t *)0) {
        print_str("ERR: model not found: "); print_line(model);
        return STATUS_NOTFOUND;
    }
    if (m->num_versions >= MAX_VERSIONS) {
        print_line("ERR: max versions reached");
        return STATUS_ERR;
    }
    int i;
    for (i = 0; i < m->num_versions; i++) {
        if (str_eq(m->versions[i].version, ver)) {
            print_str("ERR: version exists: "); print_line(ver);
            return STATUS_ERR;
        }
    }
    model_version_t *v = &m->versions[m->num_versions];
    str_cpy(v->version, ver, MAX_NAME_LEN);
    v->stage = STAGE_NONE;
    v->num_metrics = 0;
    v->approved = 0;
    v->download_count = 0;
    v->cached = 0;
    v->hot_loaded = 0;
    v->lineage.framework = -1;
    v->lineage.num_params = 0;
    m->num_versions++;
    if (m->active_version < 0) m->active_version = 0;
    print_str("OK: created version "); print_str(ver);
    print_str(" for "); print_line(model);
    return STATUS_OK;
}

static int version_list(registry_t *reg, const char *model) {
    model_t *m = model_find(reg, model);
    if (m == (model_t *)0) {
        print_str("ERR: model not found: "); print_line(model);
        return STATUS_NOTFOUND;
    }
    print_str("[Versions for "); print_str(model); print_line("]");
    int i;
    for (i = 0; i < m->num_versions; i++) {
        model_version_t *v = &m->versions[i];
        print_str("  "); print_str(v->version);
        print_str(" ["); print_str(stage_name(v->stage)); print_str("]");
        if (v->approved) print_str(" (approved)");
        if (v->cached) print_str(" (cached)");
        print_str("\n");
    }
    return STATUS_OK;
}

static model_version_t *version_get(registry_t *reg, const char *model, const char *ver) {
    model_t *m = model_find(reg, model);
    if (m == (model_t *)0) return (model_version_t *)0;
    int i;
    for (i = 0; i < m->num_versions; i++) {
        if (str_eq(m->versions[i].version, ver)) return &m->versions[i];
    }
    return (model_version_t *)0;
}

static int version_delete(registry_t *reg, const char *model, const char *ver) {
    model_t *m = model_find(reg, model);
    if (m == (model_t *)0) return STATUS_NOTFOUND;
    int i;
    for (i = 0; i < m->num_versions; i++) {
        if (str_eq(m->versions[i].version, ver)) {
            int j;
            for (j = i; j < m->num_versions - 1; j++) {
                m->versions[j] = m->versions[j + 1];
            }
            m->num_versions--;
            print_str("OK: deleted version "); print_str(ver);
            print_str(" from "); print_line(model);
            return STATUS_OK;
        }
    }
    print_str("ERR: version not found: "); print_line(ver);
    return STATUS_NOTFOUND;
}

/* ── Stage Functions ────────────────────────────────────────────────────── */

static const char *stage_name(int stage) {
    if (stage == STAGE_NONE) return "None";
    if (stage == STAGE_STAGING) return "Staging";
    if (stage == STAGE_PRODUCTION) return "Production";
    if (stage == STAGE_ARCHIVED) return "Archived";
    return "Unknown";
}

static int stage_transition(registry_t *reg, const char *model, const char *ver, int new_stage) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) {
        print_str("ERR: version not found: "); print_str(ver);
        print_str("/"); print_line(model);
        return STATUS_NOTFOUND;
    }
    /* Validate transition rules */
    int old = v->stage;
    int valid = 0;
    if (old == STAGE_NONE && new_stage == STAGE_STAGING) valid = 1;
    if (old == STAGE_STAGING && new_stage == STAGE_PRODUCTION) valid = 1;
    if (old == STAGE_STAGING && new_stage == STAGE_NONE) valid = 1;
    if (old == STAGE_PRODUCTION && new_stage == STAGE_ARCHIVED) valid = 1;
    if (old == STAGE_PRODUCTION && new_stage == STAGE_STAGING) valid = 1;
    if (old == STAGE_ARCHIVED && new_stage == STAGE_STAGING) valid = 1;
    if (!valid) {
        print_str("ERR: invalid transition ");
        print_str(stage_name(old)); print_str(" -> ");
        print_str(stage_name(new_stage)); print_str("\n");
        return STATUS_ERR;
    }
    v->stage = new_stage;
    print_str("OK: stage transition "); print_str(ver);
    print_str(": "); print_str(stage_name(old));
    print_str(" -> "); print_str(stage_name(new_stage));
    print_str("\n");
    return STATUS_OK;
}

/* ── Alias Functions ────────────────────────────────────────────────────── */

static int alias_create(registry_t *reg, const char *alias, const char *ver) {
    if (reg->num_aliases >= MAX_ALIASES) {
        print_line("ERR: max aliases reached");
        return STATUS_ERR;
    }
    int i;
    for (i = 0; i < reg->num_aliases; i++) {
        if (str_eq(reg->aliases[i].alias, alias)) {
            str_cpy(reg->aliases[i].version, ver, MAX_NAME_LEN);
            print_str("OK: updated alias "); print_str(alias);
            print_str(" -> "); print_line(ver);
            return STATUS_OK;
        }
    }
    alias_t *a = &reg->aliases[reg->num_aliases];
    str_cpy(a->alias, alias, MAX_NAME_LEN);
    str_cpy(a->version, ver, MAX_NAME_LEN);
    reg->num_aliases++;
    print_str("OK: alias "); print_str(alias);
    print_str(" -> "); print_line(ver);
    return STATUS_OK;
}

static const char *alias_resolve(registry_t *reg, const char *alias) {
    int i;
    for (i = 0; i < reg->num_aliases; i++) {
        if (str_eq(reg->aliases[i].alias, alias)) {
            return reg->aliases[i].version;
        }
    }
    return (const char *)0;
}

static int alias_delete(registry_t *reg, const char *alias) {
    int i;
    for (i = 0; i < reg->num_aliases; i++) {
        if (str_eq(reg->aliases[i].alias, alias)) {
            int j;
            for (j = i; j < reg->num_aliases - 1; j++) {
                reg->aliases[j] = reg->aliases[j + 1];
            }
            reg->num_aliases--;
            print_str("OK: deleted alias: "); print_line(alias);
            return STATUS_OK;
        }
    }
    print_str("ERR: alias not found: "); print_line(alias);
    return STATUS_NOTFOUND;
}

/* ── Metric Functions ───────────────────────────────────────────────────── */

static int metric_record(registry_t *reg, const char *model, const char *ver, int type, int value) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    /* Update existing or add new */
    int i;
    for (i = 0; i < v->num_metrics; i++) {
        if (v->metrics[i].type == type) {
            v->metrics[i].value_i = value;
            print_str("OK: updated metric type="); print_int(type);
            print_str(" val="); print_int(value); print_str("\n");
            return STATUS_OK;
        }
    }
    if (v->num_metrics >= MAX_METRICS) {
        print_line("ERR: max metrics reached");
        return STATUS_ERR;
    }
    v->metrics[v->num_metrics].type = type;
    v->metrics[v->num_metrics].value_i = value;
    v->num_metrics++;
    print_str("OK: recorded metric type="); print_int(type);
    print_str(" val="); print_int(value); print_str("\n");
    return STATUS_OK;
}

static int metric_get(registry_t *reg, const char *model, const char *ver, int type) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return -1;
    int i;
    for (i = 0; i < v->num_metrics; i++) {
        if (v->metrics[i].type == type) return v->metrics[i].value_i;
    }
    return -1;
}

static int metric_list(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    print_str("[Metrics for "); print_str(ver); print_line("]");
    int i;
    for (i = 0; i < v->num_metrics; i++) {
        print_str("  type="); print_int(v->metrics[i].type);
        print_str(" value="); print_int(v->metrics[i].value_i);
        print_str("\n");
    }
    return STATUS_OK;
}

/* ── Lineage Functions ──────────────────────────────────────────────────── */

static int lineage_track(registry_t *reg, const char *model, const char *ver,
                          const char *dataset, int fw, int params) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    str_cpy(v->lineage.dataset, dataset, MAX_NAME_LEN);
    v->lineage.framework = fw;
    v->lineage.num_params = params;
    str_cpy(v->lineage.name, ver, MAX_NAME_LEN);
    print_str("OK: lineage tracked for "); print_str(ver);
    print_str(" dataset="); print_str(dataset);
    print_str(" fw="); print_int(fw);
    print_str(" params="); print_int(params);
    print_str("\n");
    return STATUS_OK;
}

static int lineage_dataset(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    print_str("dataset: "); print_line(v->lineage.dataset);
    return STATUS_OK;
}

static int lineage_params(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    print_str("params: "); print_int(v->lineage.num_params); print_str("\n");
    return STATUS_OK;
}

static int lineage_framework(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    print_str("framework: ");
    if (v->lineage.framework == FW_PYTORCH) print_line("PyTorch");
    else if (v->lineage.framework == FW_TF) print_line("TensorFlow");
    else if (v->lineage.framework == FW_ONNX) print_line("ONNX");
    else print_line("Unknown");
    return STATUS_OK;
}

/* ── Compare Functions ──────────────────────────────────────────────────── */

static int compare_models(registry_t *reg, const char *model_a, const char *ver_a,
                           const char *model_b, const char *ver_b) {
    model_version_t *va = version_get(reg, model_a, ver_a);
    model_version_t *vb = version_get(reg, model_b, ver_b);
    if (va == (model_version_t *)0 || vb == (model_version_t *)0) {
        print_line("ERR: model/version not found for comparison");
        return STATUS_NOTFOUND;
    }
    print_line("[Model Comparison]");
    print_str("  A: "); print_str(model_a); print_str("/"); print_line(ver_a);
    print_str("  B: "); print_str(model_b); print_str("/"); print_line(ver_b);
    print_sep();
    int i, j;
    for (i = 0; i < va->num_metrics; i++) {
        for (j = 0; j < vb->num_metrics; j++) {
            if (va->metrics[i].type == vb->metrics[j].type) {
                print_str("  metric type="); print_int(va->metrics[i].type);
                print_str("  A="); print_int(va->metrics[i].value_i);
                print_str("  B="); print_int(vb->metrics[j].value_i);
                int diff = va->metrics[i].value_i - vb->metrics[j].value_i;
                print_str("  diff="); print_int(diff);
                print_str("\n");
            }
        }
    }
    return STATUS_OK;
}

static int compare_ab_config(registry_t *reg, const char *ma, const char *mb,
                              int metric, int split_a, int split_b) {
    if (split_a + split_b != 100) {
        print_line("ERR: traffic split must sum to 100");
        return STATUS_ERR;
    }
    if (reg->num_ab_configs >= MAX_ALIASES) {
        print_line("ERR: max AB configs reached");
        return STATUS_ERR;
    }
    ab_config_t *ab = &reg->ab_configs[reg->num_ab_configs];
    str_cpy(ab->model_a, ma, MAX_NAME_LEN);
    str_cpy(ab->model_b, mb, MAX_NAME_LEN);
    ab->metric_type = metric;
    ab->threshold_pct = 5;
    ab->traffic_split_a = split_a;
    ab->traffic_split_b = split_b;
    reg->num_ab_configs++;
    print_str("OK: AB config created: "); print_str(ma);
    print_str("("); print_int(split_a); print_str("%) vs ");
    print_str(mb); print_str("("); print_int(split_b);
    print_str("%)\n");
    return STATUS_OK;
}

static int compare_benchmark(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    print_line("[Benchmark Report]");
    print_str("  Model: "); print_str(model);
    print_str("/"); print_line(ver);
    print_str("  Metrics count: "); print_int(v->num_metrics); print_str("\n");
    print_str("  Approved: "); print_int(v->approved); print_str("\n");
    print_str("  Downloads: "); print_int(v->download_count); print_str("\n");
    print_str("  Cached: "); print_int(v->cached); print_str("\n");
    print_sep();
    return STATUS_OK;
}

/* ── Approval Functions ─────────────────────────────────────────────────── */

static int approval_request(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    if (reg->num_approvals >= MAX_VERSIONS) {
        print_line("ERR: max approvals reached");
        return STATUS_ERR;
    }
    /* Auto-approve if enabled */
    if (reg->auto_approve_enabled) {
        v->approved = 1;
        print_str("AUTO-APPROVED: "); print_str(ver);
        print_str("/"); print_line(model);
        return STATUS_OK;
    }
    approval_t *ap = &reg->approvals[reg->num_approvals];
    ap->request_id = reg->next_request_id++;
    str_cpy(ap->model, model, MAX_NAME_LEN);
    str_cpy(ap->version, ver, MAX_NAME_LEN);
    ap->status = APPROVAL_PENDING;
    ap->reviewer[0] = '\0';
    reg->num_approvals++;
    print_str("OK: approval requested id="); print_int(ap->request_id);
    print_str(" for "); print_str(ver); print_str("/"); print_line(model);
    return STATUS_OK;
}

static int approval_approve(registry_t *reg, int req_id, const char *reviewer) {
    int i;
    for (i = 0; i < reg->num_approvals; i++) {
        if (reg->approvals[i].request_id == req_id) {
            reg->approvals[i].status = APPROVAL_APPROVED;
            str_cpy(reg->approvals[i].reviewer, reviewer, MAX_NAME_LEN);
            /* Mark version as approved */
            model_version_t *v = version_get(reg, reg->approvals[i].model,
                                              reg->approvals[i].version);
            if (v != (model_version_t *)0) v->approved = 1;
            print_str("OK: approved req="); print_int(req_id);
            print_str(" by "); print_line(reviewer);
            return STATUS_OK;
        }
    }
    print_str("ERR: request not found: "); print_int(req_id); print_str("\n");
    return STATUS_NOTFOUND;
}

static int approval_reject(registry_t *reg, int req_id, const char *reviewer) {
    int i;
    for (i = 0; i < reg->num_approvals; i++) {
        if (reg->approvals[i].request_id == req_id) {
            reg->approvals[i].status = APPROVAL_REJECTED;
            str_cpy(reg->approvals[i].reviewer, reviewer, MAX_NAME_LEN);
            print_str("OK: rejected req="); print_int(req_id);
            print_str(" by "); print_line(reviewer);
            return STATUS_OK;
        }
    }
    print_str("ERR: request not found: "); print_int(req_id); print_str("\n");
    return STATUS_NOTFOUND;
}

static int approval_auto_rule(registry_t *reg, int enabled) {
    reg->auto_approve_enabled = enabled;
    print_str("OK: auto-approve ");
    print_line(enabled ? "enabled" : "disabled");
    return STATUS_OK;
}

static int approval_history(registry_t *reg) {
    int i;
    print_line("[Approval History]");
    if (reg->num_approvals == 0) {
        print_line("  (empty)");
        return STATUS_OK;
    }
    for (i = 0; i < reg->num_approvals; i++) {
        approval_t *a = &reg->approvals[i];
        print_str("  id="); print_int(a->request_id);
        print_str(" "); print_str(a->model); print_str("/");
        print_str(a->version);
        print_str(" status="); print_int(a->status);
        if (a->reviewer[0]) {
            print_str(" by "); print_str(a->reviewer);
        }
        print_str("\n");
    }
    return STATUS_OK;
}

static int approval_rollback(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    v->approved = 0;
    v->stage = STAGE_NONE;
    /* Find and mark approval as rejected */
    int i;
    for (i = 0; i < reg->num_approvals; i++) {
        if (str_eq(reg->approvals[i].model, model) &&
            str_eq(reg->approvals[i].version, ver)) {
            reg->approvals[i].status = APPROVAL_REJECTED;
        }
    }
    print_str("OK: rolled back "); print_str(ver);
    print_str("/"); print_line(model);
    return STATUS_OK;
}

/* ── Download Functions ─────────────────────────────────────────────────── */

static int download_model(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    if (!v->approved) {
        print_line("ERR: model not approved for download");
        return STATUS_ERR;
    }
    v->download_count++;
    print_str("OK: downloaded "); print_str(ver);
    print_str("/"); print_str(model);
    print_str(" (total: "); print_int(v->download_count); print_str(")\n");
    return STATUS_OK;
}

static int deploy_config(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    print_line("[Deploy Config]");
    print_str("  model: "); print_line(model);
    print_str("  version: "); print_line(ver);
    print_str("  stage: "); print_str(stage_name(v->stage)); print_str("\n");
    print_str("  framework: "); print_int(v->lineage.framework); print_str("\n");
    print_str("  cached: "); print_int(v->cached); print_str("\n");
    print_sep();
    return STATUS_OK;
}

static int cache_model(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    v->cached = 1;
    print_str("OK: cached "); print_str(ver);
    print_str("/"); print_line(model);
    return STATUS_OK;
}

static int hot_load(registry_t *reg, const char *model, const char *ver) {
    model_version_t *v = version_get(reg, model, ver);
    if (v == (model_version_t *)0) return STATUS_NOTFOUND;
    if (!v->cached) {
        print_line("ERR: model not cached, cache first");
        return STATUS_ERR;
    }
    v->hot_loaded = 1;
    print_str("OK: hot-loaded "); print_str(ver);
    print_str("/"); print_line(model);
    return STATUS_OK;
}

static int hot_update(registry_t *reg, const char *model,
                       const char *old_ver, const char *new_ver) {
    model_version_t *vold = version_get(reg, model, old_ver);
    model_version_t *vnew = version_get(reg, model, new_ver);
    if (vold == (model_version_t *)0 || vnew == (model_version_t *)0) {
        print_line("ERR: old or new version not found");
        return STATUS_NOTFOUND;
    }
    vold->hot_loaded = 0;
    vold->stage = STAGE_ARCHIVED;
    vnew->hot_loaded = 1;
    vnew->stage = STAGE_PRODUCTION;
    print_str("OK: hot-updated "); print_str(model);
    print_str(": "); print_str(old_ver);
    print_str(" -> "); print_line(new_ver);
    return STATUS_OK;
}

/* ── Dependency Functions ───────────────────────────────────────────────── */

static int dependency_add(registry_t *reg, const char *from, const char *to) {
    if (reg->num_deps >= MAX_DEPS) {
        print_line("ERR: max deps reached");
        return STATUS_ERR;
    }
    dependency_t *d = &reg->deps[reg->num_deps];
    str_cpy(d->from_model, from, MAX_NAME_LEN);
    str_cpy(d->to_model, to, MAX_NAME_LEN);
    d->compat = 1; /* default compatible */
    reg->num_deps++;
    print_str("OK: dependency added: "); print_str(from);
    print_str(" -> "); print_line(to);
    return STATUS_OK;
}

static int dependency_check_compat(registry_t *reg, const char *from, const char *to) {
    int i;
    for (i = 0; i < reg->num_deps; i++) {
        if (str_eq(reg->deps[i].from_model, from) &&
            str_eq(reg->deps[i].to_model, to)) {
            print_str("compat: "); print_str(from);
            print_str(" -> "); print_str(to);
            print_str(": "); print_int(reg->deps[i].compat);
            print_str("\n");
            return reg->deps[i].compat;
        }
    }
    print_str("no dependency found: "); print_str(from);
    print_str(" -> "); print_line(to);
    return 0;
}

static int dependency_graph(registry_t *reg, const char *model) {
    print_str("[Dependency Graph for "); print_str(model); print_line("]");
    int i;
    int found = 0;
    /* Forward dependencies */
    print_line("  Forward (depends on):");
    for (i = 0; i < reg->num_deps; i++) {
        if (str_eq(reg->deps[i].from_model, model)) {
            print_str("    -> "); print_line(reg->deps[i].to_model);
            found++;
        }
    }
    /* Backward dependencies */
    print_line("  Backward (depended by):");
    for (i = 0; i < reg->num_deps; i++) {
        if (str_eq(reg->deps[i].to_model, model)) {
            print_str("    <- "); print_line(reg->deps[i].from_model);
            found++;
        }
    }
    if (found == 0) print_line("  (no dependencies)");
    print_sep();
    return STATUS_OK;
}

/* ── Test Suite ─────────────────────────────────────────────────────────── */

static int test_suite(void) {
    int pass = 0;
    int fail = 0;
    registry_t *reg = (registry_t *)mem_alloc(sizeof(registry_t));
    if (reg == (registry_t *)0) {
        print_line("FATAL: cannot alloc registry");
        host_exit(1);
        return -1;
    }
    registry_init(reg);

    print_sep();
    print_line("MODEL REGISTRY TEST SUITE");
    print_sep();

    /* 1. Model registration */
    print_line("\n[TEST] Model Registration");
    if (model_register(reg, "resnet50", "Image classification ResNet-50") == STATUS_OK) pass++; else fail++;
    if (model_register(reg, "bert-base", "NLP BERT base model") == STATUS_OK) pass++; else fail++;
    if (model_register(reg, "gpt-mini", "Mini GPT text generation") == STATUS_OK) pass++; else fail++;
    if (model_register(reg, "resnet50", "duplicate") == STATUS_ERR) pass++; else fail++;
    model_list(reg);

    /* 2. Version creation */
    print_line("\n[TEST] Version Creation");
    if (version_create(reg, "resnet50", "v1.0") == STATUS_OK) pass++; else fail++;
    if (version_create(reg, "resnet50", "v1.1") == STATUS_OK) pass++; else fail++;
    if (version_create(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (version_create(reg, "bert-base", "v1.0") == STATUS_OK) pass++; else fail++;
    if (version_create(reg, "bert-base", "v2.0") == STATUS_OK) pass++; else fail++;
    if (version_create(reg, "gpt-mini", "v0.1") == STATUS_OK) pass++; else fail++;
    version_list(reg, "resnet50");

    /* 3. Stage transitions: None -> Staging -> Production */
    print_line("\n[TEST] Stage Transitions");
    if (stage_transition(reg, "resnet50", "v1.0", STAGE_STAGING) == STATUS_OK) pass++; else fail++;
    if (stage_transition(reg, "resnet50", "v1.0", STAGE_PRODUCTION) == STATUS_OK) pass++; else fail++;
    if (stage_transition(reg, "resnet50", "v1.1", STAGE_STAGING) == STATUS_OK) pass++; else fail++;
    if (stage_transition(reg, "resnet50", "v1.0", STAGE_STAGING) == STATUS_OK) pass++; else fail++;
    if (stage_transition(reg, "resnet50", "v1.0", STAGE_ARCHIVED) == STATUS_ERR) pass++; else fail++;
    version_list(reg, "resnet50");

    /* 4. Alias management */
    print_line("\n[TEST] Alias Management");
    if (alias_create(reg, "latest", "v2.0") == STATUS_OK) pass++; else fail++;
    if (alias_create(reg, "stable", "v1.0") == STATUS_OK) pass++; else fail++;
    if (alias_create(reg, "canary", "v1.1") == STATUS_OK) pass++; else fail++;
    const char *resolved = alias_resolve(reg, "latest");
    if (resolved != (const char *)0 && str_eq(resolved, "v2.0")) pass++; else fail++;
    print_str("  alias 'latest' -> "); print_line(resolved ? resolved : "NULL");
    if (alias_delete(reg, "canary") == STATUS_OK) pass++; else fail++;

    /* 5. Metric recording */
    print_line("\n[TEST] Metric Recording");
    if (metric_record(reg, "resnet50", "v2.0", METRIC_ACCURACY, 9520) == STATUS_OK) pass++; else fail++;
    if (metric_record(reg, "resnet50", "v2.0", METRIC_LOSS, 480) == STATUS_OK) pass++; else fail++;
    if (metric_record(reg, "resnet50", "v2.0", METRIC_F1, 9410) == STATUS_OK) pass++; else fail++;
    if (metric_record(reg, "resnet50", "v1.0", METRIC_ACCURACY, 9100) == STATUS_OK) pass++; else fail++;
    if (metric_record(reg, "resnet50", "v1.0", METRIC_LOSS, 620) == STATUS_OK) pass++; else fail++;
    if (metric_record(reg, "bert-base", "v1.0", METRIC_ACCURACY, 8950) == STATUS_OK) pass++; else fail++;
    int acc = metric_get(reg, "resnet50", "v2.0", METRIC_ACCURACY);
    if (acc == 9520) pass++; else fail++;
    metric_list(reg, "resnet50", "v2.0");

    /* 6. Lineage tracking */
    print_line("\n[TEST] Lineage Tracking");
    if (lineage_track(reg, "resnet50", "v2.0", "ImageNet-2024", FW_PYTORCH, 25000000) == STATUS_OK) pass++; else fail++;
    if (lineage_track(reg, "bert-base", "v1.0", "WikiText-103", FW_TF, 110000000) == STATUS_OK) pass++; else fail++;
    if (lineage_dataset(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (lineage_params(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (lineage_framework(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (lineage_framework(reg, "bert-base", "v1.0") == STATUS_OK) pass++; else fail++;

    /* 7. Model comparison */
    print_line("\n[TEST] Model Comparison");
    if (compare_models(reg, "resnet50", "v1.0", "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (compare_ab_config(reg, "resnet50-v1.0", "resnet50-v2.0", METRIC_ACCURACY, 70, 30) == STATUS_OK) pass++; else fail++;
    if (compare_ab_config(reg, "a", "b", METRIC_LOSS, 110, 0) == STATUS_ERR) pass++; else fail++;
    if (compare_benchmark(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;

    /* 8. Approval workflow */
    print_line("\n[TEST] Approval Workflow");
    if (approval_request(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    int req_id = reg->approvals[0].request_id;
    if (approval_approve(reg, req_id, "admin") == STATUS_OK) pass++; else fail++;
    if (approval_request(reg, "resnet50", "v1.1") == STATUS_OK) pass++; else fail++;
    int req_id2 = reg->approvals[1].request_id;
    if (approval_reject(reg, req_id2, "reviewer1") == STATUS_OK) pass++; else fail++;
    approval_history(reg);
    if (approval_auto_rule(reg, 1) == STATUS_OK) pass++; else fail++;
    if (approval_request(reg, "bert-base", "v2.0") == STATUS_OK) pass++; else fail++;
    if (approval_auto_rule(reg, 0) == STATUS_OK) pass++; else fail++;

    /* 9. Download and deployment */
    print_line("\n[TEST] Download & Deployment");
    if (download_model(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (download_model(reg, "resnet50", "v1.1") == STATUS_ERR) pass++; else fail++;
    if (deploy_config(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (cache_model(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (hot_load(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    if (hot_load(reg, "resnet50", "v1.0") == STATUS_ERR) pass++; else fail++;

    /* 10. Hot update */
    print_line("\n[TEST] Hot Update");
    version_create(reg, "resnet50", "v2.1");
    stage_transition(reg, "resnet50", "v2.1", STAGE_STAGING);
    stage_transition(reg, "resnet50", "v2.1", STAGE_PRODUCTION);
    metric_record(reg, "resnet50", "v2.1", METRIC_ACCURACY, 9650);
    cache_model(reg, "resnet50", "v2.1");
    hot_load(reg, "resnet50", "v2.1");
    if (hot_update(reg, "resnet50", "v2.0", "v2.1") == STATUS_OK) pass++; else fail++;

    /* 11. Dependency management */
    print_line("\n[TEST] Dependency Management");
    if (dependency_add(reg, "bert-base-v1.0", "tokenizer-v3") == STATUS_OK) pass++; else fail++;
    if (dependency_add(reg, "resnet50-v2.1", "preprocessor-v2") == STATUS_OK) pass++; else fail++;
    if (dependency_add(reg, "gpt-mini-v0.1", "tokenizer-v3") == STATUS_OK) pass++; else fail++;
    if (dependency_check_compat(reg, "bert-base-v1.0", "tokenizer-v3") == 1) pass++; else fail++;
    if (dependency_check_compat(reg, "nonexistent", "tokenizer-v3") == 0) pass++; else fail++;
    if (dependency_graph(reg, "tokenizer-v3") == STATUS_OK) pass++; else fail++;
    if (dependency_graph(reg, "resnet50-v2.1") == STATUS_OK) pass++; else fail++;

    /* 12. Approval rollback */
    print_line("\n[TEST] Approval Rollback");
    if (approval_rollback(reg, "resnet50", "v2.0") == STATUS_OK) pass++; else fail++;
    model_version_t *rv = version_get(reg, "resnet50", "v2.0");
    if (rv != (model_version_t *)0 && rv->approved == 0) pass++; else fail++;
    approval_history(reg);

    /* 13. Version and model deletion */
    print_line("\n[TEST] Deletion");
    if (version_delete(reg, "resnet50", "v1.0") == STATUS_OK) pass++; else fail++;
    if (model_delete(reg, "gpt-mini") == STATUS_OK) pass++; else fail++;
    model_list(reg);

    /* Summary */
    print_sep();
    print_str("RESULTS: "); print_int(pass); print_str(" passed, ");
    print_int(fail); print_str(" failed, total: "); print_int(pass + fail);
    print_str("\n");
    if (fail == 0) {
        print_line("ALL TESTS PASSED");
    } else {
        print_line("SOME TESTS FAILED");
    }
    print_sep();

    return fail;
}

/* ── Entry Point ────────────────────────────────────────────────────────── */

void _start(void) {
    char *buf = (char *)mem_alloc(256);
    if (buf == (char *)0) {
        host_exit(1);
        return;
    }
    int argc = host_get_argv(0, 256);

    if (argc <= 0) {
        /* Default: run tests */
        int failures = test_suite();
        host_exit(failures);
        return;
    }

    /* Parse first argument from buffer */
    if (str_eq(buf, "-h") || str_eq(buf, "--help")) {
        print_line("model_registry - AI-ASM Model Registry Module");
        print_line("Usage: model_registry [options]");
        print_line("  -h, --help     Show this help");
        print_line("  -t, --test     Run test suite");
        print_line("  -l, --list     List registered models");
        print_line("  -r, --register Register a new model");
        print_line("  -c, --compare  Compare model versions");
        print_line("  -a, --approve  Approval workflow");
        print_line("  -d, --deploy   Deployment operations");
        host_exit(0);
        return;
    }

    if (str_eq(buf, "-t") || str_eq(buf, "--test")) {
        int failures = test_suite();
        host_exit(failures);
        return;
    }

    if (str_eq(buf, "-l") || str_eq(buf, "--list")) {
        registry_t *reg = (registry_t *)mem_alloc(sizeof(registry_t));
        if (reg == (registry_t *)0) { host_exit(1); return; }
        registry_init(reg);
        print_line("Model Registry initialized (empty)");
        model_list(reg);
        host_exit(0);
        return;
    }

    /* Unknown option */
    print_str("Unknown option: "); print_line(buf);
    print_line("Use -h for help");
    host_exit(1);
}
