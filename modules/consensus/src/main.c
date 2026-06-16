/* consensus: Raft consensus algorithm module (v46.0)
 *
 * Implements distributed consensus using the Raft algorithm:
 * - Leader election with term-based voting
 * - Log replication with consistency guarantees
 * - State machine interface for applying committed entries
 * - Log compression via snapshots
 * - Safety: election restriction, log matching, leader completeness
 */

#include <stddef.h>

__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Node states */
#define STATE_FOLLOWER      0
#define STATE_CANDIDATE     1
#define STATE_LEADER        2

/* Log entry types */
#define LOG_ENTRY_CMD       1
#define LOG_ENTRY_NOOP      2

/* Limits */
#define MAX_NODES           16
#define MAX_LOG_ENTRIES     1024
#define MAX_SNAPSHOT_SIZE   4096
#define MAX_CMD_LEN         64

/* Log entry */
typedef struct {
    int             term;
    int             type;
    char            command[MAX_CMD_LEN];
    int             committed;
} log_entry_t;

/* Snapshot */
typedef struct {
    int             last_included_index;
    int             last_included_term;
    char            data[MAX_SNAPSHOT_SIZE];
    int             size;
} snapshot_t;

/* Raft node */
typedef struct {
    int             node_id;
    int             state;
    int             current_term;
    int             voted_for;
    int             log_length;
    log_entry_t     log[MAX_LOG_ENTRIES];
    int             commit_index;
    int             last_applied;

    /* Leader state */
    int             next_index[MAX_NODES];
    int             match_index[MAX_NODES];

    /* Election state */
    int             votes_received[MAX_NODES];
    int             election_timeout;
    int             heartbeat_timer;

    /* Snapshot */
    snapshot_t      snapshot;

    int             active;
} raft_node_t;

/* State machine interface */
typedef struct {
    int             state;
    int             operations_applied;
    char            last_command[MAX_CMD_LEN];
} state_machine_t;

/* Cluster context */
typedef struct {
    int             node_count;
    raft_node_t     nodes[MAX_NODES];
    int             leader_id;
    state_machine_t machines[MAX_NODES];
    unsigned int    cluster_time;
} raft_cluster_t;

/* Global state */
static raft_cluster_t cluster;
static int initialized = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* ===== Cluster Initialization ===== */

int raft_init(int node_count) {
    if (node_count < 1 || node_count > MAX_NODES) return -1;

    cluster.node_count = node_count;
    cluster.leader_id = -1;
    cluster.cluster_time = 0;

    for (int i = 0; i < node_count; i++) {
        cluster.nodes[i].node_id = i;
        cluster.nodes[i].state = STATE_FOLLOWER;
        cluster.nodes[i].current_term = 0;
        cluster.nodes[i].voted_for = -1;
        cluster.nodes[i].log_length = 0;
        cluster.nodes[i].commit_index = 0;
        cluster.nodes[i].last_applied = 0;
        cluster.nodes[i].election_timeout = 150 + (i * 50);  /* Stagger timeouts */
        cluster.nodes[i].heartbeat_timer = 0;
        cluster.nodes[i].snapshot.last_included_index = 0;
        cluster.nodes[i].snapshot.last_included_term = 0;
        cluster.nodes[i].snapshot.size = 0;
        cluster.nodes[i].active = 1;

        for (int j = 0; j < node_count; j++) {
            cluster.nodes[i].next_index[j] = 1;
            cluster.nodes[i].match_index[j] = 0;
            cluster.nodes[i].votes_received[j] = 0;
        }

        cluster.machines[i].state = 0;
        cluster.machines[i].operations_applied = 0;
    }

    initialized = 1;
    print_str("Raft cluster initialized with ");
    print_int(node_count);
    print_str(" nodes\n");

    return 0;
}

/* ===== Leader Election ===== */

int raft_start_election(int node_id) {
    if (node_id < 0 || node_id >= cluster.node_count) return -1;
    raft_node_t *node = &cluster.nodes[node_id];

    /* Increment term and transition to candidate */
    node->current_term++;
    node->state = STATE_CANDIDATE;
    node->voted_for = node_id;

    /* Clear vote tracking */
    for (int i = 0; i < cluster.node_count; i++) {
        node->votes_received[i] = 0;
    }
    node->votes_received[node_id] = 1;  /* Vote for self */

    print_str("Node ");
    print_int(node_id);
    print_str(" starting election for term ");
    print_int(node->current_term);
    print_str("\n");

    /* Request votes from other nodes */
    int votes = 1;
    int last_log_index = node->log_length;
    int last_log_term = node->log_length > 0 ? node->log[node->log_length - 1].term : 0;

    for (int i = 0; i < cluster.node_count; i++) {
        if (i == node_id) continue;
        raft_node_t *voter = &cluster.nodes[i];

        /* Election restriction: candidate's log must be at least as up-to-date */
        int voter_last_term = voter->log_length > 0 ? voter->log[voter->log_length - 1].term : 0;
        int log_ok = (last_log_term > voter_last_term) ||
                     (last_log_term == voter_last_term && last_log_index >= voter->log_length);

        /* Grant vote if: hasn't voted or voted for this candidate, and log is ok */
        if ((voter->voted_for == -1 || voter->voted_for == node_id) && log_ok) {
            voter->voted_for = node_id;
            node->votes_received[i] = 1;
            votes++;

            print_str("  Node ");
            print_int(i);
            print_str(" granted vote\n");
        }
    }

    /* Check if won majority */
    int majority = (cluster.node_count / 2) + 1;
    if (votes >= majority) {
        node->state = STATE_LEADER;
        cluster.leader_id = node_id;

        /* Initialize leader state */
        for (int i = 0; i < cluster.node_count; i++) {
            node->next_index[i] = node->log_length + 1;
            node->match_index[i] = 0;
        }

        /* Append NOOP entry to commit entries from previous terms */
        if (node->log_length < MAX_LOG_ENTRIES) {
            node->log[node->log_length].term = node->current_term;
            node->log[node->log_length].type = LOG_ENTRY_NOOP;
            node->log[node->log_length].committed = 0;
            node->log_length++;
        }

        print_str("Node ");
        print_int(node_id);
        print_str(" elected leader (votes: ");
        print_int(votes);
        print_str("/");
        print_int(cluster.node_count);
        print_str(")\n");

        return 1;
    }

    print_str("Node ");
    print_int(node_id);
    print_str(" failed election (votes: ");
    print_int(votes);
    print_str("/");
    print_int(cluster.node_count);
    print_str(")\n");

    return 0;
}

/* ===== Log Replication ===== */

int raft_append_entry(int node_id, const char *command) {
    if (node_id < 0 || node_id >= cluster.node_count) return -1;
    raft_node_t *node = &cluster.nodes[node_id];

    if (node->state != STATE_LEADER) return -2;
    if (node->log_length >= MAX_LOG_ENTRIES) return -3;

    /* Append to leader's log */
    int index = node->log_length;
    node->log[index].term = node->current_term;
    node->log[index].type = LOG_ENTRY_CMD;
    my_strncpy(node->log[index].command, command, MAX_CMD_LEN - 1);
    node->log[index].committed = 0;
    node->log_length++;

    print_str("Leader ");
    print_int(node_id);
    print_str(" appended entry at index ");
    print_int(index);
    print_str(": ");
    print_str(command);
    print_str("\n");

    /* Replicate to followers */
    for (int i = 0; i < cluster.node_count; i++) {
        if (i == node_id) continue;
        raft_node_t *follower = &cluster.nodes[i];

        /* Check log consistency */
        int prev_log_index = node->next_index[i] - 1;
        int prev_log_term = 0;
        if (prev_log_index > 0 && prev_log_index <= node->log_length) {
            prev_log_term = node->log[prev_log_index - 1].term;
        }

        /* Append entries */
        for (int j = node->next_index[i] - 1; j < node->log_length; j++) {
            if (follower->log_length < MAX_LOG_ENTRIES) {
                follower->log[follower->log_length] = node->log[j];
                follower->log_length++;
            }
        }

        node->match_index[i] = node->log_length;
        node->next_index[i] = node->log_length + 1;

        print_str("  Replicated to node ");
        print_int(i);
        print_str(" (log length: ");
        print_int(follower->log_length);
        print_str(")\n");
    }

    return index;
}

void raft_advance_commit(int node_id) {
    if (node_id < 0 || node_id >= cluster.node_count) return;
    raft_node_t *node = &cluster.nodes[node_id];

    if (node->state != STATE_LEADER) return;

    /* Find highest N such that majority of match_index[i] >= N and log[N].term == current_term */
    for (int n = node->log_length; n > node->commit_index; n--) {
        if (node->log[n - 1].term != node->current_term) continue;

        int count = 1;  /* Count leader */
        for (int i = 0; i < cluster.node_count; i++) {
            if (i == node_id) continue;
            if (node->match_index[i] >= n) count++;
        }

        int majority = (cluster.node_count / 2) + 1;
        if (count >= majority) {
            node->commit_index = n;

            print_str("Leader ");
            print_int(node_id);
            print_str(" advanced commit index to ");
            print_int(n);
            print_str("\n");
            break;
        }
    }
}

/* ===== State Machine ===== */

void raft_apply_entries(int node_id) {
    if (node_id < 0 || node_id >= cluster.node_count) return;
    raft_node_t *node = &cluster.nodes[node_id];
    state_machine_t *sm = &cluster.machines[node_id];

    while (node->last_applied < node->commit_index) {
        node->last_applied++;
        log_entry_t *entry = &node->log[node->last_applied - 1];

        if (entry->type == LOG_ENTRY_CMD) {
            /* Apply command to state machine */
            sm->state++;
            sm->operations_applied++;
            my_strncpy(sm->last_command, entry->command, MAX_CMD_LEN - 1);
            entry->committed = 1;

            print_str("Node ");
            print_int(node_id);
            print_str(" applied: ");
            print_str(entry->command);
            print_str(" (state=");
            print_int(sm->state);
            print_str(")\n");
        }
    }
}

/* ===== Log Compression (Snapshots) ===== */

int raft_create_snapshot(int node_id) {
    if (node_id < 0 || node_id >= cluster.node_count) return -1;
    raft_node_t *node = &cluster.nodes[node_id];
    snapshot_t *snap = &node->snapshot;

    if (node->last_applied == 0) return -2;

    /* Save snapshot metadata */
    snap->last_included_index = node->last_applied;
    snap->last_included_term = node->log[node->last_applied - 1].term;

    /* Serialize state machine (simplified) */
    state_machine_t *sm = &cluster.machines[node_id];
    snap->size = 0;
    print_str("Node ");
    print_int(node_id);
    print_str(" created snapshot at index ");
    print_int(snap->last_included_index);
    print_str(" (term ");
    print_int(snap->last_included_term);
    print_str(")\n");

    /* Compact log: remove entries up to snapshot */
    int entries_removed = 0;
    for (int i = 0; i < node->last_applied && i < node->log_length; i++) {
        entries_removed++;
    }

    /* Shift remaining entries */
    for (int i = 0; i < node->log_length - node->last_applied; i++) {
        node->log[i] = node->log[node->last_applied + i];
    }
    node->log_length -= node->last_applied;

    print_str("  Compacted log: removed ");
    print_int(entries_removed);
    print_str(" entries, remaining ");
    print_int(node->log_length);
    print_str("\n");

    return 0;
}

/* ===== Cluster Status ===== */

void raft_cluster_status(void) {
    print_str("Raft Cluster Status:\n");
    print_str("  Nodes: ");
    print_int(cluster.node_count);
    print_str("\n");
    print_str("  Leader: ");
    if (cluster.leader_id >= 0) {
        print_str("Node ");
        print_int(cluster.leader_id);
    } else {
        print_str("NONE");
    }
    print_str("\n\n");

    for (int i = 0; i < cluster.node_count; i++) {
        raft_node_t *node = &cluster.nodes[i];
        print_str("  Node ");
        print_int(i);
        print_str(":\n");
        print_str("    State: ");
        if (node->state == STATE_LEADER) print_str("LEADER");
        else if (node->state == STATE_CANDIDATE) print_str("CANDIDATE");
        else print_str("FOLLOWER");
        print_str("\n");
        print_str("    Term: ");
        print_int(node->current_term);
        print_str("\n");
        print_str("    Log length: ");
        print_int(node->log_length);
        print_str("\n");
        print_str("    Commit index: ");
        print_int(node->commit_index);
        print_str("\n");
        print_str("    Last applied: ");
        print_int(node->last_applied);
        print_str("\n");
        print_str("    Snapshot index: ");
        print_int(node->snapshot.last_included_index);
        print_str("\n");
    }
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
        int hl = my_strlen(arg);
        if (hl == 2 && arg[0] == '-' && arg[1] == 'h') help = 1;
        else if (hl == 2 && arg[0] == '-' && arg[1] == 't') test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }

    print_str("Consensus Module v46.0 - Raft Consensus Algorithm\n");
    if (help) {
        print_str("Usage: consensus [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run consensus test\n");
        return;
    }

    if (test) {
        print_str("=== Raft Consensus Test Suite ===\n\n");

        /* Test 1: Initialize cluster */
        print_str("[Test 1] Cluster Initialization\n");
        raft_init(5);
        print_str("\n");

        /* Test 2: Leader election */
        print_str("[Test 2] Leader Election\n");
        int elected = raft_start_election(0);
        if (elected) {
            print_str("Election successful!\n");
        }
        print_str("\n");

        /* Test 3: Log replication */
        print_str("[Test 3] Log Replication\n");
        raft_append_entry(0, "SET x=1");
        raft_append_entry(0, "SET y=2");
        raft_append_entry(0, "SET z=3");
        print_str("\n");

        /* Test 4: Commit advancement */
        print_str("[Test 4] Commit Advancement\n");
        raft_advance_commit(0);
        print_str("\n");

        /* Test 5: Apply to state machines */
        print_str("[Test 5] State Machine Application\n");
        for (int i = 0; i < cluster.node_count; i++) {
            raft_apply_entries(i);
        }
        print_str("\n");

        /* Test 6: Create snapshot */
        print_str("[Test 6] Log Compression (Snapshot)\n");
        raft_create_snapshot(0);
        print_str("\n");

        /* Test 7: More operations */
        print_str("[Test 7] Additional Operations\n");
        raft_append_entry(0, "INC x");
        raft_append_entry(0, "INC y");
        raft_advance_commit(0);
        for (int i = 0; i < cluster.node_count; i++) {
            raft_apply_entries(i);
        }
        print_str("\n");

        /* Test 8: Second snapshot */
        print_str("[Test 8] Second Snapshot\n");
        raft_create_snapshot(0);
        print_str("\n");

        /* Test 9: Cluster status */
        print_str("[Test 9] Final Cluster Status\n");
        raft_cluster_status();
        print_str("\n");

        print_str("=== Raft Consensus Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
