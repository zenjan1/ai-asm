/* distributed_fs: Distributed filesystem module (v1.0)
 *
 * Provides distributed filesystem support including:
 * - Data sharding (hash-based and range-based)
 * - Replica synchronization (synchronous and asynchronous)
 * - Consistent hashing with virtual nodes
 * - Metadata management (file catalog, inode tracking)
 * - Failure recovery (node failure detection, replica promotion)
 */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_NODES         32
#define MAX_VNODES        128
#define MAX_SHARDS        64
#define MAX_REPLICAS       4
#define MAX_FILES         64
#define MAX_PATH          64
#define MAX_NAME          32
#define MAX_METADATA      128
#define REPL_FACTOR        3

/* Node states */
#define NODE_ACTIVE     0
#define NODE_DEGRADED   1
#define NODE_FAILED     2
#define NODE_RECOVERING 3

/* Shard strategies */
#define SHARD_HASH      0
#define SHARD_RANGE     1

/* Sync modes */
#define SYNC_SYNC       0
#define SYNC_ASYNC      1

/* Replica states */
#define REPLICA_PRIMARY   0
#define REPLICA_SECONDARY 1
#define REPLICA_STALE     2
#define REPLICA_SYNCING   3

/* File types */
#define FILE_REGULAR    0
#define FILE_DIRECTORY  1
#define FILE_SYMLINK    2

/* ===== Data Structures ===== */

typedef struct {
    int     node_id;
    char    name[MAX_NAME];
    char    address[MAX_NAME];
    int     state;
    int     vnodes;
    int     shard_count;
    int     replica_count;
    int     capacity_gb;
    int     used_gb;
    int     last_heartbeat;
} node_t;

typedef struct {
    int     vnode_id;
    int     node_id;
    unsigned int hash_ring_pos;
} vnode_t;

typedef struct {
    int     shard_id;
    int     strategy;
    unsigned int range_start;
    unsigned int range_end;
    int     primary_node;
    int     replica_nodes[MAX_REPLICAS];
    int     replica_count;
    int     object_count;
    unsigned long size_bytes;
} shard_t;

typedef struct {
    int     replica_id;
    int     shard_id;
    int     node_id;
    int     role;
    int     state;
    int     version;
    int     sync_seq;
    int     last_sync_time;
} replica_t;

typedef struct {
    int     inode;
    char    path[MAX_PATH];
    int     file_type;
    int     shard_id;
    unsigned long size_bytes;
    int     repl_count;
    int     version;
    int     created_at;
    int     modified_at;
    int     owner_id;
} file_entry_t;

typedef struct {
    int     key;
    int     value;
    int     owner_inode;
} metadata_t;

/* Global state */
static node_t     nodes[MAX_NODES];
static vnode_t    vnodes[MAX_VNODES];
static shard_t    shards[MAX_SHARDS];
static replica_t  replicas[MAX_SHARDS * MAX_REPLICAS];
static file_entry_t files[MAX_FILES];
static metadata_t metadata[MAX_METADATA];

static int node_count = 0;
static int vnode_count = 0;
static int shard_count = 0;
static int replica_count = 0;
static int file_count = 0;
static int meta_count = 0;
static int clock = 0;
static int next_inode = 1000;

/* ===== Utility Functions ===== */

static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) { buf[pos++] = '0'; buf[pos] = '\0'; host_print(buf); return; }
    { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
      while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}
static void print_uint(unsigned int val) {
    char buf[32]; int pos = 0;
    if (val == 0) { buf[pos++] = '0'; buf[pos] = '\0'; host_print(buf); return; }
    { int d = 0; unsigned int t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
      while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* Simple hash function (DJB2-like) */
static unsigned int hash_key(const char *key) {
    unsigned int hash = 5381;
    for (int i = 0; key[i]; i++) {
        hash = ((hash << 5) + hash) + (unsigned int)key[i];
    }
    return hash;
}

static unsigned int hash_uint(unsigned int val) {
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = ((val >> 16) ^ val) * 0x45d9f3b;
    val = (val >> 16) ^ val;
    return val;
}

/* ===== Node Management ===== */

int node_register(const char *name, const char *address, int capacity_gb) {
    if (node_count >= MAX_NODES) return -1;
    int idx = node_count;
    int nid = idx + 1;
    nodes[idx].node_id = nid;
    my_strncpy(nodes[idx].name, name, MAX_NAME - 1);
    my_strncpy(nodes[idx].address, address, MAX_NAME - 1);
    nodes[idx].state = NODE_ACTIVE;
    nodes[idx].vnodes = 0;
    nodes[idx].shard_count = 0;
    nodes[idx].replica_count = 0;
    nodes[idx].capacity_gb = capacity_gb;
    nodes[idx].used_gb = 0;
    nodes[idx].last_heartbeat = ++clock;
    node_count++;
    return nid;
}

static node_t *find_node(int node_id) {
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].node_id == node_id) return &nodes[i];
    }
    return 0;
}

int node_heartbeat(int node_id) {
    node_t *n = find_node(node_id);
    if (!n) return -1;
    n->last_heartbeat = ++clock;
    if (n->state == NODE_FAILED) n->state = NODE_RECOVERING;
    return 0;
}

int node_detect_failures(int timeout) {
    int failed = 0;
    for (int i = 0; i < node_count; i++) {
        if (nodes[i].state == NODE_ACTIVE) {
            if ((clock - nodes[i].last_heartbeat) > timeout) {
                nodes[i].state = NODE_FAILED;
                failed++;
            }
        }
    }
    return failed;
}

/* ===== Consistent Hashing ===== */

int vnode_add(int node_id, int vnode_index) {
    if (vnode_count >= MAX_VNODES) return -1;
    node_t *n = find_node(node_id);
    if (!n || n->state != NODE_ACTIVE) return -1;

    char key[MAX_NAME + 16];
    my_strncpy(key, n->name, MAX_NAME - 1);
    { int sl = my_strlen(key); key[sl] = '#';
      char nb[12]; int p = 0; int v = vnode_index;
      if (v == 0) { nb[p++] = '0'; } else { int d=0,t=v; while(t>0){d++;t/=10;} while(v>0){nb[--d]='0'+(v%10);v/=10;} p+=d; }
      nb[p] = '\0';
      int j = 0; while (nb[j] && sl + 1 + j < MAX_NAME + 15) { key[sl+1+j] = nb[j]; j++; }
      key[sl+1+j] = '\0';
    }

    int idx = vnode_count;
    vnodes[idx].vnode_id = vnode_count + 1;
    vnodes[idx].node_id = node_id;
    vnodes[idx].hash_ring_pos = hash_key(key);
    vnode_count++;
    n->vnodes++;
    return vnodes[idx].vnode_id;
}

int vnode_find_owner(unsigned int hash_pos) {
    if (vnode_count == 0) return -1;
    int best = -1;
    unsigned int best_dist = 0xFFFFFFFF;
    for (int i = 0; i < vnode_count; i++) {
        unsigned int dist = vnodes[i].hash_ring_pos - hash_pos;
        if (vnodes[i].hash_ring_pos >= hash_pos && dist < best_dist) {
            best_dist = dist;
            best = i;
        }
    }
    if (best < 0) {
        /* wrap around: pick smallest position */
        best_dist = 0xFFFFFFFF;
        for (int i = 0; i < vnode_count; i++) {
            unsigned int dist = vnodes[i].hash_ring_pos + (0xFFFFFFFF - hash_pos);
            if (dist < best_dist) { best_dist = dist; best = i; }
        }
    }
    return best >= 0 ? vnodes[best].node_id : -1;
}

/* ===== Data Sharding ===== */

int shard_create(int strategy, unsigned int range_start, unsigned int range_end) {
    if (shard_count >= MAX_SHARDS) return -1;
    int idx = shard_count;
    int sid = shard_count + 1;
    shards[idx].shard_id = sid;
    shards[idx].strategy = strategy;
    shards[idx].range_start = range_start;
    shards[idx].range_end = range_end;
    shards[idx].primary_node = -1;
    shards[idx].replica_count = 0;
    shards[idx].object_count = 0;
    shards[idx].size_bytes = 0;
    for (int r = 0; r < MAX_REPLICAS; r++) shards[idx].replica_nodes[r] = -1;
    shard_count++;
    return sid;
}

int shard_assign_node(int shard_id, int node_id) {
    for (int i = 0; i < shard_count; i++) {
        if (shards[i].shard_id == shard_id) {
            node_t *n = find_node(node_id);
            if (!n || n->state == NODE_FAILED) return -1;
            shards[i].primary_node = node_id;
            n->shard_count++;
            return 0;
        }
    }
    return -1;
}

int shard_route_key(const char *key) {
    unsigned int h = hash_key(key);
    /* Hash-based: use consistent hashing ring */
    int owner = vnode_find_owner(h);
    if (owner < 0) return -1;
    for (int i = 0; i < shard_count; i++) {
        if (shards[i].primary_node == owner) return shards[i].shard_id;
    }
    /* Fallback: hash modulo shard count */
    if (shard_count == 0) return -1;
    return (int)(h % (unsigned int)shard_count) + 1;
}

int shard_route_range(unsigned int key_val) {
    for (int i = 0; i < shard_count; i++) {
        if (shards[i].strategy == SHARD_RANGE &&
            key_val >= shards[i].range_start &&
            key_val < shards[i].range_end) {
            return shards[i].shard_id;
        }
    }
    return -1;
}

/* ===== Replica Management ===== */

int replica_create(int shard_id, int node_id, int role) {
    if (replica_count >= MAX_SHARDS * MAX_REPLICAS) return -1;
    shard_t *sh = 0;
    for (int i = 0; i < shard_count; i++) {
        if (shards[i].shard_id == shard_id) { sh = &shards[i]; break; }
    }
    if (!sh) return -1;

    int idx = replica_count;
    replicas[idx].replica_id = replica_count + 1;
    replicas[idx].shard_id = shard_id;
    replicas[idx].node_id = node_id;
    replicas[idx].role = role;
    replicas[idx].state = REPLICA_SYNCING;
    replicas[idx].version = 0;
    replicas[idx].sync_seq = 0;
    replicas[idx].last_sync_time = ++clock;
    replica_count++;

    if (sh->replica_count < MAX_REPLICAS) {
        sh->replica_nodes[sh->replica_count] = node_id;
        sh->replica_count++;
    }
    node_t *n = find_node(node_id);
    if (n) n->replica_count++;
    return replicas[idx].replica_id;
}

int replica_sync(int replica_id) {
    for (int i = 0; i < replica_count; i++) {
        if (replicas[i].replica_id == replica_id) {
            replicas[i].state = REPLICA_SECONDARY;
            replicas[i].sync_seq++;
            replicas[i].last_sync_time = ++clock;
            /* Find primary and match version */
            for (int j = 0; j < replica_count; j++) {
                if (replicas[j].shard_id == replicas[i].shard_id &&
                    replicas[j].role == REPLICA_PRIMARY) {
                    replicas[i].version = replicas[j].version;
                    break;
                }
            }
            return 0;
        }
    }
    return -1;
}

int replica_sync_shard(int shard_id, int mode) {
    int synced = 0;
    for (int i = 0; i < replica_count; i++) {
        if (replicas[i].shard_id == shard_id && replicas[i].state == REPLICA_STALE) {
            if (mode == SYNC_SYNC) {
                replica_sync(replicas[i].replica_id);
                synced++;
            } else {
                replicas[i].state = REPLICA_SYNCING;
                synced++;
            }
        }
    }
    return synced;
}

int replica_write(int shard_id, unsigned long size) {
    for (int i = 0; i < replica_count; i++) {
        if (replicas[i].shard_id == shard_id && replicas[i].role == REPLICA_PRIMARY) {
            replicas[i].version++;
            /* Mark secondaries stale */
            for (int j = 0; j < replica_count; j++) {
                if (replicas[j].shard_id == shard_id && replicas[j].role != REPLICA_PRIMARY) {
                    replicas[j].state = REPLICA_STALE;
                }
            }
            for (int j = 0; j < shard_count; j++) {
                if (shards[j].shard_id == shard_id) {
                    shards[j].object_count++;
                    shards[j].size_bytes += size;
                    break;
                }
            }
            return replicas[i].version;
        }
    }
    return -1;
}

/* ===== Failure Recovery ===== */

int recovery_detect_and_repair(void) {
    int repaired = 0;
    for (int s = 0; s < shard_count; s++) {
        node_t *pn = find_node(shards[s].primary_node);
        if (!pn || pn->state == NODE_FAILED) {
            /* Promote first healthy secondary */
            for (int r = 0; r < replica_count; r++) {
                if (replicas[r].shard_id == shards[s].shard_id &&
                    replicas[r].role == REPLICA_SECONDARY) {
                    node_t *rn = find_node(replicas[r].node_id);
                    if (rn && rn->state == NODE_ACTIVE) {
                        shards[s].primary_node = replicas[r].node_id;
                        replicas[r].role = REPLICA_PRIMARY;
                        replicas[r].state = REPLICA_SECONDARY;
                        repaired++;
                        break;
                    }
                }
            }
        }
        /* Ensure replication factor */
        int live_replicas = 0;
        for (int r = 0; r < replica_count; r++) {
            if (replicas[r].shard_id == shards[s].shard_id) {
                node_t *rn = find_node(replicas[r].node_id);
                if (rn && rn->state == NODE_ACTIVE) live_replicas++;
            }
        }
        if (live_replicas < REPL_FACTOR) {
            for (int n = 0; n < node_count && live_replicas < REPL_FACTOR; n++) {
                if (nodes[n].state != NODE_ACTIVE) continue;
                int already = 0;
                for (int r = 0; r < replica_count; r++) {
                    if (replicas[r].shard_id == shards[s].shard_id &&
                        replicas[r].node_id == nodes[n].node_id) { already = 1; break; }
                }
                if (!already) {
                    replica_create(shards[s].shard_id, nodes[n].node_id, REPLICA_SECONDARY);
                    live_replicas++;
                    repaired++;
                }
            }
        }
    }
    return repaired;
}

/* ===== Metadata Management ===== */

int file_create(const char *path, int file_type, int shard_id) {
    if (file_count >= MAX_FILES) return -1;
    int idx = file_count;
    int ino = next_inode++;
    files[idx].inode = ino;
    my_strncpy(files[idx].path, path, MAX_PATH - 1);
    files[idx].file_type = file_type;
    files[idx].shard_id = shard_id;
    files[idx].size_bytes = 0;
    files[idx].repl_count = 0;
    files[idx].version = 1;
    files[idx].created_at = ++clock;
    files[idx].modified_at = clock;
    files[idx].owner_id = 0;
    file_count++;
    return ino;
}

static file_entry_t *find_file_by_inode(int inode) {
    for (int i = 0; i < file_count; i++) {
        if (files[i].inode == inode) return &files[i];
    }
    return 0;
}

int file_lookup(const char *path) {
    for (int i = 0; i < file_count; i++) {
        if (my_strcmp(files[i].path, path) == 0) return files[i].inode;
    }
    return -1;
}

int meta_set(int inode, int key, int value) {
    /* Update existing */
    for (int i = 0; i < meta_count; i++) {
        if (metadata[i].owner_inode == inode && metadata[i].key == key) {
            metadata[i].value = value;
            return 0;
        }
    }
    if (meta_count >= MAX_METADATA) return -1;
    metadata[meta_count].key = key;
    metadata[meta_count].value = value;
    metadata[meta_count].owner_inode = inode;
    meta_count++;
    return 0;
}

int meta_get(int inode, int key) {
    for (int i = 0; i < meta_count; i++) {
        if (metadata[i].owner_inode == inode && metadata[i].key == key) {
            return metadata[i].value;
        }
    }
    return -1;
}

/* ===== Status Display ===== */

void cluster_status(void) {
    print_str("  Cluster Status\n");
    print_str("  ==================================================================\n");
    int active = 0, degraded = 0, failed = 0;
    for (int i = 0; i < node_count; i++) {
        const char *st = "UNKNOWN";
        if (nodes[i].state == NODE_ACTIVE) { st = "ACTIVE"; active++; }
        else if (nodes[i].state == NODE_DEGRADED) { st = "DEGRADED"; degraded++; }
        else if (nodes[i].state == NODE_FAILED) { st = "FAILED"; failed++; }
        else if (nodes[i].state == NODE_RECOVERING) { st = "RECOVERING"; }

        print_str("  Node "); print_int(nodes[i].node_id);
        print_str(" ["); print_str(nodes[i].name); print_str("] ");
        print_str(st);
        print_str(" shards="); print_int(nodes[i].shard_count);
        print_str(" replicas="); print_int(nodes[i].replica_count);
        print_str(" used="); print_int(nodes[i].used_gb);
        print_str("/"); print_int(nodes[i].capacity_gb);
        print_str("GB\n");
    }
    print_str("  Summary: "); print_int(active); print_str(" active, ");
    print_int(degraded); print_str(" degraded, ");
    print_int(failed); print_str(" failed\n");
    print_str("  Shards: "); print_int(shard_count);
    print_str("  Replicas: "); print_int(replica_count);
    print_str("  Files: "); print_int(file_count);
    print_str("\n");
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("DistributedFS v1.0 - Distributed Filesystem Module\n");
    if (help) {
        print_str("Usage: distributed_fs [options]\n");
        print_str("  -h, --help    Show help\n");
        print_str("  -t, --test    Run distributed filesystem tests\n");
        return;
    }

    if (test) {
        print_str("=== DistributedFS Test Suite ===\n\n");

        /* Test 1: Node registration */
        print_str("[Test 1] Node Registration\n");
        int n1 = node_register("node-alpha", "10.0.1.1", 500);
        int n2 = node_register("node-beta", "10.0.1.2", 500);
        int n3 = node_register("node-gamma", "10.0.1.3", 1000);
        int n4 = node_register("node-delta", "10.0.1.4", 500);
        print_str("  Registered nodes: ");
        print_int(n1); print_str(", "); print_int(n2); print_str(", ");
        print_int(n3); print_str(", "); print_int(n4); print_str("\n");
        cluster_status();
        print_str("\n");

        /* Test 2: Consistent hashing (virtual nodes) */
        print_str("[Test 2] Consistent Hashing\n");
        vnode_add(n1, 0); vnode_add(n1, 1); vnode_add(n1, 2);
        vnode_add(n2, 0); vnode_add(n2, 1); vnode_add(n2, 2);
        vnode_add(n3, 0); vnode_add(n3, 1); vnode_add(n3, 2);
        vnode_add(n4, 0); vnode_add(n4, 1); vnode_add(n4, 2);
        print_str("  Added "); print_int(vnode_count);
        print_str(" virtual nodes across "); print_int(node_count);
        print_str(" physical nodes\n");

        const char *test_keys[] = {"file_a.dat", "file_b.dat", "file_c.dat",
                                   "photo.jpg", "video.mp4", "index.html"};
        print_str("  Key routing (consistent hash):\n");
        for (int k = 0; k < 6; k++) {
            unsigned int h = hash_key(test_keys[k]);
            int owner = vnode_find_owner(h);
            print_str("    "); print_str(test_keys[k]);
            print_str(" -> hash="); print_uint(h);
            print_str(" -> node "); print_int(owner); print_str("\n");
        }
        print_str("\n");

        /* Test 3: Hash-based sharding */
        print_str("[Test 3] Hash-Based Sharding\n");
        int s1 = shard_create(SHARD_HASH, 0, 0);
        int s2 = shard_create(SHARD_HASH, 0, 0);
        int s3 = shard_create(SHARD_HASH, 0, 0);
        print_str("  Created hash shards: ");
        print_int(s1); print_str(", "); print_int(s2); print_str(", ");
        print_int(s3); print_str("\n");
        shard_assign_node(s1, n1);
        shard_assign_node(s2, n2);
        shard_assign_node(s3, n3);
        print_str("  Assigned primaries: shard1->node1, shard2->node2, shard3->node3\n");

        for (int k = 0; k < 6; k++) {
            int sid = shard_route_key(test_keys[k]);
            print_str("    "); print_str(test_keys[k]);
            print_str(" -> shard "); print_int(sid); print_str("\n");
        }
        print_str("\n");

        /* Test 4: Range-based sharding */
        print_str("[Test 4] Range-Based Sharding\n");
        int rs1 = shard_create(SHARD_RANGE, 0, 1000);
        int rs2 = shard_create(SHARD_RANGE, 1000, 2000);
        int rs3 = shard_create(SHARD_RANGE, 2000, 4000);
        shard_assign_node(rs1, n1);
        shard_assign_node(rs2, n2);
        shard_assign_node(rs3, n3);
        print_str("  Range shards: [0,1000)->N1 [1000,2000)->N2 [2000,4000)->N3\n");
        unsigned int test_vals[] = {500, 1500, 2500, 999, 3999};
        for (int v = 0; v < 5; v++) {
            int sid = shard_route_range(test_vals[v]);
            print_str("    key="); print_uint(test_vals[v]);
            print_str(" -> shard "); print_int(sid); print_str("\n");
        }
        print_str("\n");

        /* Test 5: Replica creation and sync */
        print_str("[Test 5] Replica Synchronization\n");
        int r1 = replica_create(s1, n1, REPLICA_PRIMARY);
        int r2 = replica_create(s1, n2, REPLICA_SECONDARY);
        int r3 = replica_create(s1, n3, REPLICA_SECONDARY);
        int r4 = replica_create(s1, n4, REPLICA_SECONDARY);
        print_str("  Created replicas for shard "); print_int(s1);
        print_str(": "); print_int(r1); print_str("(P), ");
        print_int(r2); print_str(", "); print_int(r3); print_str(", ");
        print_int(r4); print_str("\n");

        /* Simulate writes */
        int ver = replica_write(s1, 4096);
        print_str("  Write to shard "); print_int(s1);
        print_str(" -> version="); print_int(ver);
        print_str(" size=4096\n");
        ver = replica_write(s1, 8192);
        print_str("  Write to shard "); print_int(s1);
        print_str(" -> version="); print_int(ver);
        print_str(" size=8192\n");

        /* Sync async */
        int synced = replica_sync_shard(s1, SYNC_ASYNC);
        print_str("  Async sync triggered: "); print_int(synced);
        print_str(" replicas stale->syncing\n");

        /* Sync sync */
        synced = replica_sync_shard(s1, SYNC_SYNC);
        print_str("  Sync sync completed: "); print_int(synced);
        print_str(" replicas caught up\n");
        print_str("\n");

        /* Test 6: Metadata management */
        print_str("[Test 6] Metadata Management\n");
        int ino1 = file_create("/data/users/alice/photo.jpg", FILE_REGULAR, s1);
        int ino2 = file_create("/data/users/bob/document.pdf", FILE_REGULAR, s2);
        int ino3 = file_create("/data/shared/videos/", FILE_DIRECTORY, s3);
        print_str("  Created files: inode=");
        print_int(ino1); print_str(", "); print_int(ino2); print_str(", ");
        print_int(ino3); print_str("\n");

        meta_set(ino1, 1, 100);   /* owner_id */
        meta_set(ino1, 2, 0644);  /* permissions */
        meta_set(ino1, 3, 1);     /* encrypted */
        meta_set(ino2, 1, 200);
        meta_set(ino2, 2, 0600);
        meta_set(ino3, 1, 0);     /* shared */
        meta_set(ino3, 2, 0755);
        print_str("  Set metadata: owner, permissions, encryption flag\n");

        int lookup = file_lookup("/data/users/alice/photo.jpg");
        print_str("  Lookup '/data/users/alice/photo.jpg': inode=");
        print_int(lookup); print_str("\n");

        int owner = meta_get(ino1, 1);
        int perms = meta_get(ino1, 2);
        int enc = meta_get(ino1, 3);
        print_str("  Metadata for inode "); print_int(ino1);
        print_str(": owner="); print_int(owner);
        print_str(" perms="); print_int(perms);
        print_str(" encrypted="); print_int(enc); print_str("\n\n");

        /* Test 7: Failure recovery */
        print_str("[Test 7] Failure Recovery\n");
        cluster_status();

        /* Simulate node failure */
        node_t *fail_node = find_node(n1);
        if (fail_node) fail_node->state = NODE_FAILED;
        print_str("  SIMULATED FAILURE: node "); print_int(n1);
        print_str(" (node-alpha) marked as FAILED\n\n");

        cluster_status();
        print_str("\n  Running failure detection...\n");
        int detected = node_detect_failures(0);
        print_str("  Detected "); print_int(detected);
        print_str(" failed node(s)\n");

        print_str("  Running repair (promote replicas, re-replicate)...\n");
        int repaired = recovery_detect_and_repair();
        print_str("  Repaired "); print_int(repaired);
        print_str(" issue(s)\n\n");

        cluster_status();

        /* Test 8: Node recovery */
        print_str("\n[Test 8] Node Recovery\n");
        node_heartbeat(n1);
        print_str("  Node "); print_int(n1);
        print_str(" sent heartbeat -> state=RECOVERING\n");
        node_heartbeat(n1);
        print_str("  Node "); print_int(n1);
        print_str(" sent another heartbeat\n");

        file_entry_t *f = find_file_by_inode(ino1);
        if (f) {
            print_str("  File lookup by inode "); print_int(ino1);
            print_str(": path="); print_str(f->path);
            print_str(" shard="); print_int(f->shard_id);
            print_str(" ver="); print_int(f->version); print_str("\n");
        }

        print_str("\n=== DistributedFS Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
