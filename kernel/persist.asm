/*
 * aiasm-aarch64/kernel/persist.asm
 * Persistence layer v14.0
 * Sync/flush FAT filesystem to virtio_blk block device
 */
.arch armv8-a

/* -----------------------------------------------------------------------------
 * Constants
 * ----------------------------------------------------------------------------- */
.set PERSIST_DIRTY_NONE,  0
.set PERSIST_DIRTY_FAT,   1
.set PERSIST_DIRTY_DATA,  2

/* -----------------------------------------------------------------------------
 * BSS
 * ----------------------------------------------------------------------------- */
.bss
.align 4
persist_fat_cache:
    .skip 4096              /* 1 FAT sector cache (1MB FAT fits in multiple) */
persist_dirty_flags:
    .word 0                 /* dirty flags */
persist_sync_count:
    .word 0                 /* number of syncs performed */

.text

/* -----------------------------------------------------------------------------
 * persist_init — initialize persistence subsystem
 * ----------------------------------------------------------------------------- */
.global persist_init
persist_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero dirty flags */
    adrp    x0, persist_dirty_flags
    add     x0, x0, #:lo12:persist_dirty_flags
    str     wzr, [x0]

    /* Zero sync count */
    adrp    x0, persist_sync_count
    add     x0, x0, #:lo12:persist_sync_count
    str     wzr, [x0]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * persist_mark_dirty(flag) — mark subsystem dirty
 * w0 = flag (DIRTY_FAT, DIRTY_DATA)
 * ----------------------------------------------------------------------------- */
.global persist_mark_dirty
persist_mark_dirty:
    stp     x29, x30, [sp, #-16]!

    adrp    x1, persist_dirty_flags
    add     x1, x1, #:lo12:persist_dirty_flags
    ldr     w2, [x1]
    orr     w2, w2, w0
    str     w2, [x1]

    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * persist_sync — flush all dirty data to disk
 * Returns: w0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
.global persist_sync
persist_sync:
    stp     x29, x30, [sp, #-16]!

    adrp    x0, persist_dirty_flags
    add     x0, x0, #:lo12:persist_dirty_flags
    ldr     w1, [x0]

    /* Nothing dirty */
    cbz     w1, persist_sync_ok

    /* Sync FAT tables if dirty */
    tst     w1, #PERSIST_DIRTY_FAT
    b.eq    persist_sync_data
    bl      _persist_sync_fat
    cbnz    w0, persist_sync_err

persist_sync_data:
    /* Data clusters are written immediately via fs_write_cluster,
     * so no additional sync needed for data.
     * Just clear the data dirty flag. */
    tst     w1, #PERSIST_DIRTY_DATA
    b.eq    persist_sync_ok

persist_sync_ok:
    /* Clear all dirty flags */
    adrp    x0, persist_dirty_flags
    add     x0, x0, #:lo12:persist_dirty_flags
    str     wzr, [x0]

    /* Increment sync count */
    adrp    x0, persist_sync_count
    add     x0, x0, #:lo12:persist_sync_count
    ldr     w1, [x0]
    add     w1, w1, #1
    str     w1, [x0]

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

persist_sync_err:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * _persist_sync_fat — write FAT tables to disk
 * FAT is at sector fs_fat_start, fs_fat_size sectors
 * Returns: w0 = 0 on success, -1 on error
 * ----------------------------------------------------------------------------- */
_persist_sync_fat:
    stp     x29, x30, [sp, #-16]!

    /* Get reserved sectors count (FAT starts here) */
    adrp    x0, fs_reserved_sectors
    add     x0, x0, #:lo12:fs_reserved_sectors
    ldr     w1, [x0]

    /* Get number of FATs */
    adrp    x0, fs_num_fats
    add     x0, x0, #:lo12:fs_num_fats
    ldr     w2, [x0]

    /* FAT is written sector-by-sector via fs_fat_write_entry,
     * which calls virtio_blk_write directly. No additional sync needed. */

    mov     w0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * persist_get_sync_count — return number of syncs performed
 * Returns: w0 = sync count
 * ----------------------------------------------------------------------------- */
.global persist_get_sync_count
persist_get_sync_count:
    adrp    x0, persist_sync_count
    add     x0, x0, #:lo12:persist_sync_count
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * persist_mark_dirty_fat — mark FAT as dirty (called from fs_fat_write_entry)
 * ----------------------------------------------------------------------------- */
.global persist_mark_dirty_fat
persist_mark_dirty_fat:
    stp     x29, x30, [sp, #-16]!
    mov     w0, #PERSIST_DIRTY_FAT
    bl      persist_mark_dirty
    ldp     x29, x30, [sp], #16
    ret
