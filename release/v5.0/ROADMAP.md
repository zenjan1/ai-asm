# AI-ASM AArch64 开发规划 (v4.0)

## 当前状态 (v3.0)

v3.0 已完成多进程并发支持，包括 IPC 管道、消息队列和信号机制。

### v3.0 成果总结

| 功能 | 描述 | 状态 |
|------|------|------|
| 管道 IPC | 8个管道，4KB环形缓冲 | ✓ 完成 |
| 消息队列 | 8个队列，16x64字节消息 | ✓ 完成 |
| 信号机制 | SIGKILL/SIGTERM/SIGSTOP/SIGCONT/SIGCHLD | ✓ 完成 |
| 网络应用 | browser.wasm + net_test.wasm | ✓ 完成 |
| WASM模块 | 9个完整编译 | ✓ 完成 |

- kernel.elf: 3.7MB (29 asm + 2 C + wasm3)
- wasm_host.c: 1842行，132个host函数
- 发布包: aiasm-v1.0.tar.gz (252KB), v2.0.tar.gz (99KB), v3.0.tar.gz (99KB)
- 9 WASM模块: init, shell, test, editor, calc, paint, launcher, browser, net_test

---

## v4.0规划 (2026-07)

### 核心主题: 性能优化 + 高级系统服务

### 性能优化

| 任务 | 描述 | 优先级 |
|------|------|--------|
| WASM JIT缓存 | 预编译热点模块，减少解释开销 | P0 |
| 双缓冲GUI | 前后帧缓冲切换，消除画面撕裂 | P0 |
| 中断驱动I/O | virtio-net/virtio-blk中断替代轮询 | P0 |
| 内存池管理 | buddy allocator替代bump分配器 | P1 |
| 进程创建优化 | PCB预分配，快速上下文切换 | P1 |

### 高级系统服务

| 模块 | 描述 | 优先级 |
|------|------|--------|
| proc_monitor.wasm | 进程监控、崩溃自动重启、资源统计 | P1 |
| syslog.wasm | 结构化系统日志、过滤、持久化 | P1 |
| devmgr.wasm | 设备热插拔管理、驱动注册 | P2 |
| user.wasm | 多用户认证、权限管理 | P2 |

### v4.0 新增 WASM 模块

| 模块 | 预计大小 | 功能 |
|------|----------|------|
| proc_monitor.wasm | ~4KB | 进程监控+重启 |
| syslog.wasm | ~3KB | 系统日志服务 |
| devmgr.wasm | ~3KB | 设备管理 |

### 性能目标

| 指标 | 当前 | v4.0目标 |
|------|------|----------|
| 启动时间 | ~3秒 | <1秒 |
| WASM加载 | ~200ms | <100ms |
| GUI刷新 | ~10fps | 30fps |
| 内存使用 | ~64MB | <50MB |

---

## 历史版本

### v3.0 (2026-05-24) - IPC and Signal Mechanism
- 管道IPC + 消息队列
- POSIX信号机制
- 网络应用框架

### v2.0 (2026-05-24) - Application Ecosystem
- 7个WASM应用模块
- 应用启动器
- GUI窗口管理器

### v1.0 (2026-05-24) - WASM-native OS
- 完整三层架构
- wasm3运行时
- FAT32文件系统
- TCP/IP网络栈
- WASI系统调用

### v0.1 ~ v0.9 - 内核开发阶段
- 中断系统、抢占式调度
- MMU虚拟内存
- VirtIO设备驱动

---

## 技术约束与设计原则

1. **内核保持最小化**: 核心功能用汇编，复杂逻辑放WASM
2. **安全第一**: 进程隔离、权限检查、资源配额
3. **渐进式开发**: 每个版本可运行可测试
4. **QEMU验证**: 所有功能先在QEMU virt平台验证
5. **真实硬件兼容**: 设计考虑未来移植到真实ARM设备
6. **WASM为中心**: 应用层和大部分服务用WASM实现
7. **性能不妥协**: 优化不影响功能完整性

---

## 参考资源

- AArch64 Architecture Reference Manual
- ARM Generic Interrupt Controller Specification
- wasm3: https://github.com/aspect/wasm3
- WASI: https://wasi.dev/
- VirtIO Specification
- lwIP TCP/IP Stack (参考实现)
