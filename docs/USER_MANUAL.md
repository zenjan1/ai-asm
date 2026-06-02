# AI-ASM OS User Manual (v28.0)

## Quick Start Guide

### Prerequisites

- QEMU system AArch64 emulator, OR
- Raspberry Pi 4 (4GB+ RAM recommended)

### Running on QEMU

```bash
# Clone and build
git clone https://github.com/zenjan1/ai-asm.git
cd ai-asm
make qemu

# Or run directly
qemu-system-aarch64 -M virt -cpu cortex-a72 -m 2G \
  -kernel kernel/bootloader.bin \
  -drive file=build/rootfs.img,format=raw \
  -nographic -serial mon:stdio
```

### Running on Raspberry Pi 4

```bash
# Flash SD card
sudo dd if=build/rpi4.img of=/dev/sdX bs=4M status=progress

# Insert SD card, connect UART, power on
```

### First Boot

```
[    0] AI-ASM OS v28.0 booting...
[   12] Kernel initialized (62 modules)
[   45] WASM runtime ready (372 host functions)
[   89] Mounting filesystem...
[  102] Starting services...
[  120] Welcome to AI-ASM OS v28.0
AI-ASM OS v28.0> _
```

## Command Reference

### POSIX Commands

```
ls [-la] [path]          List directory contents
cat <file>               Display file contents
echo <text>              Print text
wc [-lwc] <file>         Count lines/words/chars
head [-n N] <file>       Show first N lines
tail [-n N] <file>       Show last N lines
grep <pattern> <file>    Search for pattern
sort <file>              Sort lines
uniq <file>              Remove duplicate lines
tr <set1> <set2>         Translate characters
cut [-d delim] <file>    Cut columns
awk <script> <file>      Pattern processing
sed <script> <file>      Stream editor
date                     Display date/time
sleep <ms>               Sleep milliseconds
chmod <mode> <file>      Change permissions
chown <uid> <file>       Change ownership
df                       Show disk usage
free                     Show memory usage
ps                       List processes
kill <pid>               Kill process
```

### Network Commands

```
wget <url>               Download file
ping <host>              Ping host
nc <host> <port>         Netcat connect
curl <url>               HTTP request
dig <domain>             DNS lookup
```

### WASM Module Commands

```
wasm run <module>        Run WASM module
wasm list                List available modules
wasm status              Show module status
wasm stop <id>           Stop module
wasm reload <id>         Reload module
```

### Agent Commands

```
agent create <name>      Create agent
agent send <id> <msg>    Send to agent
agent recv <id>          Receive from agent
agent list               List agents
agent destroy <id>       Destroy agent
```

### AI Commands

```
ai query <prompt>        Query AI model
ai status                Show AI status
ai models                List AI models
```

## System Configuration

### Environment Variables

```
PATH=/bin                Command search path
HOME=/                   Home directory
SHELL=shell              Default shell
```

### File System Layout

```
/
├── bin/                 WASM modules (POSIX tools)
├── etc/                 Configuration files
├── tmp/                 Temporary files
├── var/
│   └── log/             System logs
├── dev/                 Device files
└── home/                User directories
```

## Security

### Permission Levels

| Level | Name | Capabilities |
|-------|------|-------------|
| 0 | Root | Full system access |
| 1 | Admin | All except delete |
| 2 | User | Read/write, no delete/create |
| 3 | Guest | Read only |

### User Authentication

```
user add <name>          Add user
user login <name>        Login
user logout              Logout
user list                List users
```

## Troubleshooting

### Common Issues

**Module not found**: Use `wasm list` to check available modules.

**Permission denied**: Check permission level with `perm_get_level`.

**Out of memory**: Use `free` to check memory usage, `ps` for process list.

### Log Access

```
log read                 Read kernel log
log size                 Get log size
```
