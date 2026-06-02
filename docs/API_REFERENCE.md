# AI-ASM OS API Reference (v28.0)

## System Calls (Syscall Interface)

### Process Management (syscall 0-19)

| Number | Name | Description |
|--------|------|-------------|
| 0 | `sys_fork` | Create child process |
| 1 | `sys_exit` | Terminate current process |
| 2 | `sys_wait` | Wait for child process |
| 3 | `sys_exec` | Execute WASM module |
| 4 | `sys_get_pid` | Get current process ID |
| 5 | `sys_get_ppid` | Get parent process ID |
| 6 | `sys_get_status` | Get process status |
| 7 | `sys_yield` | Yield CPU to scheduler |
| 8 | `sys_sleep` | Sleep for N milliseconds |
| 9 | `sys_spawn` | Spawn new WASM module |
| 10 | `sys_terminate` | Terminate target process |
| 11 | `sys_list_processes` | List all processes |

### Memory Management (syscall 20-39)

| Number | Name | Description |
|--------|------|-------------|
| 20 | `sys_alloc` | Allocate memory block |
| 21 | `sys_free` | Free memory block |
| 22 | `sys_realloc` | Reallocate memory |
| 23 | `sys_mmap` | Memory map region |
| 24 | `sys_munmap` | Unmap memory region |
| 25 | `sys_mprotect` | Set memory protection |
| 26 | `sys_get_mem_info` | Get memory statistics |

### File System (syscall 40-59)

| Number | Name | Description |
|--------|------|-------------|
| 40 | `sys_open` | Open file |
| 41 | `sys_close` | Close file |
| 42 | `sys_read` | Read from file |
| 43 | `sys_write` | Write to file |
| 44 | `sys_seek` | Seek in file |
| 45 | `sys_create` | Create file |
| 46 | `sys_delete` | Delete file |
| 47 | `sys_list` | List directory |
| 48 | `sys_get_cwd` | Get working directory |
| 49 | `sys_set_cwd` | Set working directory |

### Network (syscall 60-79)

| Number | Name | Description |
|--------|------|-------------|
| 60 | `sys_connect` | TCP connect |
| 61 | `sys_listen` | TCP listen |
| 62 | `sys_accept` | TCP accept |
| 63 | `sys_send` | Send data |
| 64 | `sys_recv` | Receive data |
| 65 | `sys_close_conn` | Close connection |

### IPC (syscall 80-99)

| Number | Name | Description |
|--------|------|-------------|
| 80 | `sys_pipe_create` | Create pipe |
| 81 | `sys_pipe_read` | Read from pipe |
| 82 | `sys_pipe_write` | Write to pipe |
| 83 | `sys_msgq_create` | Create message queue |
| 84 | `sys_msgq_send` | Send message |
| 85 | `sys_msgq_recv` | Receive message |
| 86 | `sys_shmem_alloc` | Allocate shared memory |
| 87 | `sys_shmem_free` | Free shared memory |
| 88 | `sys_sem_create` | Create semaphore |
| 89 | `sys_sem_wait` | Wait on semaphore |
| 90 | `sys_sem_post` | Post semaphore |

### Agent (syscall 100-119)

| Number | Name | Description |
|--------|------|-------------|
| 100 | `sys_agent_create` | Create agent |
| 101 | `sys_agent_send` | Send to agent |
| 102 | `sys_agent_receive` | Receive from agent |
| 103 | `sys_agent_destroy` | Destroy agent |

### Security (syscall 120-139)

| Number | Name | Description |
|--------|------|-------------|
| 120 | `sys_perm_get` | Get permission level |
| 121 | `sys_perm_set` | Set permission level |
| 122 | `sys_audit_log` | Write audit log entry |
| 123 | `sys_audit_query` | Query audit log |
| 124 | `sys_user_login` | Authenticate user |
| 125 | `sys_quota_get` | Get resource quota |
| 126 | `sys_quota_set` | Set resource quota |

### Maintenance (syscall 140-159)

| Number | Name | Description |
|--------|------|-------------|
| 140 | `sys_event_subscribe` | Subscribe to kernel event |
| 141 | `sys_event_poll` | Poll kernel events |
| 142 | `sys_persist_sync` | Sync persistence |
| 150 | `sys_update_check` | Check for security updates |
| 151 | `sys_patch_apply` | Apply security patch |
| 152 | `sys_rollback` | Rollback patch |

---

## Host Functions (WASM Import Interface)

### Core Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_print` | `v(ii)` | Print string to console |
| `host_get_tick` | `I()` | Get system tick count |
| `host_exit` | `v(i)` | Exit WASM module with code |
| `host_alloc` | `i(ii)` | Allocate memory (size, align) |
| `host_free` | `v(i)` | Free memory |
| `host_getc` | `i()` | Read character from stdin |
| `host_spawn` | `i(ii)` | Spawn module by name |
| `host_get_argv` | `i(ii)` | Get command-line arguments |
| `host_set_argv` | `v(ii)` | Set command-line arguments |

### File System Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_fs_open` | `i(ii)` | Open file by name |
| `host_fs_read` | `i(iii)` | Read from file handle |
| `host_fs_write` | `i(iii)` | Write to file handle |
| `host_fs_create` | `i(ii)` | Create new file |
| `host_fs_delete` | `i(ii)` | Delete file |
| `host_fs_close` | `v(i)` | Close file handle |
| `host_fs_list` | `i(ii)` | List directory contents |
| `host_get_cwd` | `i(ii)` | Get current working directory |

### Network Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_net_connect` | `i(iii)` | Connect to host:port |
| `host_net_send` | `i(iii)` | Send data to socket |
| `host_net_recv` | `i(iii)` | Receive data from socket |
| `host_net_close` | `v(i)` | Close socket |
| `host_net_listen` | `i(i)` | Listen on port |
| `host_net_accept` | `i(i)` | Accept connection |

### IPC Functions

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_pipe_create` | `i(ii)` | Create named pipe |
| `host_pipe_read` | `i(iii)` | Read from pipe |
| `host_pipe_write` | `i(iii)` | Write to pipe |
| `host_pipe_close` | `v(i)` | Close pipe |
| `host_msgq_create` | `i()` | Create message queue |
| `host_msgq_send` | `i(ii)` | Send message to queue |
| `host_msgq_recv` | `i(ii)` | Receive message from queue |
| `host_shmem_alloc` | `i(ii)` | Allocate shared memory |
| `host_shmem_read` | `i(iii)` | Read shared memory |
| `host_shmem_write` | `i(iii)` | Write shared memory |
| `host_sem_create` | `i(i)` | Create semaphore |
| `host_sem_wait` | `i(i)` | Wait semaphore |
| `host_sem_post` | `i(i)` | Post semaphore |

### Performance Functions (v27.1)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_perf_get_cycles` | `I()` | Get CPU cycle count |
| `host_perf_get_instructions` | `I()` | Get instruction count |
| `host_perf_get_cache_miss` | `I()` | Get cache miss count |
| `host_perf_get_memory_bw` | `I()` | Get memory bandwidth |
| `host_perf_reset` | `v()` | Reset performance counters |

### Maintenance Functions (v27.1)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_maintenance_check` | `i()` | Check update status |
| `host_maintenance_apply` | `i(i)` | Apply patch by ID |
| `host_maintenance_verify` | `i(i)` | Verify update signature |
| `host_maintenance_rollback` | `i(i)` | Rollback to version |
| `host_maintenance_status` | `i()` | Get maintenance status |

### Apple Silicon Functions (v27.2)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_apple_get_soc` | `i()` | Get SoC type |
| `host_apple_amx_init` | `i()` | Initialize AMX |
| `host_apple_amx_compute` | `i(i)` | Run AMX computation |
| `host_apple_neural_infer` | `i(i)` | Run Neural Engine inference |
| `host_apple_gpu_compute` | `i(i)` | Run GPU computation |

### Quantum Functions (v27.2)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_quantum_init` | `i(i)` | Initialize N qubits |
| `host_quantum_gate` | `i(ii)` | Apply gate to qubit |
| `host_quantum_measure` | `i(i)` | Measure qubit |
| `host_quantum_entangle` | `i(ii)` | Entangle two qubits |

### Neuromorphic Functions (v27.3)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_snn_create_network` | `i(ii)` | Create SNN network |
| `host_snn_process_spike` | `i(ii)` | Process spike event |
| `host_snn_stdp_learn` | `v()` | Apply STDP learning |
| `host_snn_get_energy` | `i()` | Get energy consumption |

### Photonics Functions (v27.3)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_photonics_init_chip` | `i(i)` | Initialize photonic chip |
| `host_photonics_create_channel` | `i(ii)` | Create optical channel |
| `host_photonics_route` | `i(ii)` | Create optical route |
| `host_photonics_compute` | `i(i)` | Execute optical compute |

### Ethical/AGI Functions (v27.4)

| Function | Signature | Description |
|----------|-----------|-------------|
| `host_ethical_check` | `i(i)` | Check ethical score |
| `host_meta_cognition_level` | `i()` | Get meta-cognition level |
| `host_self_improve_cycle` | `v()` | Run self-improvement |
| `host_social_context` | `i(i)` | Get social context score |

---

## WASI Interface (snapshot_preview1)

| Function | Signature | Description |
|----------|-----------|-------------|
| `fd_write` | `i(iiii)` | Write to file descriptor |
| `fd_read` | `i(iiii)` | Read from file descriptor |
| `fd_close` | `i(i)` | Close file descriptor |
| `fd_seek` | `i(iIi)` | Seek in file descriptor |
| `fd_tell` | `i(ii)` | Get file position |
| `fd_fdstat_get` | `i(ii)` | Get file descriptor stat |
| `path_open` | `i(iiiiiIIIi)` | Open path |
| `path_removefile` | `i(ii)` | Remove file at path |
| `proc_exit` | `v(i)` | Exit process |
| `clock_time_get` | `i(iiI)` | Get clock time |
| `random_get` | `i(ii)` | Get random bytes |
| `args_sizes_get` | `i(ii)` | Get argument sizes |
| `args_get` | `i(ii)` | Get arguments |
| `environ_sizes_get` | `i(ii)` | Get environment sizes |
| `environ_get` | `i(ii)` | Get environment variables |
