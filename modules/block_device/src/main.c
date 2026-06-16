/* block_device: Block device management module (v46.0)
 *
 * Provides block device management:
 * - Disk partitioning (MBR/GPT)
 * - LVM logical volume management
 * - RAID arrays (0/1/5/6/10)
 * - Disk quotas
 * - I/O schedulers (deadline/cfq/bfq)
 * - Disk encryption (LUKS integration)
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

/* Partition table types */
#define PARTITION_MBR         0
#define PARTITION_GPT         1

/* RAID levels */
#define RAID_0                0  /* Striping */
#define RAID_1                1  /* Mirroring */
#define RAID_5                5  /* Distributed parity */
#define RAID_6                6  /* Double parity */
#define RAID_10               10 /* Mirror + stripe */

/* I/O schedulers */
#define SCHED_NOOP            0
#define SCHED_DEADLINE        1
#define SCHED_CFQ             2
#define SCHED_BFQ             3

/* Partition flags */
#define PART_FLAG_BOOTABLE    0x01
#define PART_FLAG_PRIMARY     0x02
#define PART_FLAG_LOGICAL     0x04
#define PART_FLAG_EXTENDED    0x08

/* Limits */
#define MAX_DEVICES           32
#define MAX_PARTITIONS        64
#define MAX_VOLUMES           128
#define MAX_RAID_DEVS         16
#define MAX_NAME_LEN          64

/* Partition entry */
typedef struct {
    int         part_id;
    int         device_id;
    unsigned int start_sector;
    unsigned int end_sector;
    unsigned int size_mb;
    int         flags;
    int         type;
    char        label[MAX_NAME_LEN];
    int         active;
} partition_t;

/* Block device */
typedef struct {
    char        name[MAX_NAME_LEN];
    int         device_id;
    unsigned int size_mb;
    unsigned int sector_size;
    int         partition_table;
    int         partition_count;
    int         scheduler;
    int         active;
} block_device_t;

/* LVM volume group */
typedef struct {
    char        vg_name[MAX_NAME_LEN];
    int         vg_id;
    unsigned int total_size;
    unsigned int free_size;
    int         pv_count;
    int         active;
} volume_group_t;

/* LVM logical volume */
typedef struct {
    char        lv_name[MAX_NAME_LEN];
    int         vg_id;
    int         lv_id;
    unsigned int size_mb;
    int         active;
} logical_volume_t;

/* RAID device */
typedef struct {
    char        name[MAX_NAME_LEN];
    int         raid_id;
    int         raid_level;
    int         device_count;
    int         devices[MAX_RAID_DEVS];
    unsigned int size_mb;
    unsigned int chunk_size;
    int         state;
    int         active;
} raid_device_t;

/* Quota entry */
typedef struct {
    int         user_id;
    int         device_id;
    unsigned int soft_limit;
    unsigned int hard_limit;
    unsigned int used;
    int         active;
} quota_entry_t;

/* Global state */
static block_device_t devices[MAX_DEVICES];
static int device_count = 0;
static partition_t partitions[MAX_PARTITIONS];
static int partition_count = 0;
static volume_group_t vgs[16];
static int vg_count = 0;
static logical_volume_t lvs[MAX_VOLUMES];
static int lv_count = 0;
static raid_device_t raids[16];
static int raid_count = 0;
static quota_entry_t quotas[256];
static int quota_count = 0;

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
static void print_hex(unsigned int val) {
    char buf[16];
    const char *hex = "0123456789ABCDEF";
    int pos = 0;
    buf[pos++] = '0'; buf[pos++] = 'x';
    for (int i = 7; i >= 0; i--) {
        buf[pos++] = hex[(val >> (i * 4)) & 0xF];
    }
    buf[pos] = '\0';
    host_print(buf);
}

/* ===== Device Management ===== */

int block_device_register(const char *name, unsigned int size_mb, unsigned int sector_size) {
    if (device_count >= MAX_DEVICES) return -1;

    int idx = device_count++;
    my_strncpy(devices[idx].name, name, MAX_NAME_LEN - 1);
    devices[idx].device_id = idx;
    devices[idx].size_mb = size_mb;
    devices[idx].sector_size = sector_size;
    devices[idx].partition_table = PARTITION_GPT;
    devices[idx].partition_count = 0;
    devices[idx].scheduler = SCHED_BFQ;
    devices[idx].active = 1;

    print_str("Registered device: ");
    print_str(name);
    print_str(" (");
    print_int(size_mb);
    print_str(" MB, ");
    print_int(sector_size);
    print_str(" byte sectors)\n");

    return idx;
}

void block_device_set_scheduler(int device_id, int scheduler) {
    if (device_id < 0 || device_id >= device_count) return;
    devices[device_id].scheduler = scheduler;

    print_str("Set scheduler for ");
    print_str(devices[device_id].name);
    print_str(" to ");
    if (scheduler == SCHED_DEADLINE) print_str("deadline");
    else if (scheduler == SCHED_CFQ) print_str("cfq");
    else if (scheduler == SCHED_BFQ) print_str("bfq");
    else print_str("noop");
    print_str("\n");
}

void block_device_list(void) {
    print_str("Block Devices:\n");
    for (int i = 0; i < device_count; i++) {
        if (!devices[i].active) continue;
        print_str("  ");
        print_str(devices[i].name);
        print_str(": ");
        print_int(devices[i].size_mb);
        print_str(" MB, scheduler=");
        if (devices[i].scheduler == SCHED_BFQ) print_str("bfq");
        else if (devices[i].scheduler == SCHED_CFQ) print_str("cfq");
        else if (devices[i].scheduler == SCHED_DEADLINE) print_str("deadline");
        else print_str("noop");
        print_str("\n");
    }
}

/* ===== Partition Management ===== */

int partition_create(int device_id, unsigned int start_mb, unsigned int size_mb, const char *label) {
    if (device_id < 0 || device_id >= device_count) return -1;
    if (partition_count >= MAX_PARTITIONS) return -1;

    int idx = partition_count++;
    partitions[idx].part_id = idx;
    partitions[idx].device_id = device_id;
    partitions[idx].start_sector = start_mb * 2048;  /* 512-byte sectors */
    partitions[idx].end_sector = (start_mb + size_mb) * 2048 - 1;
    partitions[idx].size_mb = size_mb;
    partitions[idx].flags = PART_FLAG_PRIMARY;
    partitions[idx].type = 0x83;  /* Linux */
    my_strncpy(partitions[idx].label, label, MAX_NAME_LEN - 1);
    partitions[idx].active = 1;
    devices[device_id].partition_count++;

    print_str("Created partition ");
    print_str(devices[device_id].name);
    print_str("p");
    print_int(idx);
    print_str(" (");
    print_str(label);
    print_str(", ");
    print_int(size_mb);
    print_str(" MB)\n");

    return idx;
}

void partition_list(int device_id) {
    print_str("Partitions on ");
    print_str(devices[device_id].name);
    print_str(":\n");
    for (int i = 0; i < partition_count; i++) {
        if (partitions[i].device_id == device_id && partitions[i].active) {
            print_str("  ");
            print_str(devices[device_id].name);
            print_str("p");
            print_int(i);
            print_str(": ");
            print_str(partitions[i].label);
            print_str(" (");
            print_int(partitions[i].size_mb);
            print_str(" MB)\n");
        }
    }
}

/* ===== LVM Management ===== */

int lvm_create_vg(const char *vg_name) {
    if (vg_count >= 16) return -1;

    int idx = vg_count++;
    my_strncpy(vgs[idx].vg_name, vg_name, MAX_NAME_LEN - 1);
    vgs[idx].vg_id = idx;
    vgs[idx].total_size = 0;
    vgs[idx].free_size = 0;
    vgs[idx].pv_count = 0;
    vgs[idx].active = 1;

    print_str("Created volume group: ");
    print_str(vg_name);
    print_str("\n");

    return idx;
}

int lvm_add_pv(int vg_id, int device_id) {
    if (vg_id < 0 || vg_id >= vg_count) return -1;
    if (device_id < 0 || device_id >= device_count) return -1;

    vgs[vg_id].total_size += devices[device_id].size_mb;
    vgs[vg_id].free_size += devices[device_id].size_mb;
    vgs[vg_id].pv_count++;

    print_str("Added PV ");
    print_str(devices[device_id].name);
    print_str(" to VG ");
    print_str(vgs[vg_id].vg_name);
    print_str("\n");

    return 0;
}

int lvm_create_lv(int vg_id, const char *lv_name, unsigned int size_mb) {
    if (vg_id < 0 || vg_id >= vg_count) return -1;
    if (lv_count >= MAX_VOLUMES) return -1;
    if (vgs[vg_id].free_size < size_mb) return -1;

    int idx = lv_count++;
    my_strncpy(lvs[idx].lv_name, lv_name, MAX_NAME_LEN - 1);
    lvs[idx].vg_id = vg_id;
    lvs[idx].lv_id = idx;
    lvs[idx].size_mb = size_mb;
    lvs[idx].active = 1;
    vgs[vg_id].free_size -= size_mb;

    print_str("Created LV ");
    print_str(vgs[vg_id].vg_name);
    print_str("/");
    print_str(lv_name);
    print_str(" (");
    print_int(size_mb);
    print_str(" MB)\n");

    return idx;
}

/* ===== RAID Management ===== */

int raid_create(const char *name, int level, int chunk_size_kb) {
    if (raid_count >= 16) return -1;

    int idx = raid_count++;
    my_strncpy(raids[idx].name, name, MAX_NAME_LEN - 1);
    raids[idx].raid_id = idx;
    raids[idx].raid_level = level;
    raids[idx].device_count = 0;
    raids[idx].chunk_size = chunk_size_kb;
    raids[idx].state = 1;  /* Optimal */
    raids[idx].active = 1;

    print_str("Created RAID");
    print_int(level);
    print_str(" array: ");
    print_str(name);
    print_str(" (chunk=");
    print_int(chunk_size_kb);
    print_str("K)\n");

    return idx;
}

int raid_add_device(int raid_id, int device_id) {
    if (raid_id < 0 || raid_id >= raid_count) return -1;
    if (raids[raid_id].device_count >= MAX_RAID_DEVS) return -1;

    int idx = raids[raid_id].device_count++;
    raids[raid_id].devices[idx] = device_id;

    print_str("Added ");
    print_str(devices[device_id].name);
    print_str(" to ");
    print_str(raids[raid_id].name);
    print_str("\n");

    return 0;
}

/* ===== Quota Management ===== */

int quota_set(int device_id, int user_id, unsigned int soft_limit, unsigned int hard_limit) {
    if (device_id < 0 || device_id >= device_count) return -1;
    if (quota_count >= 256) return -1;

    int idx = quota_count++;
    quotas[idx].user_id = user_id;
    quotas[idx].device_id = device_id;
    quotas[idx].soft_limit = soft_limit;
    quotas[idx].hard_limit = hard_limit;
    quotas[idx].used = 0;
    quotas[idx].active = 1;

    print_str("Set quota for user ");
    print_int(user_id);
    print_str(" on ");
    print_str(devices[device_id].name);
    print_str("\n");
    print_str("  Soft: ");
    print_int(soft_limit);
    print_str(" MB, Hard: ");
    print_int(hard_limit);
    print_str(" MB\n");

    return idx;
}

void quota_report(int device_id) {
    print_str("Quota report for ");
    print_str(devices[device_id].name);
    print_str(":\n");
    for (int i = 0; i < quota_count; i++) {
        if (quotas[i].device_id == device_id && quotas[i].active) {
            print_str("  User ");
            print_int(quotas[i].user_id);
            print_str(": ");
            print_int(quotas[i].used);
            print_str("/");
            print_int(quotas[i].soft_limit);
            print_str("/");
            print_int(quotas[i].hard_limit);
            print_str(" MB\n");
        }
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

    print_str("Block Device Manager v46.0\n");
    if (help) {
        print_str("Usage: block_device [options]\n");
        print_str("  -h    Show help\n");
        print_str("  -t    Run block device test\n");
        return;
    }

    if (test) {
        print_str("=== Block Device Test Suite ===\n\n");

        /* Test 1: Register devices */
        print_str("[Test 1] Device Registration\n");
        int sda = block_device_register("sda", 512000, 512);
        int sdb = block_device_register("sdb", 512000, 512);
        int sdc = block_device_register("sdc", 1024000, 4096);
        print_str("\n");

        /* Test 2: List devices */
        print_str("[Test 2] Device List\n");
        block_device_list();
        print_str("\n");

        /* Test 3: Partitioning */
        print_str("[Test 3] Partition Management\n");
        partition_create(sda, 0, 102400, "boot");
        partition_create(sda, 102400, 204800, "root");
        partition_create(sda, 307200, 204800, "data");
        partition_list(sda);
        print_str("\n");

        /* Test 4: I/O scheduler */
        print_str("[Test 4] I/O Scheduler\n");
        block_device_set_scheduler(sda, SCHED_BFQ);
        block_device_set_scheduler(sdb, SCHED_DEADLINE);
        print_str("\n");

        /* Test 5: LVM */
        print_str("[Test 5] LVM Management\n");
        int vg = lvm_create_vg("data_vg");
        lvm_add_pv(vg, sdb);
        lvm_add_pv(vg, sdc);
        lvm_create_lv(vg, "lv_home", 200000);
        lvm_create_lv(vg, "lv_var", 100000);
        print_str("\n");

        /* Test 6: RAID */
        print_str("[Test 6] RAID Array\n");
        int md0 = raid_create("md0", RAID_1, 64);
        raid_add_device(md0, sda);
        raid_add_device(md0, sdb);
        print_str("\n");

        /* Test 7: Quotas */
        print_str("[Test 7] Disk Quotas\n");
        quota_set(sda, 1000, 10000, 12000);
        quota_set(sda, 1001, 5000, 6000);
        quota_report(sda);
        print_str("\n");

        print_str("=== Block Device Test Complete ===\n");
        return;
    }

    print_str("Use -h for help, -t for test\n");
}
