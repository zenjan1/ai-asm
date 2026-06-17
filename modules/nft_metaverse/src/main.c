/* nft_metaverse - NFT and Metaverse Platform for AI-ASM OS */
#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MN 64
#define MU 128
#define MT 64
#define ML 32
#define MA 32
#define MC 16
#define MI 32
#define ME 16
#define MP 16
#define MH 8
#define MW 32
#define MM 64
#define MS 16
#define MR 32

static void platform_init(void);
static void demo_collections(void);
static void demo_minting(void);
static void demo_virtual_world(void);
static void demo_avatars(void);
static void demo_land_parcels(void);
static void demo_marketplace(void);
static void demo_events(void);
static void demo_governance(void);
static void demo_social(void);
static void demo_interop(void);
static void print_stats(void);

static const char *HX = "0123456789ABCDEF";
static void pr_uint(unsigned int v) {
    char b[16]; int i = 15; b[15] = '\0';
    if (v == 0) { b[0] = '0'; b[1] = '\0'; host_print(b); return; }
    while (v > 0 && i >= 0) { b[i] = HX[v & 0xF]; v >>= 4; i--; }
    i++; int j = 0;
    while (i < 16) { b[j] = b[i]; j++; i++; }
    b[j] = '\0'; host_print(b);
}
static void sc(char *d, const char *s, int m) {
    int i = 0; while (i < m - 1 && s[i]) { d[i] = s[i]; i++; } d[i] = '\0';
}
static void *ab(unsigned int sz) {
    unsigned int p = host_alloc(sz, 4);
    if (!p) { host_print("FATAL: alloc failed\n"); host_exit(1); }
    return (void *)p;
}

enum nft_std { NFT_721 = 0x721, NFT_1155 = 0x1155, NFT_DYN = 0xD99 };
enum asset_cat { CAT_ART = 0, CAT_GAME = 1, CAT_LAND = 2, CAT_MEDIA = 3, CAT_COLL = 4 };
enum list_ty { LST_FIX = 0, LST_AUC = 1, LST_BID = 2 };
enum scene_ty { SCN_VOX = 0, SCN_MESH = 1, SCN_PROC = 2 };
enum event_ty { EVT_CON = 0, EVT_EXPO = 1, EVT_MEET = 2, EVT_CONF = 3 };
enum prop_st { PS_ACT = 0, PS_PASS = 1, PS_REJ = 2 };
enum zone_ty { ZN_RES = 0, ZN_COM = 1, ZN_ENT = 2, ZN_IND = 3 };

struct royalty { unsigned int recip; unsigned int bp; };
struct tmeta { char uri[MU]; char name[MN]; char desc[MN]; unsigned int ed; unsigned int tot; };
struct nft {
    unsigned int tid; unsigned int oid; unsigned int cid;
    enum nft_std std; enum asset_cat cat; struct tmeta meta;
    struct royalty roy; unsigned int sup; unsigned int blk;
    unsigned int dflags; unsigned int estage; int listed; unsigned int lprice;
};
struct col {
    unsigned int cid; char name[MN]; char sym[16]; unsigned int cr;
    enum nft_std std; unsigned int tc; unsigned int bsup; unsigned int rbp;
    unsigned int blk; unsigned int fl;
};
struct parcel {
    unsigned int pid; int cx; int cy; int cz; unsigned int oid;
    unsigned int w; unsigned int d; unsigned int h; enum zone_ty zn;
    unsigned int bperm; unsigned int dev; unsigned int lp; int listed;
};
struct wear { unsigned int wid; unsigned int nid; char slot[16]; };
struct avatar {
    unsigned int aid; unsigned int oid; char name[MN];
    struct wear wlist[MW]; unsigned int wc;
    float px; float py; float pz; unsigned int anim; unsigned int rep;
};
struct scene {
    unsigned int sid; char name[MN]; enum scene_ty st;
    unsigned int vox; unsigned int msh; unsigned int lit;
    float grav; int coll; unsigned int prefs[ML]; unsigned int prc;
};
struct body {
    unsigned int bid; float px; float py; float pz;
    float vx; float vy; float vz; float mass; int st; int hc;
};
struct listing {
    unsigned int lid; unsigned int tid; unsigned int sid;
    enum list_ty lt; unsigned int pr; unsigned int mb; unsigned int cb;
    unsigned int hb; unsigned int exp; int act;
};
struct trade {
    unsigned int tid; unsigned int nid; unsigned int fid; unsigned int toid;
    unsigned int pr; unsigned int roy; unsigned int blk;
};
struct event {
    unsigned int eid; char name[MN]; enum event_ty et; unsigned int org;
    unsigned int pid; unsigned int sb; unsigned int dur; unsigned int att;
    unsigned int max; unsigned int tp; unsigned int rev; int act;
};
struct prop {
    unsigned int pid; char title[MN]; char desc[MN]; unsigned int pro;
    enum prop_st st; unsigned int vf; unsigned int va; unsigned int qu;
    unsigned int sb; unsigned int eb; unsigned int xb;
};
struct chan { unsigned int cid; char name[MN]; unsigned int mc; int prox; float rng; };
struct cmsg { unsigned int sid; unsigned int cid; char txt[MN]; unsigned int ts; };
struct did { unsigned int uid; char uri[MU]; unsigned int rep; unsigned int vf; unsigned int cc; };
struct bridge {
    unsigned int bid; unsigned int tid; unsigned int sc; unsigned int dc;
    unsigned int lb; unsigned int ub; int done;
};
struct token_eco {
    unsigned int tid; char name[MN]; char sym[16]; unsigned int tsup;
    unsigned int circ; unsigned int stk; unsigned int rr; unsigned int gw;
};

struct state {
    struct col cols[MC]; unsigned int cc;
    struct nft toks[MT]; unsigned int tc;
    struct parcel prc[ML]; unsigned int pc;
    struct scene scn[MS]; unsigned int sc;
    struct avatar avt[MA]; unsigned int ac;
    struct body bds[MT]; unsigned int bc;
    struct token_eco eco;
    struct listing lst[MI]; unsigned int lc;
    struct trade trd[MR]; unsigned int trc;
    unsigned int vol; unsigned int roys;
    struct event evt[ME]; unsigned int ec;
    struct chan chn[MH]; unsigned int hc;
    struct cmsg msg[MM]; unsigned int mc;
    struct prop prp[MP]; unsigned int prpc;
    struct did dids[MA]; unsigned int dc;
    struct bridge brg[MT]; unsigned int brc;
    unsigned int nt; unsigned int np; unsigned int na; unsigned int ns;
    unsigned int nl; unsigned int ne; unsigned int npr; unsigned int nch;
    unsigned int ntr; unsigned int nbr; unsigned int blk; unsigned int fl;
};
static struct state *G;

static unsigned int mk_col(const char *nm, const char *sy, unsigned int cr,
    enum nft_std st, unsigned int bs, unsigned int rb, unsigned int fl) {
    if (G->cc >= MC) return 0;
    struct col *c = &G->cols[G->cc]; c->cid = G->cc + 1;
    sc(c->name, nm, MN); sc(c->sym, sy, 16); c->cr = cr; c->std = st;
    c->tc = 0; c->bsup = bs; c->rbp = rb; c->blk = G->blk; c->fl = fl; G->cc++;
    host_print("[COL] "); host_print(nm); host_print(" id=");
    pr_uint(c->cid); host_print("\n"); return c->cid;
}
static unsigned int mint(unsigned int cid, unsigned int oid, const char *nm,
    const char *uri, enum asset_cat cat, unsigned int sup) {
    if (G->tc >= MT) return 0;
    struct col *col = 0;
    for (unsigned int i = 0; i < G->cc; i++)
        if (G->cols[i].cid == cid) { col = &G->cols[i]; break; }
    if (!col) return 0;
    struct nft *t = &G->toks[G->tc]; G->nt++;
    t->tid = G->nt; t->oid = oid; t->cid = cid; t->std = col->std; t->cat = cat;
    t->sup = sup ? sup : col->bsup; t->blk = G->blk; t->dflags = 0; t->estage = 0;
    t->listed = 0; t->lprice = 0;
    sc(t->meta.name, nm, MN); sc(t->meta.uri, uri, MU);
    t->meta.ed = col->tc + 1; t->meta.tot = 0;
    t->roy.recip = col->cr; t->roy.bp = col->rbp;
    col->tc++; G->tc++;
    host_print("[MINT] "); pr_uint(t->tid); host_print(" '"); host_print(nm);
    host_print("' sup="); pr_uint(t->sup); host_print(" std=");
    pr_uint((unsigned int)t->std); host_print("\n"); return t->tid;
}
static int xfer(unsigned int tid, unsigned int fid, unsigned int toid) {
    for (unsigned int i = 0; i < G->tc; i++) {
        struct nft *t = &G->toks[i];
        if (t->tid == tid) {
            if (t->oid != fid) return 0;
            t->oid = toid; t->listed = 0; return 1;
        }
    }
    return 0;
}
static void dyn_upd(unsigned int tid, unsigned int df, unsigned int es) {
    for (unsigned int i = 0; i < G->tc; i++) {
        struct nft *t = &G->toks[i];
        if (t->tid == tid) { t->dflags = df; t->estage = es; return; }
    }
}
static unsigned int roy_calc(unsigned int tid, unsigned int sp) {
    for (unsigned int i = 0; i < G->tc; i++)
        if (G->toks[i].tid == tid) return (sp * G->toks[i].roy.bp) / 10000;
    return 0;
}

static unsigned int mk_parcel(int cx, int cy, int cz, unsigned int oid,
    unsigned int w, unsigned int d, unsigned int h, enum zone_ty zn, unsigned int bp) {
    if (G->pc >= ML) return 0;
    G->np++; struct parcel *p = &G->prc[G->pc];
    p->pid = G->np; p->cx = cx; p->cy = cy; p->cz = cz; p->oid = oid;
    p->w = w; p->d = d; p->h = h; p->zn = zn; p->bperm = bp;
    p->dev = 0; p->listed = 0; G->pc++;
    host_print("[LAND] "); pr_uint(p->pid); host_print(" zn=");
    pr_uint((unsigned int)zn); host_print("\n"); return p->pid;
}
static unsigned int mk_scene(const char *nm, enum scene_ty st, float gv, int cl) {
    if (G->sc >= MS) return 0;
    G->ns++; struct scene *s = &G->scn[G->sc];
    s->sid = G->ns; sc(s->name, nm, MN); s->st = st;
    s->vox = 0; s->msh = 0; s->lit = 0; s->grav = gv; s->coll = cl; s->prc = 0;
    G->sc++; host_print("[SCENE] '"); host_print(nm); host_print("'\n"); return s->sid;
}
static void scn_link(unsigned int sid, unsigned int pid) {
    for (unsigned int i = 0; i < G->sc; i++) {
        struct scene *s = &G->scn[i];
        if (s->sid == sid && s->prc < ML) {
            s->prefs[s->prc] = pid; s->prc++; s->vox += 4096; s->lit += 3; return;
        }
    }
}
static unsigned int mk_avatar(unsigned int oid, const char *nm, float px, float py, float pz) {
    if (G->ac >= MA) return 0;
    G->na++; struct avatar *a = &G->avt[G->ac];
    a->aid = G->na; a->oid = oid; sc(a->name, nm, MN); a->wc = 0;
    a->px = px; a->py = py; a->pz = pz; a->anim = 0; a->rep = 100; G->ac++;
    host_print("[AVATAR] '"); host_print(nm); host_print("'\n"); return a->aid;
}
static void equip(unsigned int aid, unsigned int nid, const char *sl) {
    for (unsigned int i = 0; i < G->ac; i++) {
        struct avatar *a = &G->avt[i];
        if (a->aid == aid && a->wc < MW) {
            a->wlist[a->wc].wid = a->wc + 1; a->wlist[a->wc].nid = nid;
            sc(a->wlist[a->wc].slot, sl, 16); a->wc++; return;
        }
    }
}
static unsigned int mk_body(float px, float py, float pz, float m, int st) {
    if (G->bc >= MT) return 0;
    struct body *b = &G->bds[G->bc]; b->bid = G->bc + 1;
    b->px = px; b->py = py; b->pz = pz; b->vx = 0; b->vy = 0; b->vz = 0;
    b->mass = m; b->st = st; b->hc = 1; G->bc++; return b->bid;
}
static void phys_step(float dt) {
    unsigned int n = 0;
    for (unsigned int i = 0; i < G->bc; i++) {
        struct body *b = &G->bds[i]; if (b->st) continue;
        b->vy -= 9.81f * dt;
        b->px += b->vx * dt; b->py += b->vy * dt; b->pz += b->vz * dt;
        if (b->py < 0) { b->py = 0; b->vy = 0; } n++;
    }
    host_print("[PHYS] step "); pr_uint(n); host_print("\n");
}

static unsigned int mk_list(unsigned int tid, unsigned int sid, enum list_ty lt,
    unsigned int pr, unsigned int exp) {
    if (G->lc >= MI) return 0;
    G->nl++; struct listing *l = &G->lst[G->lc];
    l->lid = G->nl; l->tid = tid; l->sid = sid; l->lt = lt; l->pr = pr;
    l->mb = pr / 10; l->cb = 0; l->hb = 0; l->exp = G->blk + exp; l->act = 1; G->lc++;
    for (unsigned int i = 0; i < G->tc; i++)
        if (G->toks[i].tid == tid) { G->toks[i].listed = 1; G->toks[i].lprice = pr; break; }
    host_print("[LIST] "); pr_uint(l->lid); host_print(" pr="); pr_uint(pr); host_print("\n");
    return l->lid;
}
static int trade(unsigned int lid, unsigned int byr) {
    if (G->trc >= MR) return 0;
    struct listing *ls = 0;
    for (unsigned int i = 0; i < G->lc; i++)
        if (G->lst[i].lid == lid && G->lst[i].act) { ls = &G->lst[i]; break; }
    if (!ls) return 0;
    unsigned int sp = ls->pr; unsigned int ry = roy_calc(ls->tid, sp);
    G->ntr++; struct trade *tr = &G->trd[G->trc];
    tr->tid = G->ntr; tr->nid = ls->tid; tr->fid = ls->sid;
    tr->toid = byr; tr->pr = sp; tr->roy = ry; tr->blk = G->blk; G->trc++;
    xfer(ls->tid, ls->sid, byr); G->vol += sp; G->roys += ry; ls->act = 0;
    host_print("[TRADE] "); pr_uint(tr->tid); host_print(" pr="); pr_uint(sp);
    host_print(" roy="); pr_uint(ry); host_print("\n"); return 1;
}
static unsigned int mk_event(const char *nm, enum event_ty et, unsigned int org,
    unsigned int pid, unsigned int dur, unsigned int mx, unsigned int tp) {
    if (G->ec >= ME) return 0;
    G->ne++; struct event *e = &G->evt[G->ec];
    e->eid = G->ne; sc(e->name, nm, MN); e->et = et; e->org = org; e->pid = pid;
    e->sb = G->blk; e->dur = dur; e->att = 0; e->max = mx; e->tp = tp; e->rev = 0;
    e->act = 1; G->ec++;
    host_print("[EVT] '"); host_print(nm); host_print("'\n"); return e->eid;
}
static void attend(unsigned int eid, unsigned int uid) {
    for (unsigned int i = 0; i < G->ec; i++) {
        struct event *e = &G->evt[i];
        if (e->eid == eid && e->act && e->att < e->max) { e->att++; e->rev += e->tp; return; }
    }
}
static unsigned int mk_prop(const char *ti, const char *de, unsigned int pro,
    unsigned int dur, unsigned int qu) {
    if (G->prpc >= MP) return 0;
    G->npr++; struct prop *p = &G->prp[G->prpc];
    p->pid = G->npr; sc(p->title, ti, MN); sc(p->desc, de, MN);
    p->pro = pro; p->st = PS_ACT; p->vf = 0; p->va = 0; p->qu = qu;
    p->sb = G->blk; p->eb = G->blk + dur; p->xb = 0; G->prpc++;
    host_print("[GOV] '"); host_print(ti); host_print("' qu="); pr_uint(qu); host_print("\n");
    return p->pid;
}
static void do_vote(unsigned int pid, unsigned int wt, int fav) {
    for (unsigned int i = 0; i < G->prpc; i++) {
        struct prop *p = &G->prp[i];
        if (p->pid == pid && p->st == PS_ACT) {
            if (fav) p->vf += wt; else p->va += wt; return;
        }
    }
}
static void resolve(unsigned int pid) {
    for (unsigned int i = 0; i < G->prpc; i++) {
        struct prop *p = &G->prp[i];
        if (p->pid == pid && p->st == PS_ACT) {
            if (p->vf + p->va >= p->qu && p->vf > p->va) {
                p->st = PS_PASS; p->xb = G->blk + 10;
                host_print("[GOV] "); pr_uint(pid); host_print(" PASSED\n");
            } else {
                p->st = PS_REJ;
                host_print("[GOV] "); pr_uint(pid); host_print(" REJECTED\n");
            }
            return;
        }
    }
}
static unsigned int mk_chan(const char *nm, int prox, float rng) {
    if (G->hc >= MH) return 0;
    G->nch++; struct chan *c = &G->chn[G->hc];
    c->cid = G->nch; sc(c->name, nm, MN); c->mc = 0; c->prox = prox; c->rng = rng;
    G->hc++; return c->cid;
}
static void send_msg(unsigned int cid, unsigned int sid, const char *txt) {
    if (G->mc >= MM) return;
    struct cmsg *m = &G->msg[G->mc];
    m->sid = sid; m->cid = cid; sc(m->txt, txt, MN); m->ts = G->blk; G->mc++;
}
static void mk_did(unsigned int uid, const char *uri, unsigned int rep) {
    if (G->dc >= MA) return;
    struct did *d = &G->dids[G->dc];
    d->uid = uid; sc(d->uri, uri, MU); d->rep = rep; d->vf = 0; d->cc = 0; G->dc++;
}
static unsigned int bridge(unsigned int tid, unsigned int src, unsigned int dst) {
    if (G->brc >= MT) return 0;
    G->nbr++; struct bridge *b = &G->brg[G->brc];
    b->bid = G->nbr; b->tid = tid; b->sc = src; b->dc = dst;
    b->lb = G->blk; b->ub = G->blk + 100; b->done = 0; G->brc++;
    host_print("[BRIDGE] "); pr_uint(tid); host_print(" "); pr_uint(src);
    host_print("->"); pr_uint(dst); host_print("\n"); return b->bid;
}

static void platform_init(void) {
    host_print("=== NFT METAVERSE PLATFORM ===\n");
    host_get_argv(0, 0);
    G->eco.tid = 1; sc(G->eco.name, "Metaverse Token", MN); sc(G->eco.sym, "MVT", 16);
    G->eco.tsup = 1000000000; G->eco.circ = 500000000; G->eco.stk = 200000000;
    G->eco.rr = 50; G->eco.gw = 1;
    G->blk = 1000; G->cc = 0; G->tc = 0; G->pc = 0; G->sc = 0; G->ac = 0;
    G->bc = 0; G->lc = 0; G->trc = 0; G->ec = 0; G->prpc = 0;
    G->hc = 0; G->mc = 0; G->dc = 0; G->brc = 0; G->vol = 0; G->roys = 0; G->fl = 0xFF00;
    host_print("Init block="); pr_uint(G->blk); host_print(" MVT supply=");
    pr_uint(G->eco.tsup); host_print("\n--------\n");
}

static void demo_collections(void) {
    host_print("--- Collections ---\n");
    mk_col("Genesis Art", "GART", 1, NFT_721, 1, 250, 0x01);
    mk_col("Battle Gear", "BGR", 2, NFT_1155, 100, 300, 0x03);
    mk_col("MetaLand Estates", "MLE", 3, NFT_721, 1, 500, 0x01);
    mk_col("Evolvers", "EVOL", 4, NFT_DYN, 1, 350, 0x06);
    mk_col("Sound Waves", "SWAV", 5, NFT_721, 10, 200, 0x05);
    mk_col("Champions Deck", "CHMP", 6, NFT_1155, 1000, 150, 0x03);
    host_print("--------\n");
}
static void demo_minting(void) {
    host_print("--- Minting ---\n");
    mint(1, 10, "Stellar Nebula", "ipfs://QmX7DEF01234567890ABCDEF0123456789ABCDEF01", CAT_ART, 1);
    mint(1, 11, "Fractal Dreams", "ipfs://QmY8EFG12345678901ABCDEF1234567890ABCDEF12", CAT_ART, 1);
    mint(1, 12, "Quantum Garden", "ar://z9Y8x7W6v5U4t3S2r1Q0pONMLKJIHGFEDCBA987654", CAT_ART, 1);
    mint(2, 20, "Plasma Rifle", "ipfs://QmA1B2C3D4E5F6789012345678ABCDEF012345678901", CAT_GAME, 50);
    mint(2, 20, "Dragon Armor", "ipfs://QmB2C3D4E5F6789012345678ABCDEF01234567890123", CAT_GAME, 25);
    mint(2, 21, "Shadow Dagger", "ipfs://QmC3D4E5F6789012345678ABCDEF0123456789012345", CAT_GAME, 100);
    mint(3, 30, "Downtown Plaza", "ipfs://QmD4E5F6789012345678ABCDEF012345678901234567", CAT_LAND, 1);
    mint(4, 40, "Evolver 001", "ipfs://QmE5F6789012345678ABCDEF0123456789012345678A", CAT_COLL, 1);
    mint(4, 41, "Evolver 002", "ipfs://QmF6789012345678ABCDEF01234567890123456789AB", CAT_COLL, 1);
    mint(5, 50, "Cosmic Symphony", "ipfs://QmG789012345678ABCDEF012345678901234567890AB", CAT_MEDIA, 10);
    mint(6, 60, "Legendary Warrior", "ipfs://QmH89012345678ABCDEF012345678901234567890ABC", CAT_COLL, 10);
    mint(6, 61, "Arcane Mage", "ipfs://QmI9012345678ABCDEF0123456789012345678901BCD", CAT_COLL, 5);
    dyn_upd(8, 0x01, 1); dyn_upd(9, 0x03, 2);
    host_print("Dynamic NFTs evolved\n--------\n");
}
static void demo_virtual_world(void) {
    host_print("--- Virtual World ---\n");
    unsigned int s1 = mk_scene("Central Hub", SCN_VOX, 9.81f, 1);
    unsigned int s2 = mk_scene("Ocean World", SCN_MESH, 9.81f, 1);
    unsigned int s3 = mk_scene("Procedural Cave", SCN_PROC, 4.5f, 1);
    mk_body(0.0f, 10.0f, 0.0f, 70.0f, 0); mk_body(5.0f, 0.0f, 5.0f, 0.0f, 1);
    mk_body(-3.0f, 20.0f, 7.0f, 5.0f, 0);
    for (int i = 0; i < 10; i++) phys_step(0.016f);
    for (unsigned int i = 0; i < G->bc; i++) {
        struct body *b = &G->bds[i];
        host_print("  body "); pr_uint(b->bid); host_print(" y=");
        pr_uint((unsigned int)(b->py < 0 ? -b->py : b->py));
        host_print(" st="); pr_uint((unsigned int)b->st); host_print("\n");
    }
    (void)s1; (void)s2; (void)s3; host_print("--------\n");
}
static void demo_avatars(void) {
    host_print("--- Avatars ---\n");
    unsigned int a1 = mk_avatar(100, "ExplorerOne", 10.0f, 0.0f, 20.0f);
    unsigned int a2 = mk_avatar(101, "BuilderMax", -5.0f, 0.0f, 15.0f);
    unsigned int a3 = mk_avatar(102, "ArtistNova", 0.0f, 0.0f, 30.0f);
    equip(a1, 4, "body"); equip(a1, 5, "head"); equip(a1, 6, "hand");
    equip(a2, 4, "body"); equip(a2, 6, "hand");
    equip(a3, 5, "head"); equip(a3, 7, "back");
    host_print("Wearables equipped\n--------\n");
}
static void demo_land_parcels(void) {
    host_print("--- Land Parcels ---\n");
    unsigned int p1 = mk_parcel(0, 0, 0, 200, 64, 64, 32, ZN_COM, 0xFF);
    unsigned int p2 = mk_parcel(64, 0, 0, 201, 32, 32, 16, ZN_RES, 0x0F);
    unsigned int p3 = mk_parcel(0, 64, 0, 202, 48, 48, 24, ZN_ENT, 0xFF);
    unsigned int p4 = mk_parcel(-32, -32, 0, 203, 128, 128, 64, ZN_IND, 0x3F);
    scn_link(1, p1); scn_link(1, p2); scn_link(2, p3); scn_link(3, p4);
    for (unsigned int i = 0; i < G->pc; i++) G->prc[i].dev = (i + 1) * 3;
    host_print("Parcels linked and developed\n--------\n");
}
static void demo_marketplace(void) {
    host_print("--- Marketplace ---\n");
    unsigned int l1 = mk_list(1, 10, LST_FIX, 5000, 1000);
    unsigned int l3 = mk_list(4, 20, LST_FIX, 3000, 500);
    unsigned int l5 = mk_list(10, 50, LST_FIX, 2000, 800);
    mk_list(2, 11, LST_AUC, 8000, 2000); mk_list(7, 30, LST_BID, 15000, 3000);
    mk_list(12, 60, LST_AUC, 1500, 1500);
    trade(l1, 300); trade(l3, 301); trade(l5, 302);
    host_print("Royalties: t1="); pr_uint(roy_calc(1, 5000));
    host_print(" t4="); pr_uint(roy_calc(4, 3000));
    host_print(" t10="); pr_uint(roy_calc(10, 2000)); host_print("\n--------\n");
}

static void demo_events(void) {
    host_print("--- Events ---\n");
    unsigned int e1 = mk_event("Neon Beats Fest", EVT_CON, 400, 3, 500, 10000, 100);
    unsigned int e2 = mk_event("Digital Art Expo", EVT_EXPO, 401, 1, 1000, 5000, 50);
    unsigned int e3 = mk_event("DAO Summit", EVT_CONF, 402, 2, 800, 2000, 200);
    unsigned int e4 = mk_event("Builder Meetup", EVT_MEET, 403, 1, 200, 500, 0);
    for (unsigned int u = 500; u < 550; u++) attend(e1, u);
    for (unsigned int u = 500; u < 530; u++) attend(e2, u);
    for (unsigned int u = 500; u < 520; u++) attend(e3, u);
    for (unsigned int u = 500; u < 510; u++) attend(e4, u);
    for (unsigned int i = 0; i < G->ec; i++) {
        host_print("  '"); host_print(G->evt[i].name); host_print("' att=");
        pr_uint(G->evt[i].att); host_print(" rev="); pr_uint(G->evt[i].rev);
        host_print("\n");
    }
    host_print("--------\n");
}
static void demo_governance(void) {
    host_print("--- Governance ---\n");
    unsigned int p1 = mk_prop("Treasury Alloc", "5M MVT dev grants", 1, 100, 1000);
    unsigned int p2 = mk_prop("Fee Reduce", "Cut fees 2.5% to 1.5%", 2, 100, 1000);
    unsigned int p3 = mk_prop("Edu Zone", "Add learning spaces", 3, 100, 500);
    do_vote(p1, 500, 1); do_vote(p1, 300, 1); do_vote(p1, 200, 0);
    do_vote(p2, 400, 0); do_vote(p2, 350, 0); do_vote(p2, 250, 1);
    do_vote(p3, 600, 1); do_vote(p3, 100, 0);
    resolve(p1); resolve(p2); resolve(p3);
    host_print("--------\n");
}
static void demo_social(void) {
    host_print("--- Social ---\n");
    unsigned int c1 = mk_chan("General", 0, 0);
    unsigned int c2 = mk_chan("Proximity", 1, 50.0f);
    unsigned int c3 = mk_chan("Traders", 0, 0);
    unsigned int c4 = mk_chan("Builders", 0, 0);
    send_msg(c1, 100, "Welcome!"); send_msg(c1, 101, "Hello!");
    send_msg(c2, 100, "Nearby!"); send_msg(c3, 102, "Selling art NFTs");
    send_msg(c3, 200, "Need land"); send_msg(c4, 101, "Tutorial live");
    send_msg(c4, 103, "Collab at block 2000");
    mk_did(100, "did:asm:0x1A2B3C4D5E6F7890ABCDEF0123456789", 950);
    mk_did(101, "did:asm:0x2B3C4D5E6F7890ABCDEF01234567890A", 870);
    mk_did(102, "did:asm:0x3C4D5E6F7890ABCDEF01234567890AB1", 720);
    mk_did(200, "did:asm:0x4D5E6F7890ABCDEF01234567890AB1C2", 600);
    host_print("--------\n");
}
static void demo_interop(void) {
    host_print("--- Interop ---\n");
    bridge(1, 1, 2); bridge(4, 1, 3); bridge(7, 2, 1);
    bridge(10, 1, 4); bridge(12, 3, 1);
    host_print("Bridged "); pr_uint(G->brc); host_print(" assets\n");
    host_print("ERC-721, ERC-1155, EIP-2981, DID, Bridge v1\n--------\n");
}
static void print_stats(void) {
    host_print("=== STATS ===\nBlock: "); pr_uint(G->blk); host_print("\n");
    host_print("Collections: "); pr_uint(G->cc); host_print("\nTokens: "); pr_uint(G->tc); host_print("\n");
    unsigned int c7 = 0, c11 = 0, cd = 0, ts = 0;
    for (unsigned int i = 0; i < G->tc; i++) {
        if (G->toks[i].std == NFT_721) c7++;
        else if (G->toks[i].std == NFT_1155) c11++;
        else if (G->toks[i].std == NFT_DYN) cd++;
        ts += G->toks[i].sup;
    }
    host_print("  721="); pr_uint(c7); host_print(" 1155="); pr_uint(c11);
    host_print(" dyn="); pr_uint(cd); host_print(" supply="); pr_uint(ts); host_print("\n");
    host_print("Parcels: "); pr_uint(G->pc); host_print(" Scenes: "); pr_uint(G->sc);
    host_print(" Avatars: "); pr_uint(G->ac); host_print(" Bodies: "); pr_uint(G->bc);
    host_print("\n");
    unsigned int al = 0;
    for (unsigned int i = 0; i < G->lc; i++) if (G->lst[i].act) al++;
    host_print("Listings active: "); pr_uint(al); host_print(" Trades: "); pr_uint(G->trc);
    host_print("\nVolume: "); pr_uint(G->vol); host_print(" Royalties: "); pr_uint(G->roys); host_print("\n");
    host_print("Events: "); pr_uint(G->ec); host_print("\n");
    unsigned int ta = 0, tr = 0;
    for (unsigned int i = 0; i < G->ec; i++) { ta += G->evt[i].att; tr += G->evt[i].rev; }
    host_print("  attendees="); pr_uint(ta); host_print(" rev="); pr_uint(tr); host_print("\n");
    host_print("Proposals: "); pr_uint(G->prpc); host_print("\n");
    unsigned int pp = 0, pr = 0, pa = 0;
    for (unsigned int i = 0; i < G->prpc; i++) {
        if (G->prp[i].st == PS_PASS) pp++;
        else if (G->prp[i].st == PS_REJ) pr++; else pa++;
    }
    host_print("  pass="); pr_uint(pp); host_print(" rej="); pr_uint(pr);
    host_print(" act="); pr_uint(pa); host_print("\n");
    host_print("Channels: "); pr_uint(G->hc); host_print(" Msgs: "); pr_uint(G->mc);
    host_print("\nDIDs: "); pr_uint(G->dc); host_print(" Bridges: "); pr_uint(G->brc);
    host_print("\nFlags: "); pr_uint(G->fl); host_print("\n=== DONE ===\n");
}

__attribute__((visibility("default")))
int main(void) {
    G = (struct state *)ab(sizeof(struct state));
    unsigned char *r = (unsigned char *)G;
    for (unsigned int i = 0; i < sizeof(struct state); i++) r[i] = 0;
    platform_init();
    demo_collections();
    demo_minting();
    demo_virtual_world();
    demo_avatars();
    demo_land_parcels();
    demo_marketplace();
    demo_events();
    demo_governance();
    demo_social();
    demo_interop();
    print_stats();
    return 0;
}
