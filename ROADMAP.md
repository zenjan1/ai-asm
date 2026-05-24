# AI-ASM AArch64 开发规划 (v7.0)

## 当前状态 (v6.0)

v6.0 安全增强 + 设备管理已完成。

### v6.0 成果总结

| 功能 | 描述 | 状态 |
|------|------|------|
| user.wasm | 多用户认证、GUI 登录界面 | ✓ 完成 |
| devmgr.wasm | 设备热插拔管理、驱动注册 | ✓ 完成 |
| 进程隔离 | WASM 模块间内存保护 | ✓ 完成 |
| 权限系统 | root/admin/user/guest 四级 | ✓ 完成 |

- WASM 模块: 15 个
- kernel.elf: 6712KB (6873280 bytes)
- 新增内核: perm.asm, proc_mem.asm, device.asm, user.asm
- wasm_host.c: 140+ 个 host 函数

---

## v7.0 规划 (2026-09)

### 核心主题: 性能优化 + 真实硬件移植准备

### 性能目标

| 模块 | 描述 | 优先级 |
|------|------|--------|
| JIT 预编译 | 热点函数预编译为 AArch64 原生码 | P0 |
| WASM 加载优化 | 并行加载 + 缓存 | P0 |
| 内存优化 | 进程级内存池 | P1 |
| GUI 加速 | 硬件加速合成 | P1 |

### 真实硬件移植

| 任务 | 描述 | 优先级 |
|------|------|--------|
| 板级适配 | Raspberry Pi 4 / Rockchip RK3588 | P0 |
| 真实驱动 | MMC/SD 存储, RTL8211F 以太网 | P0 |
| 启动加载器 | U-Boot 集成 | P1 |

### 安全增强

| 任务 | 描述 | 优先级 |
|------|------|--------|
| 资源配额 | CPU 时间/内存上限 | P1 |
| 安全审计 | 内核事件日志审计 | P1 |

### v7.0 性能验证

| 指标 | v4.0 目标 | v6.0 实测 | v7.0 目标 |
|------|----------|----------|----------|
| 启动时间 | <1 秒 | [待测] | <0.5 秒 |
| WASM 加载 | <100ms | [待测] | <50ms |
| GUI 刷新 | 30fps | [待测] | 60fps |
| 内存使用 | <50MB | [待测] | <40MB |

---

## 历史版本

### v6.0 (2026-05-24) - Security & Device Management
- user.wasm 多用户认证
- devmgr.wasm 设备热插拔
- 进程内存隔离 (每进程 16MB)
- root/admin/user/guest 四级权限

### v5.0 (2026-05-24) - Application Ecosystem
- 13 个 WASM 应用模块
- filemgr.wasm + settings.wasm
- launcher 7-app grid

### v4.0 (2026-05-24) - Performance & Services
- JIT 缓存 + Buddy 分配 + 中断 I/O + 双缓冲
- proc_monitor + syslog 系统服务

### v3.0 (2026-05-24) - IPC and Signal Mechanism
- 管道 IPC + 消息队列
- POSIX 信号机制

### v2.0 (2026-05-24) - Application Ecosystem
- 7 个 WASM 应用模块
- GUI 窗口管理器

### v1.0 (2026-05-24) - WASM-native OS
- 完整三层架构
- wasm3 运行时
- FAT32 文件系统 + TCP/IP 网络栈

### v0.1 ~ v0.9 - 内核开发阶段
- 中断系统、抢占式调度
- MMU 虚拟内存
- VirtIO 设备驱动

---

## 技术约束与设计原则

1. **内核保持最小化**: 核心功能用汇编，复杂逻辑放 WASM
2. **安全第一**: 进程隔离、权限检查、资源配额
3. **渐进式开发**: 每个版本可运行可测试
4. **QEMU 验证**: 所有功能先在 QEMU virt 平台验证
5. **真实硬件兼容**: 设计考虑未来移植到真实 ARM 设备
6. **WASM 为中心**: 应用层和大部分服务用 WASM 实现
7. **性能不妥协**: 优化不影响功能完整性

---

## 参考资源

- AArch64 Architecture Reference Manual
- ARM Generic Interrupt Controller Specification
- wasm3: https://github.com/aspect/wasm3
- WASI: https://wasi.dev/
- VirtIO Specification
- lwIP TCP/IP Stack (参考实现)
