#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_DOCS 64
#define MAX_USERS 32
#define MAX_VERSIONS 256
#define MAX_COMMENTS 128
#define MAX_TEMPLATES 48
#define MAX_BRANCHES 16
#define MAX_TAGS 32
#define MAX_OPS 512
#define MAX_CRDT_NODES 1024
#define MAX_CURSORS 256
#define MAX_PERMS 64
#define MAX_LINKS 32
#define MAX_THREADS 64
#define MAX_MENTIONS 128
#define MAX_CELLS 4096
#define MAX_SLIDES 64
#define MAX_MAP_NODES 256
#define MAX_WB_ELEMS 512

#define DOC_TEXT 0x01
#define DOC_SHEET 0x02
#define DOC_SLIDES 0x03
#define DOC_MAP 0x04
#define DOC_BOARD 0x05
#define OP_INSERT 0x01
#define OP_DELETE 0x02
#define OP_FORMAT 0x03
#define OP_MOVE 0x04
#define OP_RESIZE 0x05
#define P_READ 0x01
#define P_WRITE 0x02
#define P_COMMENT 0x04
#define P_SHARE 0x08
#define P_ADMIN 0x10
#define S_ACTIVE 0x01
#define S_RESOLVED 0x02
#define S_DELETED 0x03

typedef struct { unsigned int id, uid, type, ts, site, sv; int pos; unsigned int len; } ot_op_t;
typedef struct { unsigned int id, pid, uid, lmp, site; int prio; unsigned int del; } crdt_node_t;
typedef struct { unsigned int uid, ctype; int px, py, ss, se; unsigned int color, last; } cursor_t;
typedef struct { unsigned int uid, join, last, online, color; } presence_t;
typedef struct { unsigned int uid, did, perm, by, at, exp; } perm_t;
typedef struct { unsigned int lid, did, perm, phash, exp, maxu, used, by, at, active; } link_t;
typedef struct { unsigned int cid, did, uid; int as, ae; unsigned int at, type, st, rby, rat; } comment_t;
typedef struct { unsigned int tid, cid, uid, pt, at; unsigned int ments[MAX_MENTIONS], mcnt; } thread_t;
typedef struct { unsigned int vid, did, uid, ts, ops, hash, pv; char lbl[64]; } ver_t;
typedef struct { unsigned int bid, did, bv, hv, by, at, mg; char nm[32]; } branch_t;
typedef struct { unsigned int tid, did, vid, by, at; char nm[32]; } tag_t;
typedef struct { unsigned int id; int r, c, sh; char val[128], fm[128]; unsigned int fmt; } cell_t;
typedef struct { unsigned int id, idx, lay, bg, tr, dur; char ttl[128]; } slide_t;
typedef struct { unsigned int id, pid; int px, py; unsigned int exp; char lbl[64]; } map_node_t;
typedef struct { unsigned int id, tp; int px, py, w, h; unsigned int fill, strk, zo, lk; } wb_elem_t;
typedef struct { unsigned int id, dt, by, at, uses, cat, sh; char nm[64], desc[256]; } tmpl_t;
typedef struct { unsigned int id, dt, ow, cat, mat, vc, ops, au, cn, cc, pc, ist; char ttl[128], cont[4096]; } doc_t;

static doc_t g_docs[MAX_DOCS]; static unsigned int g_dcnt;
static presence_t g_users[MAX_USERS]; static unsigned int g_ucnt;
static ot_op_t g_ops[MAX_OPS]; static unsigned int g_ocnt, g_otp;
static crdt_node_t g_crdt[MAX_CRDT_NODES]; static unsigned int g_ccnt;
static cursor_t g_curs[MAX_CURSORS]; static unsigned int g_curcnt;
static perm_t g_perms[MAX_PERMS]; static unsigned g_pcnt;
static link_t g_links[MAX_LINKS]; static unsigned int g_lcnt;
static comment_t g_cmts[MAX_COMMENTS]; static unsigned int g_cmncnt;
static thread_t g_thr[MAX_THREADS]; static unsigned int g_thcnt;
static ver_t g_vers[MAX_VERSIONS]; static unsigned int g_vcnt;
static branch_t g_brn[MAX_BRANCHES]; static unsigned int g_bcnt;
static tag_t g_tags[MAX_TAGS]; static unsigned int g_tcnt;
static cell_t g_cells[MAX_CELLS]; static unsigned int g_clcnt;
static slide_t g_slds[MAX_SLIDES]; static unsigned int g_slcnt;
static map_node_t g_mnodes[MAX_MAP_NODES]; static unsigned int g_mcnt;
static wb_elem_t g_wbe[MAX_WB_ELEMS]; static unsigned int g_wcnt;
static tmpl_t g_tmpl[MAX_TEMPLATES]; static unsigned int g_tmcnt;

/* Forward declarations */
static unsigned int mk_id(unsigned int base, unsigned int cnt);
static int find_doc(unsigned int did);
static int find_usr(unsigned int uid);
static void ot_apply(unsigned int did, ot_op_t *op);
static int ot_xform(ot_op_t *a, ot_op_t *b, ot_op_t *r);
static void crdt_ins(unsigned int did, unsigned int pid, unsigned int uid, int pr);
static void crdt_del(unsigned int nid, unsigned int uid);
static void sync_cur(unsigned int uid, unsigned int did, int px, int py);
static void set_online(unsigned int uid, unsigned int on);
static int chk_perm(unsigned int uid, unsigned int did, unsigned int rq);
static void grant(unsigned int uid, unsigned int did, unsigned int p, unsigned int by);
static void revoke(unsigned int uid, unsigned int did);
static unsigned int mk_link(unsigned int did, unsigned int p, unsigned int ph, unsigned int exp, unsigned int by);
static int val_link(unsigned int lid, unsigned int ph);
static unsigned int mk_cmt(unsigned int did, unsigned int uid, int as, int ae, unsigned int tp);
static unsigned int mk_thr(unsigned int cid, unsigned int uid, unsigned int pt);
static void resolve_cmt(unsigned int cid, unsigned int uid);
static void add_ment(unsigned int tid, unsigned int muid);
static unsigned int mk_ver(unsigned int did, unsigned int uid, const char *lbl);
static int diff_ver(unsigned int va, unsigned int vb);
static int rollback(unsigned int did, unsigned int tv, unsigned int uid);
static unsigned int mk_branch(unsigned int did, unsigned int bv, unsigned int uid, const char *nm);
static int merge_branch(unsigned int bid, unsigned int uid);
static unsigned int mk_tag(unsigned int did, unsigned int vid, unsigned int uid, const char *nm);
static unsigned int mk_doc(unsigned int dt, unsigned int ow, const char *ttl);
static unsigned int mk_cell(unsigned int did, int r, int c, const char *v, const char *f);
static unsigned int mk_slide(unsigned int did, unsigned int lay, const char *ttl);
static unsigned int mk_mnode(unsigned int did, unsigned int pid, int px, int py, const char *lbl);
static unsigned int mk_wbelem(unsigned int did, unsigned int tp, int px, int py, int w, int h);
static unsigned int mk_tmpl(unsigned int dt, unsigned int by, const char *nm, const char *ds, unsigned int cat);
static int apply_tmpl(unsigned int tid, unsigned int did, unsigned int uid);
static void run(void);

static unsigned int mk_id(unsigned int base, unsigned int cnt) { return base | cnt; }

static int find_doc(unsigned int did) {
    for (unsigned int i = 0; i < g_dcnt; i++) if (g_docs[i].id == did) return (int)i;
    return -1;
}
static int find_usr(unsigned int uid) {
    for (unsigned int i = 0; i < g_ucnt; i++) if (g_users[i].uid == uid) return (int)i;
    return -1;
}

static void ot_apply(unsigned int did, ot_op_t *op) {
    int di = find_doc(did);
    if (di < 0) { host_print("OT: doc not found\n"); return; }
    if (g_ocnt >= MAX_OPS) { host_print("OT: buf full\n"); return; }
    g_otp++;
    op->id = mk_id(0x000E0000, g_otp);
    g_ops[g_ocnt++] = *op;
    g_docs[di].ops++;
    g_docs[di].mat = op->ts;
    host_print("OT: applied op\n");
}

static int ot_xform(ot_op_t *a, ot_op_t *b, ot_op_t *r) {
    if (!a || !b || !r) return -1;
    r->id = a->id; r->uid = a->uid; r->type = a->type;
    r->ts = a->ts; r->site = a->site; r->sv = a->sv;
    if (a->type == OP_INSERT && b->type == OP_INSERT) {
        r->pos = (b->pos <= a->pos) ? a->pos + (int)b->len : a->pos;
        r->len = a->len;
    } else if (a->type == OP_INSERT && b->type == OP_DELETE) {
        if (b->pos <= a->pos) {
            r->pos = ((a->pos - b->pos) < (int)b->len) ? b->pos : a->pos - (int)b->len;
        } else r->pos = a->pos;
        r->len = a->len;
    } else if (a->type == OP_DELETE && b->type == OP_INSERT) {
        r->pos = (b->pos <= a->pos) ? a->pos + (int)b->len : a->pos;
        r->len = a->len;
    } else if (a->type == OP_DELETE && b->type == OP_DELETE) {
        if (b->pos >= a->pos + (int)a->len) { r->pos = a->pos; r->len = a->len; }
        else if (a->pos >= b->pos + (int)b->len) { r->pos = a->pos - (int)b->len; r->len = a->len; }
        else {
            int os = (a->pos > b->pos) ? a->pos : b->pos;
            int oe = ((a->pos + (int)a->len) < (b->pos + (int)b->len)) ? (a->pos + (int)a->len) : (b->pos + (int)b->len);
            int ov = oe - os; if (ov < 0) ov = 0;
            r->pos = (a->pos < b->pos) ? a->pos : b->pos;
            r->len = a->len - (unsigned int)ov;
        }
    } else { r->pos = a->pos; r->len = a->len; }
    return 0;
}

static unsigned int crdt_lamp(unsigned int uid) {
    unsigned int mx = 0;
    for (unsigned int i = 0; i < g_ccnt; i++) if (g_crdt[i].lmp > mx) mx = g_crdt[i].lmp;
    int ui = find_usr(uid);
    return mx + 1 + ((ui >= 0 ? (unsigned int)ui : 0) * 0x10000);
}

static void crdt_ins(unsigned int did, unsigned int pid, unsigned int uid, int pr) {
    if (g_ccnt >= MAX_CRDT_NODES) { host_print("CRDT: full\n"); return; }
    crdt_node_t *n = &g_crdt[g_ccnt];
    n->id = mk_id(0x00CD0000, g_ccnt + 1);
    n->pid = pid; n->uid = uid; n->lmp = crdt_lamp(uid);
    n->site = (unsigned int)find_usr(uid); n->prio = pr; n->del = 0;
    g_ccnt++;
    int di = find_doc(did);
    if (di >= 0) g_docs[di].cn++;
    host_print("CRDT: inserted\n");
}

static void crdt_del(unsigned int nid, unsigned int uid) {
    (void)uid;
    for (unsigned int i = 0; i < g_ccnt; i++)
        if (g_crdt[i].id == nid) { g_crdt[i].del = 1; host_print("CRDT: tombstoned\n"); return; }
    host_print("CRDT: not found\n");
}

static void sync_cur(unsigned int uid, unsigned int did, int px, int py) {
    (void)did;
    for (unsigned int i = 0; i < g_curcnt; i++)
        if (g_curs[i].uid == uid) {
            g_curs[i].px = px; g_curs[i].py = py; g_curs[i].last = g_otp;
            host_print("Cursor: updated\n"); return;
        }
    if (g_curcnt >= MAX_CURSORS) { host_print("Cursor: full\n"); return; }
    cursor_t *c = &g_curs[g_curcnt++];
    c->uid = uid; c->ctype = 0x01; c->px = px; c->py = py;
    c->ss = px; c->se = px; c->color = 0x0000FF; c->last = g_otp;
    host_print("Cursor: registered\n");
}

static void set_online(unsigned int uid, unsigned int on) {
    int ui = find_usr(uid);
    if (ui >= 0) { g_users[ui].online = on; g_users[ui].last = g_otp;
        host_print(on ? "Online\n" : "Offline\n"); return; }
    if (g_ucnt >= MAX_USERS) { host_print("Users: full\n"); return; }
    presence_t *u = &g_users[g_ucnt++];
    u->uid = uid; u->join = g_otp; u->last = g_otp; u->online = on; u->color = 0x0000FF + g_ucnt * 0x10;
    host_print("Joined\n");
}

static int chk_perm(unsigned int uid, unsigned int did, unsigned int rq) {
    int di = find_doc(did);
    if (di < 0) return 0;
    if (g_docs[di].ow == uid) return 1;
    for (unsigned int i = 0; i < g_pcnt; i++)
        if (g_perms[i].uid == uid && g_perms[i].did == did && (g_perms[i].perm & rq) == rq) {
            if (g_perms[i].exp > 0 && g_perms[i].exp < g_otp) { host_print("ACL: expired\n"); return 0; }
            return 1;
        }
    return 0;
}

static void grant(unsigned int uid, unsigned int did, unsigned int p, unsigned int by) {
    if (g_pcnt >= MAX_PERMS) { host_print("ACL: full\n"); return; }
    for (unsigned int i = 0; i < g_pcnt; i++)
        if (g_perms[i].uid == uid && g_perms[i].did == did) {
            g_perms[i].perm |= p; g_perms[i].at = g_otp;
            host_print("ACL: updated\n"); return;
        }
    perm_t *pe = &g_perms[g_pcnt++];
    pe->uid = uid; pe->did = did; pe->perm = p; pe->by = by; pe->at = g_otp; pe->exp = 0;
    host_print("ACL: granted\n");
}

static void revoke(unsigned int uid, unsigned int did) {
    for (unsigned int i = 0; i < g_pcnt; i++)
        if (g_perms[i].uid == uid && g_perms[i].did == did) {
            g_perms[i].perm = 0; host_print("ACL: revoked\n"); return;
        }
    host_print("ACL: not found\n");
}

static unsigned int mk_link(unsigned int did, unsigned int p, unsigned int ph, unsigned int exp, unsigned int by) {
    if (g_lcnt >= MAX_LINKS) { host_print("Link: full\n"); return 0; }
    link_t *l = &g_links[g_lcnt++];
    l->lid = mk_id(0x00150000, g_lcnt); l->did = did; l->perm = p; l->phash = ph;
    l->exp = exp; l->maxu = 0; l->used = 0; l->by = by; l->at = g_otp; l->active = 1;
    host_print("Link: created\n"); return l->lid;
}

static int val_link(unsigned int lid, unsigned int ph) {
    for (unsigned int i = 0; i < g_lcnt; i++)
        if (g_links[i].lid == lid) {
            if (!g_links[i].active) { host_print("Link: inactive\n"); return 0; }
            if (g_links[i].exp > 0 && g_links[i].exp < g_otp) { host_print("Link: expired\n"); return 0; }
            if (g_links[i].phash && g_links[i].phash != ph) { host_print("Link: bad pw\n"); return 0; }
            g_links[i].used++;
            if (g_links[i].maxu && g_links[i].used > g_links[i].maxu) { host_print("Link: overused\n"); return 0; }
            return 1;
        }
    return 0;
}

static unsigned int mk_cmt(unsigned int did, unsigned int uid, int as, int ae, unsigned int tp) {
    if (g_cmncnt >= MAX_COMMENTS) { host_print("Cmt: full\n"); return 0; }
    comment_t *c = &g_cmts[g_cmncnt++];
    c->cid = mk_id(0x00C00000, g_cmncnt); c->did = did; c->uid = uid;
    c->as = as; c->ae = ae; c->type = tp; c->at = g_otp; c->st = S_ACTIVE; c->rby = 0; c->rat = 0;
    int di = find_doc(did); if (di >= 0) g_docs[di].cc++;
    host_print("Cmt: created\n"); return c->cid;
}

static unsigned int mk_thr(unsigned int cid, unsigned int uid, unsigned int pt) {
    if (g_thcnt >= MAX_THREADS) { host_print("Thr: full\n"); return 0; }
    thread_t *t = &g_thr[g_thcnt++];
    t->tid = mk_id(0x007A0000, g_thcnt); t->cid = cid; t->uid = uid; t->pt = pt; t->at = g_otp; t->mcnt = 0;
    host_print("Thr: created\n"); return t->tid;
}

static void resolve_cmt(unsigned int cid, unsigned int uid) {
    for (unsigned int i = 0; i < g_cmncnt; i++)
        if (g_cmts[i].cid == cid) { g_cmts[i].st = S_RESOLVED; g_cmts[i].rby = uid; g_cmts[i].rat = g_otp;
            host_print("Cmt: resolved\n"); return; }
    host_print("Cmt: not found\n");
}

static void add_ment(unsigned int tid, unsigned int muid) {
    for (unsigned int i = 0; i < g_thcnt; i++)
        if (g_thr[i].tid == tid) {
            if (g_thr[i].mcnt >= MAX_MENTIONS) { host_print("Ment: full\n"); return; }
            g_thr[i].ments[g_thr[i].mcnt++] = muid; host_print("Ment: added\n"); return;
        }
    host_print("Ment: not found\n");
}

static unsigned int mk_ver(unsigned int did, unsigned int uid, const char *lbl) {
    if (g_vcnt >= MAX_VERSIONS) { host_print("Ver: full\n"); return 0; }
    ver_t *v = &g_vers[g_vcnt++];
    v->vid = mk_id(0x00005E00, g_vcnt); v->did = did; v->uid = uid; v->ts = g_otp;
    v->ops = 0; v->hash = 0xDEADBEEF ^ did ^ uid ^ g_vcnt;
    v->pv = (g_vcnt > 1) ? g_vers[g_vcnt - 2].vid : 0;
    for (int j = 0; j < 63 && lbl[j]; j++) v->lbl[j] = lbl[j]; v->lbl[63] = '\0';
    int di = find_doc(did); if (di >= 0) g_docs[di].vc++;
    host_print("Ver: snapshot\n"); return v->vid;
}

static int diff_ver(unsigned int va, unsigned int vb) {
    if (va >= g_vcnt || vb >= g_vcnt) { host_print("Diff: range\n"); return -1; }
    if (g_vers[va].did != g_vers[vb].did) { host_print("Diff: mismatch\n"); return -1; }
    host_print("Diff: compared\n");
    return (int)g_vers[vb].ops - (int)g_vers[va].ops + (int)((g_vers[va].hash ^ g_vers[vb].hash) & 0xFF);
}

static int rollback(unsigned int did, unsigned int tv, unsigned int uid) {
    if (!chk_perm(uid, did, P_WRITE)) { host_print("RB: no perm\n"); return -1; }
    if (tv >= g_vcnt) { host_print("RB: no ver\n"); return -1; }
    if (g_vers[tv].did != did) { host_print("RB: wrong doc\n"); return -1; }
    ot_op_t op = { 0 }; g_otp++; op.id = mk_id(0x000E0000, g_otp);
    op.uid = uid; op.type = OP_FORMAT; op.ts = g_otp; op.sv = g_vers[tv].hash;
    ot_apply(did, &op); host_print("RB: restored\n"); return 0;
}

static unsigned int mk_branch(unsigned int did, unsigned int bv, unsigned int uid, const char *nm) {
    if (g_bcnt >= MAX_BRANCHES) { host_print("Br: full\n"); return 0; }
    branch_t *b = &g_brn[g_bcnt++];
    b->bid = mk_id(0x008E0000, g_bcnt); b->did = did; b->bv = bv; b->hv = bv;
    b->by = uid; b->at = g_otp; b->mg = 0;
    for (int j = 0; j < 31 && nm[j]; j++) b->nm[j] = nm[j]; b->nm[31] = '\0';
    host_print("Br: created\n"); return b->bid;
}

static int merge_branch(unsigned int bid, unsigned int uid) {
    for (unsigned int i = 0; i < g_bcnt; i++)
        if (g_brn[i].bid == bid) {
            if (g_brn[i].mg) { host_print("Br: merged\n"); return -1; }
            if (!chk_perm(uid, g_brn[i].did, P_WRITE)) { host_print("Br: no perm\n"); return -1; }
            g_brn[i].mg = 1; g_brn[i].hv = g_vcnt; host_print("Br: merged\n"); return 0;
        }
    host_print("Br: not found\n"); return -1;
}

static unsigned int mk_tag(unsigned int did, unsigned int vid, unsigned int uid, const char *nm) {
    if (g_tcnt >= MAX_TAGS) { host_print("Tag: full\n"); return 0; }
    tag_t *t = &g_tags[g_tcnt++];
    t->tid = mk_id(0x007B0000, g_tcnt); t->did = did; t->vid = vid; t->by = uid; t->at = g_otp;
    for (int j = 0; j < 31 && nm[j]; j++) t->nm[j] = nm[j]; t->nm[31] = '\0';
    host_print("Tag: created\n"); return t->tid;
}

static unsigned int mk_doc(unsigned int dt, unsigned int ow, const char *ttl) {
    if (g_dcnt >= MAX_DOCS) { host_print("Doc: full\n"); return 0; }
    doc_t *d = &g_docs[g_dcnt++];
    d->id = mk_id(0xD0C00000, g_dcnt); d->dt = dt; d->ow = ow; d->cat = g_otp; d->mat = g_otp;
    d->vc = 0; d->ops = 0; d->au = 0; d->cn = 0; d->cc = 0; d->pc = 0; d->ist = 0;
    for (int j = 0; j < 127 && ttl[j]; j++) d->ttl[j] = ttl[j]; d->ttl[127] = '\0'; d->cont[0] = '\0';
    grant(ow, d->id, P_READ | P_WRITE | P_COMMENT | P_SHARE | P_ADMIN, ow);
    mk_ver(d->id, ow, "initial"); host_print("Doc: created\n"); return d->id;
}

static unsigned int mk_cell(unsigned int did, int r, int c, const char *v, const char *f) {
    (void)did;
    if (g_clcnt >= MAX_CELLS) { host_print("Cell: full\n"); return 0; }
    cell_t *cl = &g_cells[g_clcnt++];
    cl->id = g_clcnt; cl->r = r; cl->c = c; cl->sh = 0; cl->fmt = 0;
    for (int j = 0; j < 127 && v[j]; j++) cl->val[j] = v[j]; cl->val[127] = '\0';
    for (int j = 0; j < 127 && f && f[j]; j++) cl->fm[j] = f[j]; cl->fm[127] = '\0';
    host_print("Cell: created\n"); return cl->id;
}

static unsigned int mk_slide(unsigned int did, unsigned int lay, const char *ttl) {
    (void)did;
    if (g_slcnt >= MAX_SLIDES) { host_print("Slide: full\n"); return 0; }
    slide_t *s = &g_slds[g_slcnt++];
    s->id = g_slcnt; s->idx = g_slcnt - 1; s->lay = lay; s->bg = 0xFFFFFF; s->tr = 0; s->dur = 500;
    for (int j = 0; j < 127 && ttl[j]; j++) s->ttl[j] = ttl[j]; s->ttl[127] = '\0';
    host_print("Slide: created\n"); return s->id;
}

static unsigned int mk_mnode(unsigned int did, unsigned int pid, int px, int py, const char *lbl) {
    (void)did;
    if (g_mcnt >= MAX_MAP_NODES) { host_print("Map: full\n"); return 0; }
    map_node_t *n = &g_mnodes[g_mcnt++];
    n->id = g_mcnt; n->pid = pid; n->px = px; n->py = py; n->exp = 1;
    for (int j = 0; j < 63 && lbl[j]; j++) n->lbl[j] = lbl[j]; n->lbl[63] = '\0';
    host_print("Map: created\n"); return n->id;
}

static unsigned int mk_wbelem(unsigned int did, unsigned int tp, int px, int py, int w, int h) {
    (void)did;
    if (g_wcnt >= MAX_WB_ELEMS) { host_print("WB: full\n"); return 0; }
    wb_elem_t *e = &g_wbe[g_wcnt++];
    e->id = g_wcnt; e->tp = tp; e->px = px; e->py = py; e->w = w; e->h = h;
    e->fill = 0x00000000; e->strk = 0x000000FF; e->zo = g_wcnt - 1; e->lk = 0;
    host_print("WB: created\n"); return e->id;
}

static unsigned int mk_tmpl(unsigned int dt, unsigned int by, const char *nm, const char *ds, unsigned int cat) {
    if (g_tmcnt >= MAX_TEMPLATES) { host_print("Tmpl: full\n"); return 0; }
    tmpl_t *t = &g_tmpl[g_tmcnt++];
    t->id = mk_id(0x007E0000, g_tmcnt); t->dt = dt; t->by = by; t->at = g_otp;
    t->uses = 0; t->cat = cat; t->sh = 1;
    for (int j = 0; j < 63 && nm[j]; j++) t->nm[j] = nm[j]; t->nm[63] = '\0';
    for (int j = 0; j < 255 && ds[j]; j++) t->desc[j] = ds[j]; t->desc[255] = '\0';
    host_print("Tmpl: registered\n"); return t->id;
}

static int apply_tmpl(unsigned int tid, unsigned int did, unsigned int uid) {
    int di = find_doc(did);
    if (di < 0) { host_print("Tmpl: no doc\n"); return -1; }
    for (unsigned int i = 0; i < g_tmcnt; i++)
        if (g_tmpl[i].id == tid) {
            if (g_tmpl[i].dt != g_docs[di].dt) { host_print("Tmpl: mismatch\n"); return -1; }
            g_tmpl[i].uses++;
            ot_op_t op = { 0 }; g_otp++; op.id = mk_id(0x000E0000, g_otp);
            op.uid = uid; op.type = OP_FORMAT; op.ts = g_otp;
            ot_apply(did, &op); host_print("Tmpl: applied\n"); return 0;
        }
    host_print("Tmpl: not found\n"); return -1;
}

static unsigned int str_copy(char *dst, const char *src, unsigned int max) {
    unsigned int i = 0;
    while (i < max && src[i] != '\0') { dst[i] = src[i]; i++; }
    if (i < max) dst[i] = '\0'; else if (max > 0) dst[max - 1] = '\0';
    return i;
}

static unsigned int hash_state(unsigned int did, unsigned int uid, unsigned int ts) {
    unsigned int h = 0x811C9DC5;
    h ^= did; h *= 0x01000193;
    h ^= uid; h *= 0x01000193;
    h ^= ts; h *= 0x01000193;
    return h;
}

static void doc_summary(unsigned int did) {
    int di = find_doc(did);
    if (di < 0) { host_print("Summary: doc not found\n"); return; }
    host_print("Summary: document status\n");
    host_print("  versions: tracked\n");
    host_print("  operations: applied\n");
    host_print("  comments: recorded\n");
    host_print("  crdt nodes: synced\n");
}

static unsigned int count_active_users(unsigned int did) {
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < g_ucnt; i++) if (g_users[i].online) cnt++;
    int di = find_doc(did);
    if (di >= 0) g_docs[di].au = cnt;
    return cnt;
}

static void run(void) {
    g_dcnt = g_ucnt = g_ocnt = g_otp = g_ccnt = g_curcnt = g_pcnt = 0;
    g_lcnt = g_cmncnt = g_thcnt = g_vcnt = g_bcnt = g_tcnt = 0;
    g_clcnt = g_slcnt = g_mcnt = g_wcnt = g_tmcnt = 0;
    host_print("document_collab: init\n");

    set_online(mk_id(0x005E0000, 1), 1);
    set_online(mk_id(0x005E0000, 2), 1);
    unsigned int u0 = g_users[0].uid, u1 = g_users[1].uid;

    unsigned int rt = mk_doc(DOC_TEXT, u0, "Collaborative Report");
    unsigned int ss = mk_doc(DOC_SHEET, u0, "Budget Tracker");
    unsigned int pr = mk_doc(DOC_SLIDES, u0, "Q4 Deck");
    unsigned int mm = mk_doc(DOC_MAP, u0, "Project Brainstorm");
    unsigned int wb = mk_doc(DOC_BOARD, u0, "Architecture Diagram");

    grant(u1, rt, P_READ | P_WRITE, u0);
    grant(u1, ss, P_READ, u0);
    mk_link(rt, P_READ | P_COMMENT, 0, 0, u0);
    mk_link(pr, P_READ, 0xABCD1234, g_otp + 1000, u0);

    ot_op_t o1 = { 0 }; o1.uid = u0; o1.type = OP_INSERT; o1.pos = 0; o1.len = 12; o1.ts = g_otp; o1.site = 0;
    ot_apply(rt, &o1);
    ot_op_t o2 = { 0 }; o2.uid = u1; o2.type = OP_INSERT; o2.pos = 5; o2.len = 8; o2.ts = g_otp; o2.site = 1;
    ot_op_t tx; ot_xform(&o2, &o1, &tx); ot_apply(rt, &tx);

    crdt_ins(rt, 0, u0, 1); crdt_ins(rt, 1, u1, 2); crdt_ins(rt, 1, u0, 3);
    sync_cur(u0, rt, 0, 12); sync_cur(u1, rt, 5, 8);

    mk_cell(ss, 0, 0, "Item", ""); mk_cell(ss, 0, 1, "Amount", "");
    mk_cell(ss, 1, 0, "Revenue", "=SUM(B2:B10)"); mk_cell(ss, 1, 1, "50000", "");
    mk_slide(pr, 0, "Title Slide"); mk_slide(pr, 1, "Agenda"); mk_slide(pr, 2, "Results");
    unsigned int rn = mk_mnode(mm, 0, 400, 300, "Project");
    mk_mnode(mm, rn, 200, 150, "Phase 1"); mk_mnode(mm, rn, 600, 150, "Phase 2"); mk_mnode(mm, rn, 400, 500, "Resources");
    mk_wbelem(wb, 0x01, 50, 50, 200, 100); mk_wbelem(wb, 0x02, 300, 50, 150, 150);

    unsigned int c1 = mk_cmt(rt, u0, 0, 12, 0x01);
    unsigned int t1 = mk_thr(c1, u0, 0); add_ment(t1, u1);
    mk_thr(c1, u1, t1); resolve_cmt(c1, u1);

    mk_ver(rt, u0, "v1.0 draft");
    unsigned int v2 = mk_ver(rt, u0, "v1.1 review");
    diff_ver(0, 1); rollback(rt, v2, u0);

    unsigned int br = mk_branch(rt, 0, u1, "feature-section2");
    merge_branch(br, u0); mk_tag(rt, v2, u0, "milestone-1");

    mk_tmpl(DOC_TEXT, u0, "Meeting Notes", "Template for meeting notes", 1);
    mk_tmpl(DOC_SHEET, u0, "Budget", "Monthly budget template", 2);
    mk_tmpl(DOC_SLIDES, u0, "Pitch Deck", "Startup pitch template", 3);
    apply_tmpl(1, rt, u1);

    val_link(2, 0xABCD1234); revoke(u1, ss); chk_perm(u1, ss, P_WRITE);
    count_active_users(rt);
    doc_summary(rt); doc_summary(ss);
    unsigned int h1 = hash_state(rt, u0, g_otp);
    char buf[64]; str_copy(buf, "document_collab_demo", 64);
    (void)h1; (void)buf;
    host_print("document_collab: complete\n");
}

__attribute__((export_name("main")))
int main(void) { run(); return 0; }
