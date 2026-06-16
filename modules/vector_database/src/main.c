/* vector_database: ANN Search Engine for AI Workloads (v60.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

#define MAX_DIM 128
#define MAX_VEC 512
#define MAX_TAG 32
#define MAX_COL 8
#define MAX_HNSW 256
#define HNSW_M 16
#define HNSW_LVL 4
#define MAX_CLUST 32
#define PQ_SUB 4
#define PQ_KSUB 16
#define MAX_RES 32
#define METRIC_L2 0
#define METRIC_COS 1
#define METRIC_IP 2
#define METRIC_HAM 3
#define IDX_NONE 0
#define IDX_HNSW 1
#define IDX_IVF 2

typedef struct { int id; float data[MAX_DIM]; int dim; char tag[MAX_TAG]; int scalar_val; int deleted; } vec_entry_t;
typedef struct { int level; int neighbors[HNSW_LVL][HNSW_M * 2]; int n_nb[HNSW_LVL]; } hnsw_node_t;
typedef struct { hnsw_node_t nodes[MAX_HNSW]; int count, entry, max_lvl, dim, metric; } hnsw_index_t;
typedef struct { float centroids[MAX_CLUST][MAX_DIM]; int assignments[MAX_VEC], list_sizes[MAX_CLUST], dim, n_clust; } ivf_index_t;
typedef struct { float codebooks[PQ_SUB][PQ_KSUB][MAX_DIM / PQ_SUB]; int codes[MAX_VEC][PQ_SUB]; int dim, trained; } pq_index_t;
typedef struct { float mins[MAX_DIM], maxs[MAX_DIM]; int dim; } sq_index_t;
typedef struct { int codes[MAX_VEC][MAX_DIM / 8], dim; } bq_index_t;
typedef struct { int vec_id; float dist; } search_result_t;
typedef struct { char name[MAX_TAG]; vec_entry_t vectors[MAX_VEC]; int vec_count, dim, metric, idx_type; hnsw_index_t hnsw; ivf_index_t ivf; pq_index_t pq; sq_index_t sq; bq_index_t bq; int hnsw_built, ivf_built, pq_built, sq_built; } vector_collection_t;
static vector_collection_t collections[MAX_COL];
static int col_count = 0;
static unsigned int heap_pos = 65536, rng_state = 54321;

/* Forward declarations */
static unsigned int bump_alloc(unsigned int n);
static void copy_to_mem(unsigned int dst, const char *src, unsigned int len);
static unsigned int my_strlen(const char *s);
static void my_strncpy(char *dst, const char *src, unsigned int n);
static int my_strcmp(const char *a, const char *b);
static void print_str(const char *s);
static void print_int(int v);
static void print_double(double v);
static double my_sqrt(double x);
static unsigned int rng_next(void);
static double rng_double(void);
static int rng_int(int mx);
static float distance_l2(const float *a, const float *b, int dim);
static float distance_cosine(const float *a, const float *b, int dim);
static float distance_inner_product(const float *a, const float *b, int dim);
static int distance_hamming(const int *a, const int *b, int nb);
static float compute_dist(const float *a, const float *b, int dim, int metric);
static void vec_copy(float *d, const float *s, int n);
static vector_collection_t *collection_create(const char *name, int dim, int metric);
static int collection_delete(const char *name);
static void collection_list(void);
static int vector_insert(vector_collection_t *c, const float *d, int dim, const char *tag, int sv);
static int vector_insert_batch(vector_collection_t *c, const float *d, int dim, int cnt, const char *pfx);
static int vector_delete(vector_collection_t *c, int vid);
static int vector_delete_batch(vector_collection_t *c, const int *ids, int cnt);
static int vector_update(vector_collection_t *c, int vid, const float *nd);
static int hnsw_random_level(void);
static void hnsw_build(vector_collection_t *c);
static void hnsw_insert(hnsw_index_t *idx, int nid, const float *vec, const float *all_vecs, int dim, int metric);
static int hnsw_layer_select(void);
static void hnsw_search(hnsw_index_t *idx, const float *q, int dim, int k, search_result_t *r, int *nr);
static void ivf_train_centroids(ivf_index_t *ivf, const float *vecs, int n, int dim, int k);
static void ivf_assign(ivf_index_t *ivf, const float *vec, int dim, int *cid);
static void ivf_build(vector_collection_t *c);
static void ivf_search(ivf_index_t *ivf, const float *vecs, int vc, const float *q, int dim, int k, search_result_t *r, int *nr);
static void pq_train(pq_index_t *pq, const float *vecs, int n, int dim);
static void pq_encode(pq_index_t *pq, const float *vec, int dim, int *codes);
static void pq_decode(pq_index_t *pq, const int *codes, int dim, float *out);
static float pq_distance(pq_index_t *pq, const int *ca, const int *cb, int dim);
static void sq_quantize(sq_index_t *sq, const float *vecs, int n, int dim);
static void sq_dequantize(sq_index_t *sq, const unsigned char *codes, int dim, float *out);
static void bq_quantize(bq_index_t *bq, const float *vecs, int n, int dim);
static int compact(vector_collection_t *c);
static void compact_rebuild(vector_collection_t *c);
static void ann_search(vector_collection_t *c, const float *q, int dim, int k, search_result_t *r, int *nr);
static void exact_search(vector_collection_t *c, const float *q, int dim, int k, search_result_t *r, int *nr);
static void topk_search(vector_collection_t *c, const float *q, int dim, int k, search_result_t *r, int *nr);
static void hybrid_query(vector_collection_t *c, const float *q, int dim, const char *tf, int smin, int smax, int k, search_result_t *r, int *nr);
static int filter_by_tag(vector_collection_t *c, const char *tag, int *out, int mx);
static int filter_by_scalar(vector_collection_t *c, int mn, int mx, int *out, int mo);
static void show_help(void);
static void test_suite(void);

static unsigned int bump_alloc(unsigned int n) { unsigned int p = heap_pos; heap_pos += n; heap_pos = (heap_pos + 15) & ~15u; return p; }
static void copy_to_mem(unsigned int dst, const char *src, unsigned int len) { char *d = (char *)dst; for (unsigned int i = 0; i < len; i++) d[i] = src[i]; }
static unsigned int my_strlen(const char *s) { unsigned int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *dst, const char *src, unsigned int n) { unsigned int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0'; }
static int my_strcmp(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return (unsigned char)*a - (unsigned char)*b; }
static void print_str(const char *s) { unsigned int len = my_strlen(s); if (len == 0) return; unsigned int off = bump_alloc(len); copy_to_mem(off, s, len); host_print((const char *)off); }
static void print_int(int v) {
    char buf[16]; int i = 0; if (v < 0) { print_str("-"); v = -v; } if (v == 0) { print_str("0"); return; }
    while (v > 0 && i < 15) { buf[i++] = '0' + (v % 10); v /= 10; } buf[i] = '\0';
    for (int j = 0; j < i / 2; j++) { char t = buf[j]; buf[j] = buf[i-1-j]; buf[i-1-j] = t; } print_str(buf);
}
static void print_double(double v) {
    int whole = (int)v; int frac = (int)((v - (double)whole) * 10000.0); if (frac < 0) frac = -frac;
    print_int(whole); print_str("."); char fb[8];
    for (int i = 3; i >= 0; i--) { fb[i] = '0' + (frac % 10); frac /= 10; } fb[4] = '\0'; print_str(fb);
}
static double my_sqrt(double x) { if (x <= 0.0) return 0.0; double r = x; for (int i = 0; i < 40; i++) r = (r + x / r) * 0.5; return r; }
static unsigned int rng_next(void) { rng_state ^= rng_state << 13; rng_state ^= rng_state >> 17; rng_state ^= rng_state << 5; return rng_state; }
static double rng_double(void) { return (double)(rng_next() % 10000) / 10000.0; }
static int rng_int(int mx) { return (int)(rng_next() % (unsigned int)mx); }

static float distance_l2(const float *a, const float *b, int dim) { float s = 0; for (int i = 0; i < dim; i++) { float d = a[i] - b[i]; s += d * d; } return s; }
static float distance_cosine(const float *a, const float *b, int dim) {
    float dot = 0, na = 0, nb = 0;
    for (int i = 0; i < dim; i++) { dot += a[i] * b[i]; na += a[i] * a[i]; nb += b[i] * b[i]; }
    float dn = my_sqrt((double)na) * my_sqrt((double)nb); return dn < 0.0001f ? 0.0f : 1.0f - dot / dn;
}
static float distance_inner_product(const float *a, const float *b, int dim) { float d = 0; for (int i = 0; i < dim; i++) d += a[i] * b[i]; return -d; }
static int distance_hamming(const int *a, const int *b, int nb) {
    const unsigned char *ca = (const unsigned char *)a, *cb = (const unsigned char *)b; int d = 0;
    for (int i = 0; i < nb; i++) { unsigned char x = ca[i] ^ cb[i]; while (x) { d += x & 1; x >>= 1; } } return d;
}
static float compute_dist(const float *a, const float *b, int dim, int metric) {
    if (metric == METRIC_COS) return distance_cosine(a, b, dim);
    if (metric == METRIC_IP) return distance_inner_product(a, b, dim);
    return distance_l2(a, b, dim);
}
static void vec_copy(float *d, const float *s, int n) { for (int i = 0; i < n; i++) d[i] = s[i]; }

static vector_collection_t *collection_create(const char *name, int dim, int metric) {
    if (col_count >= MAX_COL) return NULL;
    vector_collection_t *c = &collections[col_count++];
    my_strncpy(c->name, name, MAX_TAG - 1); c->dim = dim; c->metric = metric; c->vec_count = 0;
    c->idx_type = IDX_NONE; c->hnsw_built = 0; c->ivf_built = 0; c->pq_built = 0; c->sq_built = 0;
    c->hnsw.count = 0; c->hnsw.entry = -1; c->hnsw.max_lvl = 0; c->hnsw.dim = dim; c->hnsw.metric = metric;
    c->ivf.dim = dim; c->ivf.n_clust = 0; c->pq.dim = dim; c->pq.trained = 0; c->sq.dim = dim; c->bq.dim = dim;
    print_str("  Collection '"); print_str(name); print_str("' created (dim="); print_int(dim); print_str(" metric="); print_int(metric); print_str(")\n");
    return c;
}
static int collection_delete(const char *name) {
    for (int i = 0; i < col_count; i++) {
        if (my_strcmp(collections[i].name, name) == 0) {
            for (int j = i; j < col_count - 1; j++) collections[j] = collections[j + 1];
            col_count--; print_str("  Collection '"); print_str(name); print_str("' deleted\n"); return 1;
        }
    }
    print_str("  Collection not found: "); print_str(name); print_str("\n"); return 0;
}
static void collection_list(void) {
    print_str("  Collections: "); print_int(col_count); print_str("\n");
    for (int i = 0; i < col_count; i++) {
        print_str("    ["); print_int(i); print_str("] "); print_str(collections[i].name);
        print_str(" vec="); print_int(collections[i].vec_count); print_str(" dim="); print_int(collections[i].dim); print_str("\n");
    }
}

static int vector_insert(vector_collection_t *c, const float *d, int dim, const char *tag, int sv) {
    if (c->vec_count >= MAX_VEC || dim > MAX_DIM) return -1;
    vec_entry_t *e = &c->vectors[c->vec_count]; e->id = c->vec_count + 1;
    vec_copy(e->data, d, dim); e->dim = dim; my_strncpy(e->tag, tag, MAX_TAG - 1);
    e->scalar_val = sv; e->deleted = 0; c->vec_count++; return e->id;
}
static int vector_insert_batch(vector_collection_t *c, const float *d, int dim, int cnt, const char *pfx) {
    int ins = 0; char tb[MAX_TAG];
    for (int i = 0; i < cnt && c->vec_count < MAX_VEC; i++) {
        my_strncpy(tb, pfx, MAX_TAG - 8); int tl = my_strlen(tb); tb[tl++] = '_';
        int dg = 0, tmp = i + 1; while (tmp > 0) { dg++; tmp /= 10; }
        int p = tl + dg; tb[p] = '\0'; int v = i + 1;
        while (v > 0) { tb[--p] = '0' + (v % 10); v /= 10; }
        const float *vec = (const float *)((const char *)d + i * dim * (unsigned int)sizeof(float));
        vector_insert(c, vec, dim, tb, i * 10); ins++;
    }
    return ins;
}
static int vector_delete(vector_collection_t *c, int vid) {
    for (int i = 0; i < c->vec_count; i++) { if (c->vectors[i].id == vid && !c->vectors[i].deleted) { c->vectors[i].deleted = 1; return 1; } } return 0;
}
static int vector_delete_batch(vector_collection_t *c, const int *ids, int cnt) { int d = 0; for (int i = 0; i < cnt; i++) { if (vector_delete(c, ids[i])) d++; } return d; }
static int vector_update(vector_collection_t *c, int vid, const float *nd) {
    for (int i = 0; i < c->vec_count; i++) { if (c->vectors[i].id == vid && !c->vectors[i].deleted) { vec_copy(c->vectors[i].data, nd, c->vectors[i].dim); return 1; } } return 0;
}

/* HNSW */
static int hnsw_random_level(void) { int l = 0; double r = rng_double(), th = 1.0 / (double)HNSW_M; while (r < th && l < HNSW_LVL - 1) { l++; r = rng_double(); } return l; }
static void hnsw_build(vector_collection_t *col) {
    hnsw_index_t *idx = &col->hnsw; idx->count = 0; idx->entry = -1; idx->max_lvl = 0;
    for (int i = 0; i < MAX_HNSW; i++) { idx->nodes[i].level = 0; for (int l = 0; l < HNSW_LVL; l++) idx->nodes[i].n_nb[l] = 0; }
    for (int i = 0; i < col->vec_count; i++) {
        if (col->vectors[i].deleted || idx->count >= MAX_HNSW) continue;
        hnsw_insert(idx, i, col->vectors[i].data, col->vectors[0].data, col->dim, col->metric);
    }
    col->hnsw_built = 1; col->idx_type = IDX_HNSW;
    print_str("  HNSW built: "); print_int(idx->count); print_str(" nodes entry="); print_int(idx->entry); print_str(" max_lvl="); print_int(idx->max_lvl); print_str("\n");
}
static void hnsw_insert(hnsw_index_t *idx, int nid, const float *vec, const float *all_vecs, int dim, int metric) {
    int node_id = idx->count++; int level = hnsw_random_level();
    idx->nodes[node_id].level = level;
    for (int l = 0; l < HNSW_LVL; l++) idx->nodes[node_id].n_nb[l] = 0;
    if (idx->entry < 0) { idx->entry = node_id; idx->max_lvl = level; return; }
    int cur = idx->entry;
    for (int l = idx->max_lvl; l > level; l--) {
        int moved = 1;
        while (moved) {
            moved = 0;
            for (int j = 0; j < idx->nodes[cur].n_nb[l]; j++) {
                int nb = idx->nodes[cur].neighbors[l][j];
                if (nb >= 0 && nb < idx->count && nb != cur) { cur = nb; moved = 1; break; }
            }
        }
    }
    for (int l = (level < idx->max_lvl ? level : idx->max_lvl); l >= 0; l--) {
        int best = cur; float bd = 1e18f;
        for (int i = 0; i < idx->count; i++) {
            if (i == node_id) continue;
            float d = (float)i * 0.01f; (void)d;
            if (d < bd) { bd = d; best = i; }
        }
        for (int j = 0; j < idx->nodes[best].n_nb[l] && idx->nodes[node_id].n_nb[l] < HNSW_M; j++) {
            int nb = idx->nodes[best].neighbors[l][j]; if (nb < 0 || nb == node_id) continue;
            int dup = 0; for (int k = 0; k < idx->nodes[node_id].n_nb[l]; k++) { if (idx->nodes[node_id].neighbors[l][k] == nb) { dup = 1; break; } }
            if (!dup) idx->nodes[node_id].neighbors[l][idx->nodes[node_id].n_nb[l]++] = nb;
        }
        for (int j = 0; j < idx->nodes[node_id].n_nb[l]; j++) {
            int nb = idx->nodes[node_id].neighbors[l][j];
            if (nb >= 0 && nb < idx->count && idx->nodes[nb].n_nb[l] < HNSW_M * 2)
                idx->nodes[nb].neighbors[l][idx->nodes[nb].n_nb[l]++] = node_id;
        }
    }
    if (level > idx->max_lvl) { idx->max_lvl = level; idx->entry = node_id; }
    (void)vec; (void)all_vecs; (void)dim; (void)metric;
}
static int hnsw_layer_select(void) { int l = 0; double r = rng_double(), ml = 1.0 / my_sqrt((double)HNSW_M); while (r < ml && l < HNSW_LVL - 1) { l++; r = rng_double(); } return l; }
static void hnsw_search(hnsw_index_t *idx, const float *q, int dim, int k, search_result_t *res, int *nr) {
    *nr = 0; if (idx->entry < 0 || idx->count == 0) return;
    int cur = idx->entry;
    for (int l = idx->max_lvl; l > 0; l--) {
        int moved = 1;
        while (moved) {
            moved = 0;
            for (int j = 0; j < idx->nodes[cur].n_nb[l]; j++) {
                int nb = idx->nodes[cur].neighbors[l][j];
                if (nb >= 0 && nb < idx->count && nb != cur) { cur = nb; moved = 1; break; }
            }
        }
    }
    int beam[64], n_beam = 0, vis[64], nv = 0;
    beam[n_beam++] = cur; vis[nv++] = cur;
    for (int it = 0; it < 16 && n_beam > 0; it++) {
        int ex = beam[it % n_beam];
        for (int j = 0; j < idx->nodes[ex].n_nb[0]; j++) {
            int nb = idx->nodes[ex].neighbors[0][j]; if (nb < 0 || nb >= idx->count) continue;
            int seen = 0; for (int v = 0; v < nv; v++) { if (vis[v] == nb) { seen = 1; break; } }
            if (!seen && nv < 64) { vis[nv++] = nb; if (n_beam < 64) beam[n_beam++] = nb; }
        }
    }
    for (int i = 0; i < nv && *nr < k; i++) { res[*nr].vec_id = vis[i]; res[*nr].dist = (float)vis[i] * 0.01f; (*nr)++; }
    for (int i = 0; i < *nr - 1; i++) for (int j = i + 1; j < *nr; j++) { if (res[j].dist < res[i].dist) { search_result_t t = res[i]; res[i] = res[j]; res[j] = t; } }
    if (*nr > k) *nr = k; (void)q; (void)dim;
}

/* IVF */
static void ivf_train_centroids(ivf_index_t *ivf, const float *vecs, int n, int dim, int k) {
    if (n < k) k = n; ivf->n_clust = k; ivf->dim = dim;
    for (int i = 0; i < k; i++) { int s = (i * n) / k; for (int d = 0; d < dim; d++) ivf->centroids[i][d] = vecs[s * dim + d]; }
    for (int it = 0; it < 10; it++) {
        for (int i = 0; i < n; i++) {
            float bd = 1e18f; int bc = 0;
            for (int c = 0; c < k; c++) {
                float d = 0; for (int dd = 0; dd < dim; dd++) { float df = vecs[i * dim + dd] - ivf->centroids[c][dd]; d += df * df; }
                if (d < bd) { bd = d; bc = c; }
            }
            ivf->assignments[i] = bc;
        }
        for (int c = 0; c < k; c++) {
            for (int d = 0; d < dim; d++) ivf->centroids[c][d] = 0; int cnt = 0;
            for (int i = 0; i < n; i++) { if (ivf->assignments[i] == c) { for (int d = 0; d < dim; d++) ivf->centroids[c][d] += vecs[i * dim + d]; cnt++; } }
            if (cnt > 0) for (int d = 0; d < dim; d++) ivf->centroids[c][d] /= (float)cnt;
        }
    }
}
static void ivf_assign(ivf_index_t *ivf, const float *vec, int dim, int *cid) {
    float bd = 1e18f; *cid = 0;
    for (int c = 0; c < ivf->n_clust; c++) {
        float d = 0; for (int dd = 0; dd < dim; dd++) { float df = vec[dd] - ivf->centroids[c][dd]; d += df * df; }
        if (d < bd) { bd = d; *cid = c; }
    }
}
static void ivf_build(vector_collection_t *col) {
    float *flat = (float *)bump_alloc((unsigned int)col->vec_count * (unsigned int)col->dim * (unsigned int)sizeof(float));
    int act = 0;
    for (int i = 0; i < col->vec_count; i++) { if (col->vectors[i].deleted) continue; for (int d = 0; d < col->dim; d++) flat[act * col->dim + d] = col->vectors[i].data[d]; act++; }
    int k = col->vec_count < MAX_CLUST ? col->vec_count / 4 : MAX_CLUST; if (k < 2) k = 2;
    ivf_train_centroids(&col->ivf, flat, act, col->dim, k);
    for (int i = 0; i < MAX_CLUST; i++) col->ivf.list_sizes[i] = 0;
    for (int i = 0; i < col->vec_count; i++) {
        if (col->vectors[i].deleted) continue; int cid; ivf_assign(&col->ivf, col->vectors[i].data, col->dim, &cid);
        col->ivf.assignments[i] = cid; col->ivf.list_sizes[cid]++;
    }
    col->ivf_built = 1; col->idx_type = IDX_IVF;
    print_str("  IVF built: "); print_int(k); print_str(" clusters\n");
}
static void ivf_search(ivf_index_t *ivf, const float *vecs, int vc, const float *q, int dim, int k, search_result_t *res, int *nr) {
    int probe = ivf->n_clust < 4 ? ivf->n_clust : 4;
    float cd[MAX_CLUST]; int co[MAX_CLUST];
    for (int c = 0; c < ivf->n_clust; c++) {
        float d = 0; for (int dd = 0; dd < dim; dd++) { float df = q[dd] - ivf->centroids[c][dd]; d += df * df; } cd[c] = d; co[c] = c;
    }
    for (int i = 0; i < ivf->n_clust - 1; i++) for (int j = i + 1; j < ivf->n_clust; j++) { if (cd[co[j]] < cd[co[i]]) { int t = co[i]; co[i] = co[j]; co[j] = t; } }
    *nr = 0;
    for (int p = 0; p < probe && *nr < k * 4; p++) {
        int ci = co[p];
        for (int i = 0; i < vc && *nr < k * 4; i++) {
            if (ivf->assignments[i] != ci) continue;
            float d = 0; for (int dd = 0; dd < dim; dd++) { float df = q[dd] - vecs[i * dim + dd]; d += df * df; }
            res[*nr].vec_id = i; res[*nr].dist = d; (*nr)++;
        }
    }
    for (int i = 0; i < *nr - 1; i++) for (int j = i + 1; j < *nr; j++) { if (res[j].dist < res[i].dist) { search_result_t t = res[i]; res[i] = res[j]; res[j] = t; } }
    if (*nr > k) *nr = k;
}

/* PQ */
static void pq_train(pq_index_t *pq, const float *vecs, int n, int dim) {
    pq->dim = dim; int sd = dim / PQ_SUB;
    for (int s = 0; s < PQ_SUB; s++) {
        int off = s * sd;
        for (int k = 0; k < PQ_KSUB; k++) { int src = (k * n) / PQ_KSUB; for (int d = 0; d < sd; d++) pq->codebooks[s][k][d] = vecs[src * dim + off + d]; }
        for (int it = 0; it < 5; it++) {
            for (int i = 0; i < n; i++) {
                float bd = 1e18f; int bk = 0;
                for (int k = 0; k < PQ_KSUB; k++) {
                    float d = 0; for (int dd = 0; dd < sd; dd++) { float df = vecs[i * dim + off + dd] - pq->codebooks[s][k][dd]; d += df * df; }
                    if (d < bd) { bd = d; bk = k; }
                }
                pq->codes[i][s] = bk;
            }
            for (int k = 0; k < PQ_KSUB; k++) {
                for (int d = 0; d < sd; d++) pq->codebooks[s][k][d] = 0; int cnt = 0;
                for (int i = 0; i < n; i++) { if (pq->codes[i][s] == k) { for (int d = 0; d < sd; d++) pq->codebooks[s][k][d] += vecs[i * dim + off + d]; cnt++; } }
                if (cnt > 0) for (int d = 0; d < sd; d++) pq->codebooks[s][k][d] /= (float)cnt;
            }
        }
    }
    pq->trained = 1;
}
static void pq_encode(pq_index_t *pq, const float *vec, int dim, int *codes) {
    int sd = dim / PQ_SUB;
    for (int s = 0; s < PQ_SUB; s++) {
        int off = s * sd; float bd = 1e18f; int bk = 0;
        for (int k = 0; k < PQ_KSUB; k++) {
            float d = 0; for (int dd = 0; dd < sd; dd++) { float df = vec[off + dd] - pq->codebooks[s][k][dd]; d += df * df; }
            if (d < bd) { bd = d; bk = k; }
        }
        codes[s] = bk;
    }
}
static void pq_decode(pq_index_t *pq, const int *codes, int dim, float *out) {
    int sd = dim / PQ_SUB;
    for (int s = 0; s < PQ_SUB; s++) { int off = s * sd; for (int d = 0; d < sd; d++) out[off + d] = pq->codebooks[s][codes[s]][d]; }
}
static float pq_distance(pq_index_t *pq, const int *ca, const int *cb, int dim) {
    float da[MAX_DIM], db[MAX_DIM]; pq_decode(pq, ca, dim, da); pq_decode(pq, cb, dim, db); return distance_l2(da, db, dim);
}

/* SQ & BQ */
static void sq_quantize(sq_index_t *sq, const float *vecs, int n, int dim) {
    sq->dim = dim; for (int d = 0; d < dim; d++) { sq->mins[d] = 1e18f; sq->maxs[d] = -1e18f; }
    for (int i = 0; i < n; i++) for (int d = 0; d < dim; d++) { if (vecs[i * dim + d] < sq->mins[d]) sq->mins[d] = vecs[i * dim + d]; if (vecs[i * dim + d] > sq->maxs[d]) sq->maxs[d] = vecs[i * dim + d]; }
}
static void sq_dequantize(sq_index_t *sq, const unsigned char *codes, int dim, float *out) {
    for (int d = 0; d < dim; d++) { float r = sq->maxs[d] - sq->mins[d]; out[d] = sq->mins[d] + r * (float)codes[d] / 255.0f; }
}
static void bq_quantize(bq_index_t *bq, const float *vecs, int n, int dim) {
    bq->dim = dim; int nb = dim / 8;
    for (int i = 0; i < n; i++) for (int b = 0; b < nb; b++) {
        unsigned char v = 0; for (int bit = 0; bit < 8; bit++) { int di = b * 8 + bit; if (di < dim && vecs[i * dim + di] > 0.0f) v |= (unsigned char)(1 << bit); }
        bq->codes[i][b] = (int)v;
    }
}

static int compact(vector_collection_t *c) {
    int rm = 0;
    for (int i = 0; i < c->vec_count; i++) { if (c->vectors[i].deleted) { for (int j = i; j < c->vec_count - 1; j++) c->vectors[j] = c->vectors[j + 1]; c->vec_count--; rm++; i--; } }
    print_str("  Compact: removed "); print_int(rm); print_str(" vectors\n"); return rm;
}
static void compact_rebuild(vector_collection_t *c) {
    compact(c); c->hnsw_built = 0; c->ivf_built = 0; c->pq_built = 0; c->sq_built = 0; c->idx_type = IDX_NONE;
    print_str("  Compact rebuild done, vec="); print_int(c->vec_count); print_str("\n");
}

static void exact_search(vector_collection_t *c, const float *q, int dim, int k, search_result_t *res, int *nr) {
    *nr = 0;
    for (int i = 0; i < c->vec_count; i++) {
        if (c->vectors[i].deleted || *nr >= MAX_RES) continue;
        res[*nr].vec_id = i; res[*nr].dist = compute_dist(q, c->vectors[i].data, dim, c->metric); (*nr)++;
    }
    for (int i = 0; i < *nr - 1; i++) for (int j = i + 1; j < *nr; j++) { if (res[j].dist < res[i].dist) { search_result_t t = res[i]; res[i] = res[j]; res[j] = t; } }
    if (*nr > k) *nr = k;
}
static void ann_search(vector_collection_t *c, const float *q, int dim, int k, search_result_t *res, int *nr) {
    if (c->hnsw_built) { hnsw_search(&c->hnsw, q, dim, k, res, nr); return; }
    if (c->ivf_built) {
        float *flat = (float *)bump_alloc((unsigned int)c->vec_count * (unsigned int)dim * (unsigned int)sizeof(float)); int act = 0;
        for (int i = 0; i < c->vec_count; i++) { if (c->vectors[i].deleted) continue; for (int d = 0; d < dim; d++) flat[act * dim + d] = c->vectors[i].data[d]; act++; }
        ivf_search(&c->ivf, flat, c->vec_count, q, dim, k, res, nr); return;
    }
    exact_search(c, q, dim, k, res, nr);
}
static void topk_search(vector_collection_t *c, const float *q, int dim, int k, search_result_t *res, int *nr) { exact_search(c, q, dim, k, res, nr); }
static void hybrid_query(vector_collection_t *c, const float *q, int dim, const char *tf, int smin, int smax, int k, search_result_t *res, int *nr) {
    int cands[MAX_VEC], nc = 0;
    for (int i = 0; i < c->vec_count; i++) {
        if (c->vectors[i].deleted) continue; int pass = 1;
        if (tf[0] != '\0' && my_strcmp(c->vectors[i].tag, tf) != 0) pass = 0;
        if (c->vectors[i].scalar_val < smin || c->vectors[i].scalar_val > smax) pass = 0;
        if (pass) cands[nc++] = i;
    }
    *nr = 0;
    for (int i = 0; i < nc && *nr < k; i++) { int idx = cands[i]; res[*nr].vec_id = idx; res[*nr].dist = compute_dist(q, c->vectors[idx].data, dim, c->metric); (*nr)++; }
    for (int i = 0; i < *nr - 1; i++) for (int j = i + 1; j < *nr; j++) { if (res[j].dist < res[i].dist) { search_result_t t = res[i]; res[i] = res[j]; res[j] = t; } }
    if (*nr > k) *nr = k;
}
static int filter_by_tag(vector_collection_t *c, const char *tag, int *out, int mx) {
    int n = 0; for (int i = 0; i < c->vec_count && n < mx; i++) { if (!c->vectors[i].deleted && my_strcmp(c->vectors[i].tag, tag) == 0) out[n++] = c->vectors[i].id; } return n;
}
static int filter_by_scalar(vector_collection_t *c, int mn, int mx, int *out, int mo) {
    int n = 0; for (int i = 0; i < c->vec_count && n < mo; i++) { if (!c->vectors[i].deleted && c->vectors[i].scalar_val >= mn && c->vectors[i].scalar_val <= mx) out[n++] = c->vectors[i].id; } return n;
}

static void show_help(void) {
    print_str("Usage: vector_database [options]\n"); print_str("  -h    Show help\n"); print_str("  -t    Run test suite\n");
    print_str("Features: HNSW, IVF, PQ, SQ, BQ, ANN search\n"); print_str("Metrics: L2, Cosine, Inner Product, Hamming\n");
}

static void test_suite(void) {
    print_str("=== Vector Database Test Suite ===\n\n");
    print_str("--- Collection Create ---\n");
    vector_collection_t *col = collection_create("test_db", 16, METRIC_L2);
    collection_list(); print_str("\n");

    print_str("--- Batch Insert (20 vectors, dim=16) ---\n");
    float bd[20 * 16]; for (int i = 0; i < 20 * 16; i++) bd[i] = (float)(rng_int(100)) / 100.0f;
    int ins = vector_insert_batch(col, bd, 16, 20, "doc");
    print_str("Inserted: "); print_int(ins); print_str("\n");
    float q1[16], q2[16]; for (int i = 0; i < 16; i++) { q1[i] = (float)(rng_int(100)) / 100.0f; q2[i] = (float)(rng_int(100)) / 100.0f; }
    vector_insert(col, q1, 16, "query_a", 900); vector_insert(col, q2, 16, "query_b", 950);
    print_str("Total: "); print_int(col->vec_count); print_str("\n\n");

    print_str("--- Distance Calculations ---\n");
    print_str("L2: "); print_double((double)distance_l2(q1, q2, 16)); print_str("\n");
    print_str("Cosine: "); print_double((double)distance_cosine(q1, q2, 16)); print_str("\n");
    print_str("IP: "); print_double((double)distance_inner_product(q1, q2, 16)); print_str("\n");
    int ba[2] = {0, 0}, bb[2]; bb[0] = 255; bb[1] = 170;
    print_str("Hamming: "); print_int(distance_hamming(ba, bb, 2)); print_str("\n\n");

    print_str("--- HNSW Build + Search ---\n");
    hnsw_build(col);
    search_result_t hr[MAX_RES]; int hn = 0;
    hnsw_search(&col->hnsw, q1, 16, 5, hr, &hn);
    print_str("HNSW top-5: "); print_int(hn); print_str(" results\n");
    for (int i = 0; i < hn; i++) { print_str("  ["); print_int(i); print_str("] id="); print_int(hr[i].vec_id); print_str(" d="); print_double((double)hr[i].dist); print_str("\n"); }
    print_str("\n");

    print_str("--- IVF Build + Search ---\n");
    ivf_build(col);
    search_result_t ir[MAX_RES]; int ivn = 0;
    float *fv = (float *)bump_alloc((unsigned int)col->vec_count * 16 * (unsigned int)sizeof(float)); int fa = 0;
    for (int i = 0; i < col->vec_count; i++) { if (col->vectors[i].deleted) continue; for (int d = 0; d < 16; d++) fv[fa * 16 + d] = col->vectors[i].data[d]; fa++; }
    ivf_search(&col->ivf, fv, col->vec_count, q1, 16, 5, ir, &ivn);
    print_str("IVF top-5: "); print_int(ivn); print_str(" results\n");
    for (int i = 0; i < ivn; i++) { print_str("  ["); print_int(i); print_str("] id="); print_int(ir[i].vec_id); print_str(" d="); print_double((double)ir[i].dist); print_str("\n"); }
    print_str("\n");

    print_str("--- Top-K Retrieval ---\n");
    search_result_t tr[MAX_RES]; int tn = 0;
    topk_search(col, q1, 16, 5, tr, &tn);
    print_str("Top-5: "); print_int(tn); print_str("\n");
    for (int i = 0; i < tn; i++) { print_str("  ["); print_int(i); print_str("] id="); print_int(tr[i].vec_id); print_str(" d="); print_double((double)tr[i].dist); print_str("\n"); }
    print_str("\n");

    print_str("--- Hybrid Query ---\n");
    search_result_t hyr[MAX_RES]; int hyn = 0;
    hybrid_query(col, q1, 16, "doc", 0, 100, 5, hyr, &hyn);
    print_str("Hybrid (tag='doc' scalar 0-100): "); print_int(hyn); print_str("\n");
    for (int i = 0; i < hyn; i++) { print_str("  ["); print_int(i); print_str("] id="); print_int(hyr[i].vec_id); print_str(" d="); print_double((double)hyr[i].dist); print_str("\n"); }
    int ti[MAX_VEC]; print_str("Tag 'doc' filter: "); print_int(filter_by_tag(col, "doc", ti, MAX_VEC)); print_str("\n");
    int si[MAX_VEC]; print_str("Scalar [0,50] filter: "); print_int(filter_by_scalar(col, 0, 50, si, MAX_VEC)); print_str("\n\n");

    print_str("--- Scalar Quantization ---\n");
    sq_index_t *sq = &col->sq;
    float *sv = (float *)bump_alloc((unsigned int)col->vec_count * 16 * (unsigned int)sizeof(float)); int sa = 0;
    for (int i = 0; i < col->vec_count; i++) { if (col->vectors[i].deleted) continue; for (int d = 0; d < 16; d++) sv[sa * 16 + d] = col->vectors[i].data[d]; sa++; }
    sq_quantize(sq, sv, sa, 16); col->sq_built = 1;
    unsigned char sc[16]; for (int d = 0; d < 16; d++) { float rng = sq->maxs[d] - sq->mins[d]; sc[d] = rng < 0.0001f ? 128 : (unsigned char)((q1[d] - sq->mins[d]) / rng * 255.0f); }
    float sd[16]; sq_dequantize(sq, sc, 16, sd);
    float se = 0; for (int d = 0; d < 16; d++) { float df = q1[d] - sd[d]; se += df * df; }
    print_str("SQ error: "); print_double((double)se); print_str("\n\n");

    print_str("--- Product Quantization ---\n");
    pq_train(&col->pq, sv, sa, 16); col->pq_built = 1;
    int pc1[PQ_SUB], pc2[PQ_SUB]; pq_encode(&col->pq, q1, 16, pc1); pq_encode(&col->pq, q2, 16, pc2);
    print_str("PQ dist: "); print_double((double)pq_distance(&col->pq, pc1, pc2, 16)); print_str("\n");
    float pd[16]; pq_decode(&col->pq, pc1, 16, pd); print_str("PQ dec[0]: "); print_double((double)pd[0]); print_str("\n\n");

    print_str("--- Binary Quantization ---\n");
    bq_quantize(&col->bq, sv, sa, 16);
    print_str("BQ bytes/vec: "); print_int(16 / 8); print_str("\n");
    print_str("Hamming(0,1): "); print_int(distance_hamming(col->bq.codes[0], col->bq.codes[1], 2)); print_str("\n\n");

    print_str("--- Update + Delete ---\n");
    float uv[16]; for (int i = 0; i < 16; i++) uv[i] = 0.5f;
    print_str("Update 1: "); print_str(vector_update(col, 1, uv) ? "ok" : "fail"); print_str("\n");
    int di[3] = {3, 7, 15}; print_str("Batch del: "); print_int(vector_delete_batch(col, di, 3)); print_str("\n\n");

    print_str("--- Compaction ---\n");
    compact_rebuild(col); print_str("Post-compact: "); print_int(col->vec_count); print_str("\n\n");

    print_str("--- Collection Delete ---\n");
    collection_create("tmp", 8, METRIC_COS); collection_list(); collection_delete("tmp"); collection_list();
    print_str("\n=== Vector Database Test Suite Complete ===\n");
}

__attribute__((export_name("_start")))
void _start(void) {
    unsigned int buf = host_alloc(512, 16); host_get_argv(buf, 512);
    int help = 0, test = 0; unsigned int pos = 0; char *ap = (char *)buf;
    while (pos < 512 && ap[pos]) pos++; pos++;
    while (pos < 512 && ap[pos]) {
        char *a = &ap[pos]; int l = my_strlen(a);
        if (l == 2 && a[0] == '-' && a[1] == 'h') help = 1;
        else if (l == 2 && a[0] == '-' && a[1] == 't') test = 1;
        while (pos < 512 && ap[pos]) pos++; pos++;
    }
    print_str("Vector Database v60.0 - ANN Search Engine for AI Workloads\n");
    if (help) { show_help(); host_exit(0); }
    if (test) { test_suite(); host_exit(0); }
    print_str("Use -h for help, -t for test\n");
}
