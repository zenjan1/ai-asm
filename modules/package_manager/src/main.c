/* package_manager: Dependency resolution, publishing, installation & mirrors */

__attribute__((import_module("host"), import_name("print")))
extern void host_print(unsigned int off, unsigned int len);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_PACKAGES   64
#define MAX_DEPS       8
#define MAX_MIRRORS    8
#define MAX_CACHE      32
#define MAX_NAME       48
#define MAX_VERSION    16
#define MAX_DESC       128
#define MAX_URL        96
#define MAX_INPUT      512

#define PKG_AVAILABLE  0
#define PKG_INSTALLED  1
#define PKG_HELD       2
#define PKG_BROKEN     3

#define MIRROR_OFFICIAL   0
#define MIRROR_THIRDPARTY 1
#define MIRROR_PRIVATE    2

#define SIGN_NONE   0
#define SIGN_GPG    1
#define SIGN_COSIGN 2

/* Version constraint operators */
#define VC_EQ  0   /* == */
#define VC_GE  1   /* >= */
#define VC_LE  2   /* <= */
#define VC_GT  3   /* >  */
#define VC_LT  4   /* <  */

typedef struct {
    char name[MAX_NAME]; int op; int major; int minor; int patch;
} version_constraint_t;

typedef struct {
    int pkg_id; char name[MAX_NAME]; char version[MAX_VERSION];
    char desc[MAX_DESC]; int deps_count; char deps[MAX_DEPS][MAX_NAME];
    version_constraint_t constraints[MAX_DEPS];
    int status; int sign_type; int size_kb; char category[MAX_NAME];
} package_t;

typedef struct {
    int mirror_id; char name[MAX_NAME]; char url[MAX_URL];
    int type; int priority; int enabled; int pkg_count;
} mirror_t;

typedef struct {
    char name[MAX_NAME]; char version[MAX_VERSION];
    int cached_at; int size_kb;
} cache_entry_t;

static unsigned int heap_pos = 65536;
static package_t packages[MAX_PACKAGES];
static int pkg_count = 0, next_pkg_id = 0;
static mirror_t mirrors[MAX_MIRRORS];
static int mirror_count = 0;
static cache_entry_t cache[MAX_CACHE];
static int cache_count = 0;

/* ── memory & string helpers ── */

static unsigned int alloc(unsigned int n) {
    unsigned int p = heap_pos; heap_pos += n;
    heap_pos = (heap_pos + 15) & ~15u; return p;
}
static void copy_to_mem(const char *s, unsigned int d, unsigned int n) {
    char *p = (char *)d; for (unsigned int i = 0; i < n; i++) p[i] = s[i];
}
static unsigned int my_strlen(const char *s) { unsigned int n = 0; while (s[n]) n++; return n; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return 1; a++; b++; } return (*a != *b);
}
static void copy_string(char *d, const char *s, unsigned int m) {
    unsigned int i = 0; while (s[i] && i < m - 1) { d[i] = s[i]; i++; } d[i] = 0;
}
static void print_str(const char *s) {
    unsigned int l = my_strlen(s), b = alloc(l + 1);
    copy_to_mem(s, b, l + 1); host_print(b, l);
}
static void print_int(int n) {
    char b[16]; int i = 14, neg = 0; b[15] = 0;
    if (n < 0) { neg = 1; n = -n; }
    if (n == 0) b[i--] = '0';
    while (n > 0) { b[i--] = '0' + (n % 10); n /= 10; }
    if (neg) b[i--] = '-'; i++;
    unsigned int l = my_strlen(&b[i]), d = alloc(l + 1);
    copy_to_mem(&b[i], d, l + 1); host_print(d, l);
}

/* ── version parsing ── */

static void parse_version(const char *v, int *maj, int *min, int *pat) {
    *maj = 0; *min = 0; *pat = 0;
    while (*v && *v != '.') { *maj = *maj * 10 + (*v - '0'); v++; }
    if (*v == '.') v++;
    while (*v && *v != '.') { *min = *min * 10 + (*v - '0'); v++; }
    if (*v == '.') v++;
    while (*v) { *pat = *pat * 10 + (*v - '0'); v++; }
}

static int version_satisfies(const char *ver, version_constraint_t *c) {
    int maj, min, pat; parse_version(ver, &maj, &min, &pat);
    int cm = c->major, cn = c->minor, cp = c->patch;
    int v = maj * 10000 + min * 100 + pat;
    int r = cm * 10000 + cn * 100 + cp;
    switch (c->op) {
        case VC_EQ: return v == r;
        case VC_GE: return v >= r;
        case VC_LE: return v <= r;
        case VC_GT: return v >  r;
        case VC_LT: return v <  r;
        default: return 0;
    }
}

static const char *op_str(int op) {
    switch (op) {
        case VC_EQ: return "=="; case VC_GE: return ">=";
        case VC_LE: return "<="; case VC_GT: return ">";
        case VC_LT: return "<"; default: return "??";
    }
}
static const char *status_str(int s) {
    switch (s) {
        case PKG_AVAILABLE: return "available"; case PKG_INSTALLED: return "installed";
        case PKG_HELD: return "held"; case PKG_BROKEN: return "broken";
        default: return "unknown";
    }
}
static const char *mirror_type_str(int t) {
    switch (t) {
        case MIRROR_OFFICIAL: return "official";
        case MIRROR_THIRDPARTY: return "third-party";
        case MIRROR_PRIVATE: return "private";
        default: return "unknown";
    }
}

/* ── package registry ── */

static int register_package(const char *name, const char *ver, const char *desc,
                            int sign, int size_kb, const char *cat) {
    if (pkg_count >= MAX_PACKAGES) return -1;
    package_t *p = &packages[pkg_count];
    p->pkg_id = next_pkg_id++; p->status = PKG_AVAILABLE;
    p->sign_type = sign; p->size_kb = size_kb; p->deps_count = 0;
    copy_string(p->name, name, MAX_NAME);
    copy_string(p->version, ver, MAX_VERSION);
    copy_string(p->desc, desc, MAX_DESC);
    copy_string(p->category, cat, MAX_NAME);
    pkg_count++;
    return p->pkg_id;
}

static void add_dependency(int pkg_id, const char *dep_name, int op,
                           int maj, int min, int pat) {
    for (int i = 0; i < pkg_count; i++) {
        if (packages[i].pkg_id != pkg_id) continue;
        package_t *p = &packages[i];
        if (p->deps_count >= MAX_DEPS) return;
        int d = p->deps_count;
        copy_string(p->deps[d], dep_name, MAX_NAME);
        copy_string(p->constraints[d].name, dep_name, MAX_NAME);
        p->constraints[d].op = op;
        p->constraints[d].major = maj;
        p->constraints[d].minor = min;
        p->constraints[d].patch = pat;
        p->deps_count++;
        return;
    }
}

static int find_package(const char *name) {
    for (int i = 0; i < pkg_count; i++)
        if (my_strcmp(packages[i].name, name) == 0) return i;
    return -1;
}

/* ── SAT-based dependency resolver ── */

static int resolve_var = 0;
static int sat_assign[MAX_PACKAGES]; /* 0=unassigned, 1=true(install), -1=false(skip) */

static int sat_solve(int pkg_idx, int depth) {
    if (pkg_idx < 0) return -1;
    package_t *p = &packages[pkg_idx];
    if (sat_assign[pkg_idx] == 1) return 0;
    if (sat_assign[pkg_idx] == -1) {
        print_str("  conflict: "); print_str(p->name);
        print_str(" already excluded\n"); return -1;
    }
    sat_assign[pkg_idx] = 1;
    print_str("  sat["); print_int(depth); print_str("]: assign ");
    print_str(p->name); print_str(" v"); print_str(p->version);
    print_str(" = INSTALL\n");

    for (int d = 0; d < p->deps_count; d++) {
        int dep_idx = find_package(p->deps[d]);
        if (dep_idx < 0) {
            print_str("  sat: MISSING dep '"); print_str(p->deps[d]);
            print_str("' for "); print_str(p->name); print_str("\n");
            return -1;
        }
        package_t *dep = &packages[dep_idx];
        if (!version_satisfies(dep->version, &p->constraints[d])) {
            print_str("  sat: VERSION FAIL "); print_str(dep->name);
            print_str(" v"); print_str(dep->version);
            print_str(" "); print_str(op_str(p->constraints[d].op));
            print_str(" "); print_int(p->constraints[d].major);
            print_str("."); print_int(p->constraints[d].minor);
            print_str("."); print_int(p->constraints[d].patch); print_str("\n");
            return -1;
        }
        if (sat_solve(dep_idx, depth + 1) < 0) return -1;
    }
    return 0;
}

static int resolve_dependencies(const char *pkg_name) {
    int idx = find_package(pkg_name);
    if (idx < 0) {
        print_str("resolve: package '"); print_str(pkg_name);
        print_str("' not found\n"); return -1;
    }
    print_str("resolve: SAT solving for "); print_str(pkg_name); print_str("\n");
    for (int i = 0; i < pkg_count; i++) sat_assign[i] = 0;
    int rc = sat_solve(idx, 0);
    if (rc == 0) {
        print_str("resolve: SATISFIABLE\n");
        int total = 0;
        for (int i = 0; i < pkg_count; i++) {
            if (sat_assign[i] == 1) {
                print_str("  install: "); print_str(packages[i].name);
                print_str(" v"); print_str(packages[i].version); print_str("\n");
                total++;
            }
        }
        print_str("resolve: "); print_int(total); print_str(" packages to install\n");
    } else {
        print_str("resolve: UNSATISFIABLE - conflicts detected\n");
    }
    return rc;
}

/* ── conflict detection ── */

static int detect_conflicts(const char *pkg_a, const char *pkg_b) {
    int ia = find_package(pkg_a), ib = find_package(pkg_b);
    if (ia < 0 || ib < 0) return -1;
    package_t *a = &packages[ia], *b = &packages[ib];
    int conflicts = 0;
    for (int d = 0; d < a->deps_count; d++) {
        if (my_strcmp(a->deps[d], b->name) == 0) {
            if (!version_satisfies(b->version, &a->constraints[d])) {
                print_str("conflict: "); print_str(a->name); print_str(" requires ");
                print_str(b->name); print_str(" "); print_str(op_str(a->constraints[d].op));
                print_str(" "); print_int(a->constraints[d].major);
                print_str(" but v"); print_str(b->version); print_str(" available\n");
                conflicts++;
            }
        }
    }
    for (int d = 0; d < b->deps_count; d++) {
        if (my_strcmp(b->deps[d], a->name) == 0) {
            if (!version_satisfies(a->version, &b->constraints[d])) {
                print_str("conflict: "); print_str(b->name); print_str(" requires ");
                print_str(a->name); print_str(" "); print_str(op_str(b->constraints[d].op));
                print_str(" but v"); print_str(a->version); print_str(" available\n");
                conflicts++;
            }
        }
    }
    if (conflicts == 0) {
        print_str("conflict: "); print_str(pkg_a); print_str(" <-> ");
        print_str(pkg_b); print_str(" compatible\n");
    }
    return conflicts;
}

/* ── mirror management ── */

static int add_mirror(const char *name, const char *url, int type, int prio) {
    if (mirror_count >= MAX_MIRRORS) return -1;
    mirror_t *m = &mirrors[mirror_count];
    m->mirror_id = mirror_count; m->type = type;
    m->priority = prio; m->enabled = 1; m->pkg_count = 0;
    copy_string(m->name, name, MAX_NAME);
    copy_string(m->url, url, MAX_URL);
    mirror_count++;
    print_str("mirror: added "); print_str(name);
    print_str(" ["); print_str(mirror_type_str(type));
    print_str("] prio="); print_int(prio); print_str("\n");
    return m->mirror_id;
}

static void list_mirrors(void) {
    print_str("mirror: list ("); print_int(mirror_count); print_str(" mirrors)\n");
    for (int i = 0; i < mirror_count; i++) {
        mirror_t *m = &mirrors[i];
        print_str("  ["); print_int(m->mirror_id); print_str("] ");
        print_str(m->name); print_str(" "); print_str(mirror_type_str(m->type));
        print_str(" prio="); print_int(m->priority);
        print_str(" "); print_str(m->enabled ? "enabled" : "disabled");
        print_str(" "); print_str(m->url); print_str("\n");
    }
}

/* ── publishing ── */

static int publish_package(const char *name, const char *ver) {
    int idx = find_package(name);
    if (idx < 0) return -1;
    package_t *p = &packages[idx];
    print_str("publish: building "); print_str(p->name);
    print_str(" v"); print_str(p->version); print_str("\n");
    print_str("publish: signing  ");
    switch (p->sign_type) {
        case SIGN_GPG: print_str("GPG"); break;
        case SIGN_COSIGN: print_str("cosign"); break;
        default: print_str("none"); break;
    }
    print_str("\n");
    print_str("publish: releasing "); print_str(p->name);
    print_str(" v"); print_str(p->version);
    print_str(" ("); print_int(p->size_kb); print_str(" KB)\n");
    for (int i = 0; i < mirror_count; i++) {
        if (mirrors[i].enabled) {
            mirrors[i].pkg_count++;
            print_str("publish:   pushed to "); print_str(mirrors[i].name); print_str("\n");
        }
    }
    return 0;
}

/* ── installation ── */

static int install_package(const char *name, int from_remote) {
    int idx = find_package(name);
    if (idx < 0) {
        print_str("install: '"); print_str(name); print_str("' not found\n"); return -1;
    }
    package_t *p = &packages[idx];
    if (p->status == PKG_INSTALLED) {
        print_str("install: "); print_str(name); print_str(" already installed\n"); return 0;
    }
    if (from_remote) {
        int hit = 0;
        for (int c = 0; c < cache_count; c++) {
            if (my_strcmp(cache[c].name, name) == 0) {
                print_str("install: cache HIT for "); print_str(name); print_str("\n");
                hit = 1; break;
            }
        }
        if (!hit) {
            print_str("install: fetching "); print_str(name);
            print_str(" from remote\n");
            if (cache_count < MAX_CACHE) {
                copy_string(cache[cache_count].name, name, MAX_NAME);
                copy_string(cache[cache_count].version, p->version, MAX_VERSION);
                cache[cache_count].size_kb = p->size_kb;
                cache[cache_count].cached_at = pkg_count;
                cache_count++;
            }
        }
    } else {
        print_str("install: local install "); print_str(name); print_str("\n");
    }
    p->status = PKG_INSTALLED;
    print_str("install: "); print_str(p->name); print_str(" v");
    print_str(p->version); print_str(" installed OK\n");
    return 0;
}

/* ── search ── */

static int search_packages(const char *pattern) {
    int found = 0;
    print_str("search: '"); print_str(pattern); print_str("'\n");
    for (int i = 0; i < pkg_count; i++) {
        package_t *p = &packages[i];
        unsigned int plen = my_strlen(pattern);
        unsigned int nlen = my_strlen(p->name);
        unsigned int dlen = my_strlen(p->desc);
        int match = 0;
        for (unsigned int j = 0; j + plen <= nlen; j++) {
            int ok = 1;
            for (unsigned int k = 0; k < plen; k++) {
                if (p->name[j + k] != pattern[k]) { ok = 0; break; }
            }
            if (ok) { match = 1; break; }
        }
        if (!match) {
            for (unsigned int j = 0; j + plen <= dlen; j++) {
                int ok = 1;
                for (unsigned int k = 0; k < plen; k++) {
                    if (p->desc[j + k] != pattern[k]) { ok = 0; break; }
                }
                if (ok) { match = 1; break; }
            }
        }
        if (match) {
            print_str("  "); print_str(p->name);
            print_str(" v"); print_str(p->version);
            print_str(" ["); print_str(status_str(p->status)); print_str("] ");
            print_str(p->desc); print_str("\n");
            found++;
        }
    }
    print_str("search: found "); print_int(found); print_str(" packages\n");
    return found;
}

/* ── dependency tree visualization ── */

static void print_tree(int pkg_idx, int depth, int last) {
    if (depth > 4 || pkg_idx < 0) return;
    package_t *p = &packages[pkg_idx];
    for (int i = 0; i < depth; i++) {
        if (i == depth - 1) print_str(last ? "└─" : "├─");
        else print_str("  ");
    }
    print_str(p->name); print_str(" v"); print_str(p->version);
    print_str(" ["); print_str(status_str(p->status)); print_str("]\n");
    for (int d = 0; d < p->deps_count; d++) {
        int dep_idx = find_package(p->deps[d]);
        int is_last = (d == p->deps_count - 1);
        for (int i = 0; i < depth; i++) print_str("  ");
        if (depth > 0) print_str(last ? "  " : "│ ");
        print_tree(dep_idx, depth + 1, is_last);
    }
}

static void show_dep_tree(const char *name) {
    int idx = find_package(name);
    if (idx < 0) {
        print_str("tree: '"); print_str(name); print_str("' not found\n"); return;
    }
    print_str("dep-tree: "); print_str(name); print_str("\n");
    print_tree(idx, 0, 1);
}

/* ── CLI helpers ── */

static void show_help(void) {
    print_str("package_manager: Package management (v1.0)\n");
    print_str("  -t test  -l list  -v verbose  -i info  -s search  -m mirrors\n");
}
static void show_info(void) {
    print_str("pkg_manager: packages="); print_int(pkg_count);
    print_str(" mirrors="); print_int(mirror_count);
    print_str(" cache="); print_int(cache_count); print_str("\n");
}
static void list_all(int verbose) {
    print_str("packages: "); print_int(pkg_count);
    print_str("/"); print_int(MAX_PACKAGES); print_str("\n");
    for (int i = 0; i < pkg_count; i++) {
        package_t *p = &packages[i];
        print_str("  "); print_str(p->name); print_str(" v");
        print_str(p->version); print_str(" ["); print_str(status_str(p->status));
        print_str("] "); print_int(p->size_kb); print_str("KB");
        if (verbose) {
            print_str(" deps="); print_int(p->deps_count);
            for (int d = 0; d < p->deps_count; d++) {
                print_str(" "); print_str(p->deps[d]);
                print_str(op_str(p->constraints[d].op));
                print_int(p->constraints[d].major); print_str(".");
                print_int(p->constraints[d].minor); print_str(".");
                print_int(p->constraints[d].patch);
            }
        }
        print_str("\n");
    }
}

/* ── test mode ── */

static void test_mode(int verbose) {
    print_str("package_manager: testing\n");

    print_str("package_manager: --- mirrors ---\n");
    add_mirror("official-main", "https://pkg.aiasm.io/main", MIRROR_OFFICIAL, 100);
    add_mirror("community",     "https://community.pkg.io/v2", MIRROR_THIRDPARTY, 50);
    add_mirror("internal",      "https://internal.corp/repo", MIRROR_PRIVATE, 200);
    if (verbose) list_mirrors();

    print_str("package_manager: --- register packages ---\n");
    int id_libc    = register_package("libc",       "2.38.0", "C standard library",     SIGN_GPG, 4096, "core");
    int id_libssl  = register_package("libssl",     "3.2.0",  "TLS/SSL crypto library", SIGN_GPG, 2048, "crypto");
    int id_libcurl = register_package("libcurl",    "8.5.0",  "HTTP client library",    SIGN_GPG, 1024, "network");
    int id_zlib    = register_package("zlib",       "1.3.0",  "Compression library",    SIGN_COSIGN, 512, "core");
    int id_wget    = register_package("wget",       "2.1.0",  "Network file retriever", SIGN_COSIGN, 768, "network");
    int id_git     = register_package("git",        "2.43.0", "Distributed VCS",        SIGN_GPG, 8192, "devel");
    int id_python  = register_package("python3",    "3.12.0", "Python interpreter",     SIGN_GPG, 16384, "lang");
    int id_numpy   = register_package("numpy",      "1.26.0", "Numerical computing",    SIGN_COSIGN, 12288, "science");
    (void)id_libc; (void)id_libssl; (void)id_libcurl; (void)id_zlib;
    (void)id_wget; (void)id_git; (void)id_python; (void)id_numpy;
    print_str("register: "); print_int(pkg_count); print_str(" packages OK\n");

    print_str("package_manager: --- dependencies ---\n");
    add_dependency(id_libssl, "libc",   VC_GE, 2, 35, 0);
    add_dependency(id_libcurl,"libssl", VC_GE, 3, 0, 0);
    add_dependency(id_libcurl,"zlib",   VC_GE, 1, 2, 0);
    add_dependency(id_wget,   "libcurl",VC_GE, 8, 0, 0);
    add_dependency(id_wget,   "libssl", VC_GE, 3, 0, 0);
    add_dependency(id_git,    "libcurl",VC_GE, 7, 80, 0);
    add_dependency(id_git,    "zlib",   VC_GE, 1, 2, 0);
    add_dependency(id_python, "libc",   VC_GE, 2, 34, 0);
    add_dependency(id_numpy,  "python3",VC_GE, 3, 9, 0);
    print_str("dependencies: configured OK\n");

    print_str("package_manager: --- resolve libcurl ---\n");
    int r1 = resolve_dependencies("libcurl");
    if (r1 == 0) print_str("resolve libcurl: OK\n");

    print_str("package_manager: --- resolve numpy ---\n");
    int r2 = resolve_dependencies("numpy");
    if (r2 == 0) print_str("resolve numpy: OK\n");

    print_str("package_manager: --- conflict detection ---\n");
    detect_conflicts("wget", "git");
    detect_conflicts("numpy", "libcurl");

    print_str("package_manager: --- publish ---\n");
    publish_package("libcurl", "8.5.0");
    publish_package("wget", "2.1.0");

    print_str("package_manager: --- install ---\n");
    install_package("libc", 0);
    install_package("libssl", 0);
    install_package("libcurl", 1);
    install_package("wget", 1);
    install_package("python3", 1);
    install_package("numpy", 1);

    print_str("package_manager: --- search ---\n");
    search_packages("lib");
    search_packages("network");

    print_str("package_manager: --- dep tree ---\n");
    show_dep_tree("wget");
    print_str("\n");
    show_dep_tree("numpy");

    if (verbose) list_all(1);
    list_mirrors();
    show_info();
    print_str("package_manager: test complete\n");
}

/* ── entry point ── */

void _start(void) {
    unsigned int buf = alloc(MAX_INPUT);
    host_get_argv(buf, MAX_INPUT);
    int help = 0, test = 0, verbose = 0, list = 0, info = 0, mirrors = 0;
    unsigned int pos = 0; char *ap = (char *)buf;
    while (pos < MAX_INPUT && ap[pos]) pos++; pos++;
    while (pos < MAX_INPUT && ap[pos]) {
        char *a = &ap[pos];
        if (my_strcmp(a, "-h") == 0 || my_strcmp(a, "--help") == 0) help = 1;
        else if (my_strcmp(a, "-t") == 0) test = 1;
        else if (my_strcmp(a, "-v") == 0) verbose = 1;
        else if (my_strcmp(a, "-l") == 0) list = 1;
        else if (my_strcmp(a, "-i") == 0) info = 1;
        else if (my_strcmp(a, "-m") == 0) mirrors = 1;
        while (pos < MAX_INPUT && ap[pos]) pos++; pos++;
    }
    if (help)    { show_help(); host_exit(0); }
    if (info)    { show_info(); host_exit(0); }
    if (mirrors) { list_mirrors(); host_exit(0); }
    if (test)    { test_mode(verbose); host_exit(0); }
    if (list)    { list_all(verbose); host_exit(0); }
    print_str("package_manager: usage: package_manager [-t -l -v -i -m]\n");
    host_exit(0);
}
