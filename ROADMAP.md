# AI-ASM AArch64 开发规划 (v6.0)

## 当前状态 (v5.0)

v5.0 已完成完整应用生态，包括 13 个 WASM 模块。

### v5.0 成果总结

| 功能 | 描述 | 状态 |
|------|------|------|
| 应用生态 | 13 个 WASM 应用模块 | ✓ 完成 |
| GUI 框架 | 双缓冲 +30fps | ✓ 完成 |
| 性能优化 | JIT 缓存+Buddy 分配 | ✓ 完成 |
| 系统服务 | proc_monitor+syslog | ✓ 完成 |
| 文件管理 | filemgr.wasm RAM disk 浏览 | ✓ 完成 |
| 系统设置 | settings.wasm 系统信息查询 | ✓ 完成 |

- kernel.elf: 6708KB (6869744 bytes)
- wasm_host.c: 132+ 个 host 函数
- 发布包: aiasm-v5.0.tar.gz (108KB)
- Git 标签: v1.0 ~ v5.0

---

## v6.0 规划 (2026-08)

### 核心主题: 安全增强 + 设备管理

### 安全增强

| 模块 | 描述 | 优先级 |
|------|------|--------|
| user.wasm | 多用户认证、权限管理、用户隔离 | P0 |
| 进程隔离 | WASM 模块间内存保护、权限边界 | P0 |
| 资源配额 | 单模块 CPU 时间、内存上限 | P1 |
| 安全审计 | 内核事件日志审计、异常检测 | P1 |

### 设备管理

| 模块 | 描述 | 优先级 |
|------|------|--------|
| devmgr.wasm | 设备热插拔管理、驱动注册 | P0 |
| USB 支持 | VirtIO-USB / EHCI 模拟 | P1 |
| 输入设备 | 键盘/鼠标/触摸板统一抽象 | P1 |
| 存储扩展 | 多 virtio-blk 设备支持 | P2 |

### v6.0 新增 WASM 模块

| 模块 | 预计大小 | 功能 |
|------|----------|------|
| user.wasm | ~5KB | 多用户认证+权限 |
| devmgr.wasm | ~4KB | 设备管理+热插拔 |

### v6.0 性能验证

| 指标 | v4.0 目标 | v6.0 实测 |
|------|----------|----------|
| 启动时间 | <1 秒 | [待测] |
| WASM 加载 | <100ms | [待测] |
| GUI 刷新 | 30fps | [待测] |
| 内存使用 | <50MB | [待测] |

---

## 历史版本

### v5.0 (2026-05-24) - Application Ecosystem
- 13 个 WASM 应用模块
- filemgr.wasm + settings.wasm 新增
- launcher 7-app grid
- v5.0 发布包 (108KB)

### v4.0 (2026-05-24) - Performance & Services
- JIT 缓存 + Buddy 分配 + 中断 I/O + 双缓冲
- proc_monitor + syslog 系统服务

### v3.0 (2026-05-24) - IPC and Signal Mechanism
- 管道 IPC + 消息队列
- POSIX 信号机制
- 网络应用框架

### v2.0 (2026-05-24) - Application Ecosystem
- 7 个 WASM 应用模块
- 应用启动器
- GUI 窗口管理器

### v1.0 (2026-05-24) - WASM-native OS
- 完整三层架构
- wasm3 运行时
- FAT32 文件系统
- TCP/IP 网络栈
- WASI 系统调用

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
