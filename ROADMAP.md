# AI-ASM AArch64 开发规划 (v3.0)

## 当前状态 (v2.0)

v2.0 已完成应用生态系统，包括 7 个 WASM 模块和应用启动器。

### v2.0 成果总结

| 模块 | 大小 | 功能 | 状态 |
|------|------|------|------|
| init.wasm | 1332B | 系统初始化，启动launcher | ✓ 完成 |
| shell.wasm | 3553B | 交互式命令行 | ✓ 完成 |
| editor.wasm | 4949B | 文本编辑器 | ✓ 完成 |
| calc.wasm | 4008B | 计算器 | ✓ 完成 |
| paint.wasm | 3316B | 画图工具 | ✓ 完成 |
| launcher.wasm | 3995B | 应用启动器 | ✓ 完成 |
| test.wasm | 4399B | 集成测试 | ✓ 完成 |

- kernel.elf: 3.65MB
- 发布包: aiasm-v2.0.tar.gz (96KB)
- GUI: 800x600 窗口管理器，16窗口合成
- WASM运行时: wasm3, MAX_MODULES=16, 27个host函数

---

## v3.0规划 (2026-06)

### 核心主题: 多进程并发 + 网络应用

### 网络应用

| 任务 | 描述 | 优先级 |
|------|------|--------|
| browser.wasm | HTTP GET/POST客户端，简单HTML文本渲染 | P0 |
| ftp.wasm | FTP文件传输客户端 | P1 |
| net_test.wasm | TCP连接验证，ARP/DNS测试 | P0 |
| http_server.wasm | 简单HTTP服务器，提供系统信息 | P2 |

### 多进程增强

| 任务 | 描述 | 优先级 |
|------|------|--------|
| 进程间通信 | 管道、消息队列 | P0 |
| 信号机制 | SIGKILL/SIGTERM处理 | P0 |
| 资源配额 | CPU时间限制、内存限制 | P1 |
| 终端多窗口 | 多个shell进程并发运行 | P1 |

### 文件管理器

| 任务 | 描述 | 优先级 |
|------|------|--------|
| file_manager.wasm | 目录浏览、文件创建/删除/重命名 | P0 |
| 文件查看器 | 文本文件浏览、hex查看 | P1 |

### 性能优化

| 任务 | 描述 | 优先级 |
|------|------|--------|
| WASM JIT预编译 | 热点函数预编译加速 | P2 |
| framebuffer双缓冲 | 消除画面撕裂 | P1 |
| 中断驱动网络I/O | 替代轮询模式 | P0 |

### v3.0 新增 WASM 模块

| 模块 | 预计大小 | 功能 |
|------|----------|------|
| browser.wasm | ~5KB | HTTP客户端 |
| file_manager.wasm | ~4KB | 目录浏览 |
| net_test.wasm | ~3KB | 网络测试 |

---

## 历史版本

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
- GUI窗口管理器

### v0.1 ~ v0.9 - 内核开发阶段
- 中断系统、抢占式调度
- MMU虚拟内存
- VirtIO设备驱动
- 文件系统、网络栈

---

## 技术约束与设计原则

1. **内核保持最小化**: 核心功能用汇编，复杂逻辑放WASM
2. **安全第一**: 进程隔离、权限检查、资源配额
3. **渐进式开发**: 每个版本可运行可测试
4. **QEMU验证**: 所有功能先在QEMU virt平台验证
5. **真实硬件兼容**: 设计考虑未来移植到真实ARM设备
6. **WASM为中心**: 应用层和大部分服务用WASM实现

---

## 参考资源

- AArch64 Architecture Reference Manual
- ARM Generic Interrupt Controller Specification
- wasm3: https://github.com/aspect/wasm3
- WASI: https://wasi.dev/
- VirtIO Specification
- lwIP TCP/IP Stack (参考实现)
