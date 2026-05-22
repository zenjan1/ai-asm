# AI-ASM AArch64 v0.1

100% pure AArch64 assembly AI-native OS kernel for QEMU virt on Termux.

## 前置环境

```bash
pkg update && pkg upgrade -y
pkg install binutils qemu-system-aarch64 make git
```

## 快速开始

```bash
make && make run
```

## 目录结构

```
aiasm-aarch64/
├── kernel/
│   ├── kernel.asm      # 内核入口、初始化
│   ├── pl011.asm       # PL011 串口驱动（轮询）
│   ├── log.asm         # 结构化 JSON 日志
│   ├── event.asm       # 内核事件总线（环形缓冲）
│   ├── utils.asm       # memset/memcpy/strlen/itoa_buf/strcmp/strncmp
│   ├── shell.asm       # 串口交互式 Shell
│   └── linker.ld       # 链接脚本（入口 0x40080000）
├── Makefile
├── run_kernel.sh
└── README.md
```

## Shell 命令

| 命令 | 输出 |
|------|------|
| `help` | `{"commands":["help","version","log 0\|1\|2\|3","events","reboot","shutdown"]}` |
| `version` | `{"version":"0.1-aarch64","arch":"aarch64","build":"pure-asm"}` |
| `log 0~3` | `{"level_set":true}` |
| `events` | 事件环形缓冲 JSON 数组 |
| `reboot` | `{"action":"reboot"}` 后停机 |
| `shutdown` | `{"action":"shutdown"}` 后停机 |

## 日志格式

```json
{"ts":1,"level":"INFO","event":"event","data":{"version":"0.1-aarch64","arch":"aarch64"}}
```

## 技术规格

- 加载地址：0x40080000
- 串口：PL011 @ 0x09000000，115200 8N1，轮询
- 内核栈：8KB
- 事件缓冲：16 条目环形缓冲
- 无 C 语言、无外部依赖、无 GIC、无 MMU
