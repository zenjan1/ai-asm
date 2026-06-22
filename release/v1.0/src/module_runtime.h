/*
 * aiasm-aarch64/kernel/module_runtime.h
 * Multi-module WASM runtime data structures v0.5
 */

#ifndef MODULE_RUNTIME_H
#define MODULE_RUNTIME_H

#include "wasm3.h"

#define MAX_MODULES         16
#define MAX_RAMDISK_FILES   64
#define MAX_OPEN_FILES      16
#define MAX_NAME_LEN        32
#define MAX_PATH_LEN        128

typedef enum {
    MOD_FREE = 0,
    MOD_LOADING,
    MOD_READY,
    MOD_RUNNING,
    MOD_EXITED
} module_state_t;

/* Per-module WASM runtime slot */
typedef struct {
    module_state_t state;
    uint32_t id;
    char name[MAX_NAME_LEN];
    IM3Environment env;
    IM3Runtime runtime;
    IM3Module module;
    IM3Function entry;
    int exit_code;
} wasm_module_slot_t;

/* RAM disk file entry (parsed from USTAR TAR) */
typedef struct {
    char name[MAX_PATH_LEN];
    uint32_t offset;   /* offset within ramdisk_data (after 512-byte header) */
    uint32_t size;
} ramdisk_file_t;

/* Open file descriptor */
typedef struct {
    int active;
    int file_index;    /* index into ramdisk_files[] */
    uint32_t position; /* read position */
} open_file_t;

/* Embedded WASM registry entry (name → embedded bytes) */
typedef struct {
    const char *name;
    const uint8_t *wasm_bytes;
    uint32_t wasm_size;
} wasm_registry_entry_t;

/* Global tables (defined in wasm_host.c) */
extern IM3Environment g_shared_env;
extern wasm_module_slot_t module_table[MAX_MODULES];
extern uint32_t module_count;
extern uint32_t next_module_id;
extern uint32_t current_module_id;

extern ramdisk_file_t ramdisk_files[MAX_RAMDISK_FILES];
extern uint32_t ramdisk_file_count;

/* Function declarations */
const char *wasm_host_init_multi(void);
const char *load_module(const char *name, uint32_t *out_id);
const char *run_module(uint32_t module_id);
void terminate_module(uint32_t module_id);
int find_ready_module(void);

/* RAM disk operations */
void ramdisk_init(const uint8_t *data, uint32_t size);
int ramdisk_open(const char *path, uint32_t path_len);
int ramdisk_read(int fd, uint8_t *buf, uint32_t len);
void ramdisk_close(int fd);
int ramdisk_list(uint8_t *buf, uint32_t max_len);

#endif /* MODULE_RUNTIME_H */
