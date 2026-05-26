# AI-ASM AArch64 v0.5 — Makefile for Termux
# Toolchain: native binutils (as, ld), clang for C/WASM
#
# Targets:
#   make all        — build full kernel.elf
#   make asm        — compile assembly only
#   make wasm3      — compile wasm3 library only
#   make modules    — compile WASM modules only
#   make kernel     — link kernel.elf only
#   make run        — build and launch in QEMU
#   make clean      — remove all build artifacts
#
# Incremental: only rebuilds changed objects via Make dependency tracking.

AS      = as
LD      = ld
CLANG   = clang
OBJCOPY = objcopy
QEMU    = qemu-system-aarch64

ASFLAGS = -march=armv8-a -g
LDFLAGS = -T kernel/linker.ld -nostdlib -N
CFLAGS_FREESTANDING = -c -O2 -ffreestanding -Ikernel/wasm3/source \
    -mgeneral-regs-only \
    -Dd_m3FixedHeap=1048576 -Dd_m3VerboseErrorMessages=0 \
    -Dd_m3HasFloat=0 -Dd_m3NoFloatDynamic=1 \
    -Dd_m3HasTracer=0 -Dd_m3RecordBacktraces=0 -Dd_m3LogTimestamps=0
WASM_FLAGS = --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
    -Wl,--no-entry -Wl,--export=shell_entry

KERNEL_DIR = kernel
BUILD_DIR  = build
WASM3_DIR  = $(KERNEL_DIR)/wasm3
MODULES_DIR = modules
TARGET_ELF = $(BUILD_DIR)/kernel.elf

# ---------------------------------------------------------------------------
# Assembly sources (auto-detected from kernel/*.asm)
# ---------------------------------------------------------------------------
ASM_SRCS = $(KERNEL_DIR)/kernel.asm \
           $(KERNEL_DIR)/pl011.asm \
           $(KERNEL_DIR)/log.asm \
           $(KERNEL_DIR)/event.asm \
           $(KERNEL_DIR)/utils.asm \
           $(KERNEL_DIR)/memory.asm \
           $(KERNEL_DIR)/timer.asm \
           $(KERNEL_DIR)/gic.asm \
           $(KERNEL_DIR)/exceptions.asm \
           $(KERNEL_DIR)/process.asm \
           $(KERNEL_DIR)/mmu.asm \
           $(KERNEL_DIR)/virtio.asm \
           $(KERNEL_DIR)/virtio_blk.asm \
           $(KERNEL_DIR)/virtio_net.asm \
           $(KERNEL_DIR)/fs.asm \
           $(KERNEL_DIR)/net.asm \
           $(KERNEL_DIR)/wasi.asm \
           $(KERNEL_DIR)/module.asm \
           $(KERNEL_DIR)/virtio_gpu.asm \
           $(KERNEL_DIR)/fb.asm \
           $(KERNEL_DIR)/gui.asm \
           $(KERNEL_DIR)/serial_rx.asm \
           $(KERNEL_DIR)/wasm_embed.asm \
           $(KERNEL_DIR)/ramdisk.asm \
           $(KERNEL_DIR)/ipc.asm \
           $(KERNEL_DIR)/msg.asm \
           $(KERNEL_DIR)/signal.asm \
           $(KERNEL_DIR)/jit_cache.asm \
           $(KERNEL_DIR)/buddy.asm \
           $(KERNEL_DIR)/virtio_irq.asm \
           $(KERNEL_DIR)/user.asm \
           $(KERNEL_DIR)/device.asm \
           $(KERNEL_DIR)/proc_mem.asm \
           $(KERNEL_DIR)/perm.asm \
           $(KERNEL_DIR)/jit_codegen.asm \
           $(KERNEL_DIR)/module_cache.asm \
           $(KERNEL_DIR)/quota.asm \
           $(KERNEL_DIR)/audit.asm \
           $(KERNEL_DIR)/shmem.asm \
           $(KERNEL_DIR)/semaphore.asm \
           $(KERNEL_DIR)/event_notify.asm \
           $(KERNEL_DIR)/vfs.asm \
           $(KERNEL_DIR)/vfs_ops.asm \
           $(KERNEL_DIR)/vfs_path.asm \
           $(KERNEL_DIR)/persist.asm
ASM_OBJS = $(patsubst $(KERNEL_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))

# C sources (freestanding)
C_SRCS = $(KERNEL_DIR)/libc_shim.c \
         $(KERNEL_DIR)/wasm_host.c

C_OBJS = $(BUILD_DIR)/libc_shim.o $(BUILD_DIR)/wasm_host.o

# wasm3 sources (exclude tracer/wasi/debug files we don't need)
WASM3_SRCS = $(WASM3_DIR)/source/m3_bind.c \
             $(WASM3_DIR)/source/m3_code.c \
             $(WASM3_DIR)/source/m3_compile.c \
             $(WASM3_DIR)/source/m3_core.c \
             $(WASM3_DIR)/source/m3_env.c \
             $(WASM3_DIR)/source/m3_exec.c \
             $(WASM3_DIR)/source/m3_function.c \
             $(WASM3_DIR)/source/m3_info.c \
             $(WASM3_DIR)/source/m3_module.c \
             $(WASM3_DIR)/source/m3_parse.c
WASM3_OBJS = $(patsubst $(WASM3_DIR)/source/%.c,$(BUILD_DIR)/wasm3/%.o,$(WASM3_SRCS))
WASM3_LIB  = $(BUILD_DIR)/libwasm3.a

# WASM modules
INIT_WASM   = $(MODULES_DIR)/init/init.wasm
SHELL_WASM  = $(MODULES_DIR)/shell/shell.wasm
TEST_WASM   = $(MODULES_DIR)/test/test.wasm
EDITOR_WASM = $(MODULES_DIR)/editor/editor.wasm
CALC_WASM   = $(MODULES_DIR)/calc/calc.wasm
PAINT_WASM  = $(MODULES_DIR)/paint/paint.wasm
LAUNCHER_WASM = $(MODULES_DIR)/launcher/launcher.wasm
NET_TEST_WASM = $(MODULES_DIR)/net_test/net_test.wasm
PROC_MONITOR_WASM = $(MODULES_DIR)/proc_monitor/proc_monitor.wasm
SYSLOG_WASM = $(MODULES_DIR)/syslog/syslog.wasm
FILEMGR_WASM = $(MODULES_DIR)/filemgr/filemgr.wasm
SETTINGS_WASM = $(MODULES_DIR)/settings/settings.wasm
USER_WASM     = $(MODULES_DIR)/user/user.wasm
DEVMGR_WASM   = $(MODULES_DIR)/devmgr/devmgr.wasm
BROWSER_WASM  = $(MODULES_DIR)/browser/browser.wasm
HTTPD_WASM    = $(MODULES_DIR)/httpd/httpd.wasm
DNS_RESOLVER_WASM = $(MODULES_DIR)/dns_resolver/dns_resolver.wasm
SHMEM_TEST_WASM = $(MODULES_DIR)/shmem_test/shmem_test.wasm
SEM_TEST_WASM   = $(MODULES_DIR)/sem_test/sem_test.wasm
GREP_WASM       = $(MODULES_DIR)/grep/grep.wasm
CAT_WASM        = $(MODULES_DIR)/cat/cat.wasm
ECHO_WASM       = $(MODULES_DIR)/echo/echo.wasm
TEE_WASM        = $(MODULES_DIR)/tee/tee.wasm
WC_WASM         = $(MODULES_DIR)/wc/wc.wasm
HEAD_WASM       = $(MODULES_DIR)/head/head.wasm
TAIL_WASM       = $(MODULES_DIR)/tail/tail.wasm
SORT_WASM       = $(MODULES_DIR)/sort/sort.wasm
UNIQ_WASM       = $(MODULES_DIR)/uniq/uniq.wasm
TR_WASM         = $(MODULES_DIR)/tr/tr.wasm
CUT_WASM        = $(MODULES_DIR)/cut/cut.wasm
SED_WASM        = $(MODULES_DIR)/sed/sed.wasm
XARGS_WASM      = $(MODULES_DIR)/xargs/xargs.wasm
DATE_WASM       = $(MODULES_DIR)/date/date.wasm
AWK_WASM        = $(MODULES_DIR)/awk/awk.wasm
LS_WASM         = $(MODULES_DIR)/ls/ls.wasm
PWD_WASM        = $(MODULES_DIR)/pwd/pwd.wasm
ENV_WASM        = $(MODULES_DIR)/env/env.wasm
PRINTF_WASM     = $(MODULES_DIR)/printf/printf.wasm
SLEEP_WASM      = $(MODULES_DIR)/sleep/sleep.wasm
YES_WASM        = $(MODULES_DIR)/yes/yes.wasm
REV_WASM        = $(MODULES_DIR)/rev/rev.wasm
CMP_WASM        = $(MODULES_DIR)/cmp/cmp.wasm
DIFF_WASM       = $(MODULES_DIR)/diff/diff.wasm
BASENAME_WASM   = $(MODULES_DIR)/basename/basename.wasm
DIRNAME_WASM    = $(MODULES_DIR)/dirname/dirname.wasm
NL_WASM         = $(MODULES_DIR)/nl/nl.wasm
SHUF_WASM       = $(MODULES_DIR)/shuf/shuf.wasm
COMM_WASM       = $(MODULES_DIR)/comm/comm.wasm
PASTE_WASM      = $(MODULES_DIR)/paste/paste.wasm
FOLD_WASM       = $(MODULES_DIR)/fold/fold.wasm
TAC_WASM        = $(MODULES_DIR)/tac/tac.wasm
CKSUM_WASM      = $(MODULES_DIR)/cksum/cksum.wasm
SUM_WASM        = $(MODULES_DIR)/sum/sum.wasm
TOUCH_WASM      = $(MODULES_DIR)/touch/touch.wasm
TRUE_WASM       = $(MODULES_DIR)/true/true.wasm
FALSE_WASM      = $(MODULES_DIR)/false/false.wasm
ID_WASM         = $(MODULES_DIR)/id/id.wasm
UPTIME_WASM    = $(MODULES_DIR)/uptime/uptime.wasm
WHOAMI_WASM     = $(MODULES_DIR)/whoami/whoami.wasm
HOSTNAME_WASM   = $(MODULES_DIR)/hostname/hostname.wasm
UNAME_WASM      = $(MODULES_DIR)/uname/uname.wasm
DF_WASM         = $(MODULES_DIR)/df/df.wasm
FREE_WASM       = $(MODULES_DIR)/free/free.wasm
PS_WASM         = $(MODULES_DIR)/ps/ps.wasm
KILL_WASM       = $(MODULES_DIR)/kill/kill.wasm
MKDIR_WASM      = $(MODULES_DIR)/mkdir/mkdir.wasm
RMDIR_WASM      = $(MODULES_DIR)/rmdir/rmdir.wasm
LN_WASM         = $(MODULES_DIR)/ln/ln.wasm
RM_WASM         = $(MODULES_DIR)/rm/rm.wasm
CP_WASM         = $(MODULES_DIR)/cp/cp.wasm
MV_WASM         = $(MODULES_DIR)/mv/mv.wasm
CHMOD_WASM      = $(MODULES_DIR)/chmod/chmod.wasm
TTY_WASM        = $(MODULES_DIR)/tty/tty.wasm
WHO_WASM        = $(MODULES_DIR)/who/who.wasm
STTY_WASM       = $(MODULES_DIR)/stty/stty.wasm
TPUT_WASM       = $(MODULES_DIR)/tput/tput.wasm
READLINK_WASM   = $(MODULES_DIR)/readlink/readlink.wasm
CAL_WASM        = $(MODULES_DIR)/cal/cal.wasm
FIND_WASM       = $(MODULES_DIR)/find/find.wasm
DU_WASM         = $(MODULES_DIR)/du/du.wasm
MOUNT_WASM      = $(MODULES_DIR)/mount/mount.wasm
UMOUNT_WASM     = $(MODULES_DIR)/umount/umount.wasm
IFCONFIG_WASM   = $(MODULES_DIR)/ifconfig/ifconfig.wasm
PING_WASM       = $(MODULES_DIR)/ping/ping.wasm
NETSTAT_WASM    = $(MODULES_DIR)/netstat/netstat.wasm
ROUTE_WASM      = $(MODULES_DIR)/route/route.wasm
TRACEROUTE_WASM = $(MODULES_DIR)/traceroute/traceroute.wasm
NSLOOKUP_WASM   = $(MODULES_DIR)/nslookup/nslookup.wasm
WGET_WASM       = $(MODULES_DIR)/wget/wget.wasm
CURL_WASM       = $(MODULES_DIR)/curl/curl.wasm
SSH_WASM        = $(MODULES_DIR)/ssh/ssh.wasm
SCP_WASM        = $(MODULES_DIR)/scp/scp.wasm
TAR_WASM        = $(MODULES_DIR)/tar/tar.wasm
GZIP_WASM       = $(MODULES_DIR)/gzip/gzip.wasm $(MODULES_DIR)/gunzip/gunzip.wasm $(MODULES_DIR)/zcat/zcat.wasm $(MODULES_DIR)/bzip2/bzip2.wasm $(MODULES_DIR)/bunzip2/bunzip2.wasm $(MODULES_DIR)/lzma/lzma.wasm $(MODULES_DIR)/unlzma/unlzma.wasm $(MODULES_DIR)/xz/xz.wasm $(MODULES_DIR)/column/column.wasm $(MODULES_DIR)/expand/expand.wasm $(MODULES_DIR)/unexpand/unexpand.wasm $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
GUNZIP_WASM     = $(MODULES_DIR)/gunzip/gunzip.wasm
ZCAT_WASM       = $(MODULES_DIR)/zcat/zcat.wasm
BZIP2_WASM      = $(MODULES_DIR)/bzip2/bzip2.wasm
BUNZIP2_WASM    = $(MODULES_DIR)/bunzip2/bunzip2.wasm
LZMA_WASM       = $(MODULES_DIR)/lzma/lzma.wasm
UNLZMA_WASM     = $(MODULES_DIR)/unlzma/unlzma.wasm
XZ_WASM         = $(MODULES_DIR)/xz/xz.wasm $(MODULES_DIR)/column/column.wasm $(MODULES_DIR)/expand/expand.wasm $(MODULES_DIR)/unexpand/unexpand.wasm $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
UNXZ_WASM       = $(MODULES_DIR)/unxz/unxz.wasm
SHA256SUM_WASM  = $(MODULES_DIR)/sha256sum/sha256sum.wasm
MD5SUM_WASM     = $(MODULES_DIR)/md5sum/md5sum.wasm
BASE64_WASM     = $(MODULES_DIR)/base64/base64.wasm
BASE64D_WASM    = $(MODULES_DIR)/base64d/base64d.wasm
OD_WASM         = $(MODULES_DIR)/od/od.wasm
HEXDUMP_WASM    = $(MODULES_DIR)/hexdump/hexdump.wasm
STRINGS_WASM    = $(MODULES_DIR)/strings/strings.wasm
FILETYPE_WASM   = $(MODULES_DIR)/file/file.wasm
XXD_WASM        = $(MODULES_DIR)/xxd/xxd.wasm
LESS_WASM       = $(MODULES_DIR)/less/less.wasm
MORE_WASM       = $(MODULES_DIR)/more/more.wasm
JOIN_WASM       = $(MODULES_DIR)/join/join.wasm
SPLIT_WASM      = $(MODULES_DIR)/split/split.wasm
PATCH_WASM      = $(MODULES_DIR)/patch/patch.wasm
DIFF3_WASM      = $(MODULES_DIR)/diff3/diff3.wasm
TSORT_WASM      = $(MODULES_DIR)/tsort/tsort.wasm
CSPLIT_WASM     = $(MODULES_DIR)/csplit/csplit.wasm
FMT_WASM        = $(MODULES_DIR)/fmt/fmt.wasm
PR_WASM         = $(MODULES_DIR)/pr/pr.wasm
COLUMN_WASM     = $(MODULES_DIR)/column/column.wasm $(MODULES_DIR)/expand/expand.wasm $(MODULES_DIR)/unexpand/unexpand.wasm $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
EXPAND_WASM      = $(MODULES_DIR)/expand/expand.wasm $(MODULES_DIR)/unexpand/unexpand.wasm $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
UNEXPAND_WASM    = $(MODULES_DIR)/unexpand/unexpand.wasm $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
NUMFMT_WASM      = $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
NPROC_WASM       = $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
HOSTID_WASM      = $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
SYNC_WASM        = $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
LINK_WASM        = $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
UNLINK_WASM      = $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
LOGNAME_WASM     = $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
ARCH_WASM        = $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
SETARCH_WASM     = $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
NICE_WASM        = $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
RENICE_WASM      = $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
TIMEOUT_WASM     = $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
STDBUF_WASM      = $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
FACTOR_WASM       = $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
SEQ_WASM          = $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
REALPATH_WASM     = $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
GROUPS_WASM       = $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
INSTALL_WASM       = $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
PATHCHK_WASM      = $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
MKTEMP_WASM       = $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
TRUNCATE_WASM      = $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
NOHUP_WASM         = $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
ENVSUBST_WASM     = $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
DIRCOLORS_WASM    = $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
EXPR_WASM         = $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
TEST_CMD_WASM      = $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
DD_WASM           = $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
HASHSUM_WASM      = $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
WATCH_WASM        = $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
WASM_DIS_WASM      = $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
RAMDISK_TAR = $(KERNEL_DIR)/ramdisk.tar

# All object dependencies
ALL_OBJS = $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)

# ---------------------------------------------------------------------------
# Phony targets
# ---------------------------------------------------------------------------
.PHONY: all asm wasm3 init-wasm shell-wasm ramdisk kernel run clean

all: $(TARGET_ELF)

$(BUILD_DIR) $(BUILD_DIR)/wasm3:
	mkdir -p $@

# ---------------------------------------------------------------------------
# Layered build targets
# ---------------------------------------------------------------------------

# Compile assembly only
asm: $(ASM_OBJS)
	@echo "=== ASM compilation complete ==="

# Compile wasm3 library only
wasm3: $(WASM3_LIB)
	@echo "=== wasm3 library complete ==="

# Compile WASM modules and ramdisk
modules: $(INIT_WASM) $(SHELL_WASM) $(TEST_WASM) $(EDITOR_WASM) $(CALC_WASM) $(PAINT_WASM) $(LAUNCHER_WASM) $(NET_TEST_WASM) $(BROWSER_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM) $(FILEMGR_WASM) $(SETTINGS_WASM) $(USER_WASM) $(DEVMGR_WASM) $(HTTPD_WASM) $(DNS_RESOLVER_WASM) $(SHMEM_TEST_WASM) $(SEM_TEST_WASM) $(GREP_WASM) $(CAT_WASM) $(ECHO_WASM) $(TEE_WASM) $(WC_WASM) $(HEAD_WASM) $(TAIL_WASM) $(SORT_WASM) $(UNIQ_WASM) $(TR_WASM) $(CUT_WASM) $(SED_WASM) $(XARGS_WASM) $(DATE_WASM) $(AWK_WASM) $(LS_WASM) $(PWD_WASM) $(ENV_WASM) $(PRINTF_WASM) $(SLEEP_WASM) $(YES_WASM) $(REV_WASM) $(CMP_WASM) $(DIFF_WASM) $(BASENAME_WASM) $(DIRNAME_WASM) $(NL_WASM) $(SHUF_WASM) $(COMM_WASM) $(PASTE_WASM) $(FOLD_WASM) $(TAC_WASM) $(CKSUM_WASM) $(SUM_WASM) $(TOUCH_WASM) $(TRUE_WASM) $(FALSE_WASM) $(ID_WASM) $(UPTIME_WASM) $(WHOAMI_WASM) $(HOSTNAME_WASM) $(UNAME_WASM) $(DF_WASM) $(FREE_WASM) $(PS_WASM) $(KILL_WASM) $(MKDIR_WASM) $(RMDIR_WASM) $(LN_WASM) $(RM_WASM) $(CP_WASM) $(MV_WASM) $(CHMOD_WASM) $(TTY_WASM) $(WHO_WASM) $(STTY_WASM) $(TPUT_WASM) $(READLINK_WASM) $(CAL_WASM) $(FIND_WASM) $(DU_WASM) $(MOUNT_WASM) $(UMOUNT_WASM) $(IFCONFIG_WASM) $(PING_WASM) $(NETSTAT_WASM) $(ROUTE_WASM) $(TRACEROUTE_WASM) $(NSLOOKUP_WASM) $(WGET_WASM) $(CURL_WASM) $(SSH_WASM) $(SCP_WASM) $(TAR_WASM) $(GZIP_WASM) $(GUNZIP_WASM) $(ZCAT_WASM) $(BZIP2_WASM) $(BUNZIP2_WASM) $(LZMA_WASM) $(UNLZMA_WASM) $(XZ_WASM) $(UNXZ_WASM) $(SHA256SUM_WASM) $(MD5SUM_WASM) $(BASE64_WASM) $(BASE64D_WASM) $(OD_WASM) $(HEXDUMP_WASM) $(STRINGS_WASM) $(FILETYPE_WASM) $(XXD_WASM) $(LESS_WASM) $(MORE_WASM) $(JOIN_WASM) $(SPLIT_WASM) $(PATCH_WASM) $(DIFF3_WASM) $(TSORT_WASM) $(CSPLIT_WASM) $(FMT_WASM) $(PR_WASM) $(COLUMN_WASM) $(EXPAND_WASM) $(UNEXPAND_WASM) $(NUMFMT_WASM) $(NPROC_WASM) $(HOSTID_WASM) $(SYNC_WASM) $(LINK_WASM) $(UNLINK_WASM) $(LOGNAME_WASM) $(ARCH_WASM) $(SETARCH_WASM) $(NICE_WASM) $(RENICE_WASM) $(TIMEOUT_WASM) $(STDBUF_WASM) $(FACTOR_WASM) $(SEQ_WASM) $(REALPATH_WASM) $(GROUPS_WASM) $(INSTALL_WASM) $(PATHCHK_WASM) $(MKTEMP_WASM) $(TRUNCATE_WASM) $(NOHUP_WASM) $(ENVSUBST_WASM) $(DIRCOLORS_WASM) $(EXPR_WASM) $(TEST_CMD_WASM) $(DD_WASM) $(HASHSUM_WASM) $(WATCH_WASM) $(WASM_DIS_WASM) $(RAMDISK_TAR)
	@echo "=== WASM modules and ramdisk complete ==="

# Link kernel only (assumes objects exist)
kernel: $(TARGET_ELF)
	@echo "=== kernel.elf linked ==="

# ---------------------------------------------------------------------------
# Assembly compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.asm | $(BUILD_DIR)
	@echo "  AS    $<"
	$(AS) $(ASFLAGS) -o $@ $<

# wasm_embed.o depends on shell.wasm and proc_monitor.wasm and syslog.wasm being present first
$(BUILD_DIR)/wasm_embed.o: $(SHELL_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM)

# ramdisk.o depends on init.wasm, shell.wasm, test.wasm, proc_monitor.wasm, syslog.wasm, filemgr.wasm, settings.wasm, user.wasm, devmgr.wasm, httpd.wasm, grep.wasm, cat.wasm, echo.wasm and ramdisk.tar
$(BUILD_DIR)/ramdisk.o: $(INIT_WASM) $(SHELL_WASM) $(TEST_WASM) $(PROC_MONITOR_WASM) $(SYSLOG_WASM) $(FILEMGR_WASM) $(SETTINGS_WASM) $(USER_WASM) $(DEVMGR_WASM) $(HTTPD_WASM) $(DNS_RESOLVER_WASM) $(SHMEM_TEST_WASM) $(GREP_WASM) $(CAT_WASM) $(ECHO_WASM) $(TEE_WASM) $(WC_WASM) $(HEAD_WASM) $(TAIL_WASM) $(SORT_WASM) $(UNIQ_WASM) $(TR_WASM) $(CUT_WASM) $(SED_WASM) $(XARGS_WASM) $(DATE_WASM) $(AWK_WASM) $(LS_WASM) $(PWD_WASM) $(ENV_WASM) $(PRINTF_WASM) $(SLEEP_WASM) $(YES_WASM) $(REV_WASM) $(CMP_WASM) $(DIFF_WASM) $(BASENAME_WASM) $(DIRNAME_WASM) $(NL_WASM) $(SHUF_WASM) $(COMM_WASM) $(PASTE_WASM) $(FOLD_WASM) $(TAC_WASM) $(CKSUM_WASM) $(SUM_WASM) $(TOUCH_WASM) $(TRUE_WASM) $(FALSE_WASM) $(ID_WASM) $(UPTIME_WASM) $(WHOAMI_WASM) $(HOSTNAME_WASM) $(UNAME_WASM) $(DF_WASM) $(FREE_WASM) $(PS_WASM) $(KILL_WASM) $(MKDIR_WASM) $(RMDIR_WASM) $(LN_WASM) $(RM_WASM) $(CP_WASM) $(MV_WASM) $(CHMOD_WASM) $(TTY_WASM) $(WHO_WASM) $(STTY_WASM) $(TPUT_WASM) $(READLINK_WASM) $(CAL_WASM) $(FIND_WASM) $(DU_WASM) $(MOUNT_WASM) $(UMOUNT_WASM) $(IFCONFIG_WASM) $(PING_WASM) $(NETSTAT_WASM) $(ROUTE_WASM) $(TRACEROUTE_WASM) $(NSLOOKUP_WASM) $(WGET_WASM) $(CURL_WASM) $(SSH_WASM) $(SCP_WASM) $(TAR_WASM) $(GZIP_WASM) $(GUNZIP_WASM) $(ZCAT_WASM) $(BZIP2_WASM) $(BUNZIP2_WASM) $(LZMA_WASM) $(UNLZMA_WASM) $(XZ_WASM) $(UNXZ_WASM) $(SHA256SUM_WASM) $(MD5SUM_WASM) $(BASE64_WASM) $(BASE64D_WASM) $(OD_WASM) $(HEXDUMP_WASM) $(STRINGS_WASM) $(FILETYPE_WASM) $(XXD_WASM) $(LESS_WASM) $(MORE_WASM) $(JOIN_WASM) $(SPLIT_WASM) $(PATCH_WASM) $(DIFF3_WASM) $(TSORT_WASM) $(CSPLIT_WASM) $(FMT_WASM) $(PR_WASM) $(COLUMN_WASM) $(EXPAND_WASM) $(UNEXPAND_WASM) $(NUMFMT_WASM) $(NPROC_WASM) $(HOSTID_WASM) $(SYNC_WASM) $(LINK_WASM) $(UNLINK_WASM) $(LOGNAME_WASM) $(ARCH_WASM) $(SETARCH_WASM) $(NICE_WASM) $(RENICE_WASM) $(TIMEOUT_WASM) $(STDBUF_WASM) $(FACTOR_WASM) $(SEQ_WASM) $(REALPATH_WASM) $(GROUPS_WASM) $(INSTALL_WASM) $(PATHCHK_WASM) $(MKTEMP_WASM) $(TRUNCATE_WASM) $(NOHUP_WASM) $(ENVSUBST_WASM) $(DIRCOLORS_WASM) $(EXPR_WASM) $(TEST_CMD_WASM) $(DD_WASM) $(HASHSUM_WASM) $(WATCH_WASM) $(WASM_DIS_WASM) $(RAMDISK_TAR)

# ---------------------------------------------------------------------------
# C freestanding compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/libc_shim.o: $(KERNEL_DIR)/libc_shim.c | $(BUILD_DIR)
	@echo "  CC    $<"
	$(CLANG) $(CFLAGS_FREESTANDING) -o $@ $<

$(BUILD_DIR)/wasm_host.o: $(KERNEL_DIR)/wasm_host.c | $(BUILD_DIR)
	@echo "  CC    $<"
	$(CLANG) $(CFLAGS_FREESTANDING) -O0 -o $@ $<

# ---------------------------------------------------------------------------
# wasm3 compilation
# ---------------------------------------------------------------------------
$(BUILD_DIR)/wasm3/%.o: $(WASM3_DIR)/source/%.c | $(BUILD_DIR)/wasm3
	@echo "  CC    $<"
	$(CLANG) $(CFLAGS_FREESTANDING) -O2 -o $@ $<

$(WASM3_LIB): $(WASM3_OBJS)
	@echo "  AR    $@"
	ar rcs $@ $^

# ---------------------------------------------------------------------------
# WASM init module
# ---------------------------------------------------------------------------
$(INIT_WASM): $(MODULES_DIR)/init/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/init.wasm

# ---------------------------------------------------------------------------
# RAM disk (USTAR TAR from modules/init/ramdisk/)
# ---------------------------------------------------------------------------
$(RAMDISK_TAR): ramdisk/motd.txt ramdisk/hello.txt ramdisk/readme.txt
	@echo "  TAR   $@"
	tar cf $@ -C ramdisk motd.txt hello.txt readme.txt

# ---------------------------------------------------------------------------
# WASM shell module
# ---------------------------------------------------------------------------
$(SHELL_WASM): $(MODULES_DIR)/shell/src/main.c
	@echo "  WASM  $<"
	$(CLANG) $(WASM_FLAGS) -o $@ $<
	cp $@ $(KERNEL_DIR)/shell.wasm

# ---------------------------------------------------------------------------
# WASM test module (integration test)
# ---------------------------------------------------------------------------
$(TEST_WASM): $(MODULES_DIR)/test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/test.wasm

# ---------------------------------------------------------------------------
# WASM editor module
# ---------------------------------------------------------------------------
$(EDITOR_WASM): $(MODULES_DIR)/editor/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/editor.wasm

# ---------------------------------------------------------------------------
# WASM calculator module
# ---------------------------------------------------------------------------
$(CALC_WASM): $(MODULES_DIR)/calc/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/calc.wasm

# ---------------------------------------------------------------------------
# WASM paint module
# ---------------------------------------------------------------------------
$(PAINT_WASM): $(MODULES_DIR)/paint/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/paint.wasm

# ---------------------------------------------------------------------------
# WASM launcher module
# ---------------------------------------------------------------------------
$(LAUNCHER_WASM): $(MODULES_DIR)/launcher/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/launcher.wasm

# ---------------------------------------------------------------------------
# WASM net_test module
# ---------------------------------------------------------------------------
$(NET_TEST_WASM): $(MODULES_DIR)/net_test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/net_test.wasm

# ---------------------------------------------------------------------------
# WASM browser module
# ---------------------------------------------------------------------------
$(BROWSER_WASM): $(MODULES_DIR)/browser/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/browser.wasm

# WASM proc_monitor module
# ---------------------------------------------------------------------------
$(PROC_MONITOR_WASM): $(MODULES_DIR)/proc_monitor/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/proc_monitor.wasm

# ---------------------------------------------------------------------------
# WASM syslog module
# ---------------------------------------------------------------------------
$(SYSLOG_WASM): $(MODULES_DIR)/syslog/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/syslog.wasm

# ---------------------------------------------------------------------------
# WASM file manager module
# ---------------------------------------------------------------------------
$(FILEMGR_WASM): $(MODULES_DIR)/filemgr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/filemgr.wasm

# ---------------------------------------------------------------------------
# WASM settings module
# ---------------------------------------------------------------------------
$(SETTINGS_WASM): $(MODULES_DIR)/settings/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/settings.wasm

# ---------------------------------------------------------------------------
# WASM user module
# ---------------------------------------------------------------------------
$(USER_WASM): $(MODULES_DIR)/user/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/user.wasm

# ---------------------------------------------------------------------------
# WASM device manager module
# ---------------------------------------------------------------------------
$(DEVMGR_WASM): $(MODULES_DIR)/devmgr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/devmgr.wasm

# ---------------------------------------------------------------------------
# WASM httpd module (HTTP server)
# ---------------------------------------------------------------------------
$(HTTPD_WASM): $(MODULES_DIR)/httpd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/httpd.wasm

# ---------------------------------------------------------------------------
# WASM dns_resolver module (DNS resolver)
# ---------------------------------------------------------------------------
$(DNS_RESOLVER_WASM): $(MODULES_DIR)/dns_resolver/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/dns_resolver.wasm

# ---------------------------------------------------------------------------
# WASM shmem_test module (shared memory IPC test)
# ---------------------------------------------------------------------------
$(SHMEM_TEST_WASM): $(MODULES_DIR)/shmem_test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/shmem_test.wasm

# ---------------------------------------------------------------------------
# WASM sem_test module (semaphore IPC test)
# ---------------------------------------------------------------------------
$(SEM_TEST_WASM): $(MODULES_DIR)/sem_test/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sem_test.wasm

# ---------------------------------------------------------------------------
# WASM grep module (string search utility)
# ---------------------------------------------------------------------------
$(GREP_WASM): $(MODULES_DIR)/grep/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/grep.wasm

# ---------------------------------------------------------------------------
# WASM cat module (file reader)
# ---------------------------------------------------------------------------
$(CAT_WASM): $(MODULES_DIR)/cat/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cat.wasm

# ---------------------------------------------------------------------------
# WASM echo module (argument printer)
# ---------------------------------------------------------------------------
$(ECHO_WASM): $(MODULES_DIR)/echo/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/echo.wasm

# ---------------------------------------------------------------------------
# WASM tee module (stdout + file writer)
# ---------------------------------------------------------------------------
$(TEE_WASM): $(MODULES_DIR)/tee/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tee.wasm

# ---------------------------------------------------------------------------
# WASM wc module (word count)
# ---------------------------------------------------------------------------
$(WC_WASM): $(MODULES_DIR)/wc/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/wc.wasm

# ---------------------------------------------------------------------------
# WASM head module (first N lines)
# ---------------------------------------------------------------------------
$(HEAD_WASM): $(MODULES_DIR)/head/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/head.wasm

# ---------------------------------------------------------------------------
# WASM tail module (last N lines)
# ---------------------------------------------------------------------------
$(TAIL_WASM): $(MODULES_DIR)/tail/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tail.wasm

# ---------------------------------------------------------------------------
# WASM sort module (text sorting)
# ---------------------------------------------------------------------------
$(SORT_WASM): $(MODULES_DIR)/sort/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sort.wasm

# ---------------------------------------------------------------------------
# WASM uniq module (adjacent duplicate removal)
# ---------------------------------------------------------------------------
$(UNIQ_WASM): $(MODULES_DIR)/uniq/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/uniq.wasm

# ---------------------------------------------------------------------------
# WASM tr module (character translation/deletion)
# ---------------------------------------------------------------------------
$(TR_WASM): $(MODULES_DIR)/tr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tr.wasm

# ---------------------------------------------------------------------------
# WASM cut module (column extraction)
# ---------------------------------------------------------------------------
$(CUT_WASM): $(MODULES_DIR)/cut/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cut.wasm

# ---------------------------------------------------------------------------
# WASM sed module (stream editor)
# ---------------------------------------------------------------------------
$(SED_WASM): $(MODULES_DIR)/sed/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sed.wasm

# ---------------------------------------------------------------------------
# WASM xargs module (build command from stdin)
# ---------------------------------------------------------------------------
$(XARGS_WASM): $(MODULES_DIR)/xargs/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/xargs.wasm

# ---------------------------------------------------------------------------
# WASM date module (display system time)
# ---------------------------------------------------------------------------
$(DATE_WASM): $(MODULES_DIR)/date/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/date.wasm

# ---------------------------------------------------------------------------
# WASM awk module (pattern scanning and text processing)
# ---------------------------------------------------------------------------
$(AWK_WASM): $(MODULES_DIR)/awk/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/awk.wasm

# ---------------------------------------------------------------------------
# WASM ls module (list directory contents)
# ---------------------------------------------------------------------------
$(LS_WASM): $(MODULES_DIR)/ls/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ls.wasm

# ---------------------------------------------------------------------------
# WASM pwd module (print working directory)
# ---------------------------------------------------------------------------
$(PWD_WASM): $(MODULES_DIR)/pwd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/pwd.wasm

# ---------------------------------------------------------------------------
# WASM env module (environment variables)
# ---------------------------------------------------------------------------
$(ENV_WASM): $(MODULES_DIR)/env/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/env.wasm

# ---------------------------------------------------------------------------
# WASM printf module (formatted output)
# ---------------------------------------------------------------------------
$(PRINTF_WASM): $(MODULES_DIR)/printf/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/printf.wasm

# ---------------------------------------------------------------------------
# WASM sleep module (timing delay)
# ---------------------------------------------------------------------------
$(SLEEP_WASM): $(MODULES_DIR)/sleep/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sleep.wasm

# ---------------------------------------------------------------------------
# WASM yes module (repeated output)
# ---------------------------------------------------------------------------
$(YES_WASM): $(MODULES_DIR)/yes/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/yes.wasm

# ---------------------------------------------------------------------------
# WASM rev module (line reversal)
# ---------------------------------------------------------------------------
$(REV_WASM): $(MODULES_DIR)/rev/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/rev.wasm

# ---------------------------------------------------------------------------
# WASM cmp module (file comparison)
# ---------------------------------------------------------------------------
$(CMP_WASM): $(MODULES_DIR)/cmp/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cmp.wasm

# ---------------------------------------------------------------------------
# WASM diff module (line-by-line file comparison)
# ---------------------------------------------------------------------------
$(DIFF_WASM): $(MODULES_DIR)/diff/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/diff.wasm

# ---------------------------------------------------------------------------
# WASM basename module (path processing)
# ---------------------------------------------------------------------------
$(BASENAME_WASM): $(MODULES_DIR)/basename/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/basename.wasm

# ---------------------------------------------------------------------------
# WASM dirname module (directory path extraction)
# ---------------------------------------------------------------------------
$(DIRNAME_WASM): $(MODULES_DIR)/dirname/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/dirname.wasm

# ---------------------------------------------------------------------------
# WASM nl module (line numbering)
# ---------------------------------------------------------------------------
$(NL_WASM): $(MODULES_DIR)/nl/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/nl.wasm

# ---------------------------------------------------------------------------
# WASM shuf module (random line shuffling)
# ---------------------------------------------------------------------------
$(SHUF_WASM): $(MODULES_DIR)/shuf/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/shuf.wasm

# ---------------------------------------------------------------------------
# WASM comm module (compare sorted files)
# ---------------------------------------------------------------------------
$(COMM_WASM): $(MODULES_DIR)/comm/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/comm.wasm

# ---------------------------------------------------------------------------
# WASM paste module (merge lines)
# ---------------------------------------------------------------------------
$(PASTE_WASM): $(MODULES_DIR)/paste/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/paste.wasm

# ---------------------------------------------------------------------------
# WASM fold module (line wrapping)
# ---------------------------------------------------------------------------
$(FOLD_WASM): $(MODULES_DIR)/fold/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/fold.wasm

# ---------------------------------------------------------------------------
# WASM tac module (reverse file output)
# ---------------------------------------------------------------------------
$(TAC_WASM): $(MODULES_DIR)/tac/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tac.wasm

# ---------------------------------------------------------------------------
# WASM cksum module (file checksum)
# ---------------------------------------------------------------------------
$(CKSUM_WASM): $(MODULES_DIR)/cksum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cksum.wasm

# ---------------------------------------------------------------------------
# WASM sum module (simple checksum)
# ---------------------------------------------------------------------------
$(SUM_WASM): $(MODULES_DIR)/sum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sum.wasm

# ---------------------------------------------------------------------------
# WASM touch module (create empty files)
# ---------------------------------------------------------------------------
$(TOUCH_WASM): $(MODULES_DIR)/touch/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/touch.wasm

# ---------------------------------------------------------------------------
# WASM true module (always succeeds)
# ---------------------------------------------------------------------------
$(TRUE_WASM): $(MODULES_DIR)/true/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/true.wasm

# ---------------------------------------------------------------------------
# WASM false module (always fails)
# ---------------------------------------------------------------------------
$(FALSE_WASM): $(MODULES_DIR)/false/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/false.wasm

# ---------------------------------------------------------------------------
# WASM id module (user identification)
# ---------------------------------------------------------------------------
$(ID_WASM): $(MODULES_DIR)/id/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/id.wasm

# ---------------------------------------------------------------------------
# WASM uptime module (system uptime)
# ---------------------------------------------------------------------------
$(UPTIME_WASM): $(MODULES_DIR)/uptime/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/uptime.wasm

# ---------------------------------------------------------------------------
# WASM whoami module (current user display)
# ---------------------------------------------------------------------------
$(WHOAMI_WASM): $(MODULES_DIR)/whoami/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/whoami.wasm

# ---------------------------------------------------------------------------
# WASM hostname module (display hostname)
# ---------------------------------------------------------------------------
$(HOSTNAME_WASM): $(MODULES_DIR)/hostname/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/hostname.wasm

# ---------------------------------------------------------------------------
# WASM uname module (system identification)
# ---------------------------------------------------------------------------
$(UNAME_WASM): $(MODULES_DIR)/uname/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/uname.wasm

# ---------------------------------------------------------------------------
# WASM df module (disk usage display)
# ---------------------------------------------------------------------------
$(DF_WASM): $(MODULES_DIR)/df/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/df.wasm

# ---------------------------------------------------------------------------
# WASM free module (memory usage display)
# ---------------------------------------------------------------------------
$(FREE_WASM): $(MODULES_DIR)/free/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/free.wasm

# ---------------------------------------------------------------------------
# WASM ps module (process status)
# ---------------------------------------------------------------------------
$(PS_WASM): $(MODULES_DIR)/ps/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ps.wasm

# ---------------------------------------------------------------------------
# WASM kill module (send signal to process)
# ---------------------------------------------------------------------------
$(KILL_WASM): $(MODULES_DIR)/kill/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/kill.wasm

# ---------------------------------------------------------------------------
# WASM mkdir module (directory creation)
# ---------------------------------------------------------------------------
$(MKDIR_WASM): $(MODULES_DIR)/mkdir/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/mkdir.wasm

# ---------------------------------------------------------------------------
# WASM rmdir module (directory removal)
# ---------------------------------------------------------------------------
$(RMDIR_WASM): $(MODULES_DIR)/rmdir/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/rmdir.wasm

# ---------------------------------------------------------------------------
# WASM ln module (file link creation)
# ---------------------------------------------------------------------------
$(LN_WASM): $(MODULES_DIR)/ln/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ln.wasm

# ---------------------------------------------------------------------------
# WASM rm module (file deletion)
# ---------------------------------------------------------------------------
$(RM_WASM): $(MODULES_DIR)/rm/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/rm.wasm

# ---------------------------------------------------------------------------
# WASM cp module (file copying)
# ---------------------------------------------------------------------------
$(CP_WASM): $(MODULES_DIR)/cp/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cp.wasm

# ---------------------------------------------------------------------------
# WASM mv module (file moving)
# ---------------------------------------------------------------------------
$(MV_WASM): $(MODULES_DIR)/mv/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/mv.wasm

# ---------------------------------------------------------------------------
# WASM chmod module (file permission changes)
# ---------------------------------------------------------------------------
$(CHMOD_WASM): $(MODULES_DIR)/chmod/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/chmod.wasm

# ---------------------------------------------------------------------------
# WASM tty module (terminal identification)
# ---------------------------------------------------------------------------
$(TTY_WASM): $(MODULES_DIR)/tty/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tty.wasm

# ---------------------------------------------------------------------------
# WASM who module (logged-in user display)
# ---------------------------------------------------------------------------
$(WHO_WASM): $(MODULES_DIR)/who/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/who.wasm

# ---------------------------------------------------------------------------
# WASM stty module (terminal line settings)
# ---------------------------------------------------------------------------
$(STTY_WASM): $(MODULES_DIR)/stty/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/stty.wasm

# ---------------------------------------------------------------------------
# WASM tput module (terminal capability control)
# ---------------------------------------------------------------------------
$(TPUT_WASM): $(MODULES_DIR)/tput/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tput.wasm

# ---------------------------------------------------------------------------
# WASM readlink module (symlink target display)
# ---------------------------------------------------------------------------
$(READLINK_WASM): $(MODULES_DIR)/readlink/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/readlink.wasm

# ---------------------------------------------------------------------------
# WASM cal module (calendar display)
# ---------------------------------------------------------------------------
$(CAL_WASM): $(MODULES_DIR)/cal/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/cal.wasm

# ---------------------------------------------------------------------------
# WASM find module (file search)
# ---------------------------------------------------------------------------
$(FIND_WASM): $(MODULES_DIR)/find/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/find.wasm

# ---------------------------------------------------------------------------
# WASM du module (disk usage)
# ---------------------------------------------------------------------------
$(DU_WASM): $(MODULES_DIR)/du/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/du.wasm

# ---------------------------------------------------------------------------
# WASM mount module (mount points)
# ---------------------------------------------------------------------------
$(MOUNT_WASM): $(MODULES_DIR)/mount/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/mount.wasm

# ---------------------------------------------------------------------------
# WASM umount module (unmount file systems)
# ---------------------------------------------------------------------------
$(UMOUNT_WASM): $(MODULES_DIR)/umount/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/umount.wasm

# ---------------------------------------------------------------------------
# WASM ifconfig module (network interfaces)
# ---------------------------------------------------------------------------
$(IFCONFIG_WASM): $(MODULES_DIR)/ifconfig/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ifconfig.wasm

# ---------------------------------------------------------------------------
# WASM ping module (network connectivity)
# ---------------------------------------------------------------------------
$(PING_WASM): $(MODULES_DIR)/ping/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ping.wasm

# ---------------------------------------------------------------------------
# WASM netstat module (network statistics)
# ---------------------------------------------------------------------------
$(NETSTAT_WASM): $(MODULES_DIR)/netstat/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/netstat.wasm

# ---------------------------------------------------------------------------
# WASM route module (routing table)
# ---------------------------------------------------------------------------
$(ROUTE_WASM): $(MODULES_DIR)/route/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/route.wasm

# ---------------------------------------------------------------------------
# WASM traceroute module (route tracing)
# ---------------------------------------------------------------------------
$(TRACEROUTE_WASM): $(MODULES_DIR)/traceroute/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/traceroute.wasm

# ---------------------------------------------------------------------------
# WASM nslookup module (DNS lookup)
# ---------------------------------------------------------------------------
$(NSLOOKUP_WASM): $(MODULES_DIR)/nslookup/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/nslookup.wasm

# ---------------------------------------------------------------------------
# WASM wget module (file download)
# ---------------------------------------------------------------------------
$(WGET_WASM): $(MODULES_DIR)/wget/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/wget.wasm

# ---------------------------------------------------------------------------
# WASM curl module (data transfer)
# ---------------------------------------------------------------------------
$(CURL_WASM): $(MODULES_DIR)/curl/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/curl.wasm

# ---------------------------------------------------------------------------
# WASM ssh module (remote login)
# ---------------------------------------------------------------------------
$(SSH_WASM): $(MODULES_DIR)/ssh/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/ssh.wasm

# ---------------------------------------------------------------------------
# WASM scp module (secure file copy)
# ---------------------------------------------------------------------------
$(SCP_WASM): $(MODULES_DIR)/scp/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/scp.wasm

# ---------------------------------------------------------------------------
# WASM tar module (archive creation)
# ---------------------------------------------------------------------------
$(TAR_WASM): $(MODULES_DIR)/tar/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tar.wasm

# ---------------------------------------------------------------------------
# WASM gzip module (file compression)
# ---------------------------------------------------------------------------
$(GZIP_WASM): $(MODULES_DIR)/gzip/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/gzip.wasm

# ---------------------------------------------------------------------------
# WASM gunzip module (file decompression)
# ---------------------------------------------------------------------------
$(GUNZIP_WASM): $(MODULES_DIR)/gunzip/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/gunzip.wasm

# ---------------------------------------------------------------------------
# WASM zcat module (view compressed files)
# ---------------------------------------------------------------------------
$(ZCAT_WASM): $(MODULES_DIR)/zcat/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/zcat.wasm

# ---------------------------------------------------------------------------
# WASM bzip2 module (bzip2 compression)
# ---------------------------------------------------------------------------
$(BZIP2_WASM): $(MODULES_DIR)/bzip2/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/bzip2.wasm

# ---------------------------------------------------------------------------
# WASM bunzip2 module (bzip2 decompression)
# ---------------------------------------------------------------------------
$(BUNZIP2_WASM): $(MODULES_DIR)/bunzip2/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/bunzip2.wasm

# ---------------------------------------------------------------------------
# WASM lzma module (LZMA compression)
# ---------------------------------------------------------------------------
$(LZMA_WASM): $(MODULES_DIR)/lzma/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/lzma.wasm

# ---------------------------------------------------------------------------
# WASM unlzma module (LZMA decompression)
# ---------------------------------------------------------------------------
$(UNLZMA_WASM): $(MODULES_DIR)/unlzma/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/unlzma.wasm

# ---------------------------------------------------------------------------
# WASM xz module (XZ compression, milestone v100)
# ---------------------------------------------------------------------------
$(XZ_WASM): $(MODULES_DIR)/xz/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/xz.wasm

# ---------------------------------------------------------------------------
# WASM unxz module (XZ decompression)
# ---------------------------------------------------------------------------
$(UNXZ_WASM): $(MODULES_DIR)/unxz/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/unxz.wasm

# ---------------------------------------------------------------------------
# WASM sha256sum module (SHA256 checksum verification)
# ---------------------------------------------------------------------------
$(SHA256SUM_WASM): $(MODULES_DIR)/sha256sum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sha256sum.wasm

# ---------------------------------------------------------------------------
# WASM md5sum module (MD5 checksum verification)
# ---------------------------------------------------------------------------
$(MD5SUM_WASM): $(MODULES_DIR)/md5sum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/md5sum.wasm

# ---------------------------------------------------------------------------
# WASM base64 module (Base64 encoding)
# ---------------------------------------------------------------------------
$(BASE64_WASM): $(MODULES_DIR)/base64/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/base64.wasm

# ---------------------------------------------------------------------------
# WASM base64d module (Base64 decoding)
# ---------------------------------------------------------------------------
$(BASE64D_WASM): $(MODULES_DIR)/base64d/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/base64d.wasm

# ---------------------------------------------------------------------------
# WASM od module (octal dump)
# ---------------------------------------------------------------------------
$(OD_WASM): $(MODULES_DIR)/od/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/od.wasm

# ---------------------------------------------------------------------------
# WASM hexdump module (hex dump)
# ---------------------------------------------------------------------------
$(HEXDUMP_WASM): $(MODULES_DIR)/hexdump/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/hexdump.wasm

# ---------------------------------------------------------------------------
# WASM strings module (extract printable strings)
# ---------------------------------------------------------------------------
$(STRINGS_WASM): $(MODULES_DIR)/strings/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/strings.wasm

# ---------------------------------------------------------------------------
# WASM file module (file type detection)
# ---------------------------------------------------------------------------
$(FILETYPE_WASM): $(MODULES_DIR)/file/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/file.wasm

# ---------------------------------------------------------------------------
# WASM xxd module (hex viewer)
# ---------------------------------------------------------------------------
$(XXD_WASM): $(MODULES_DIR)/xxd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/xxd.wasm

# ---------------------------------------------------------------------------
# WASM less module (paginated file viewer)
# ---------------------------------------------------------------------------
$(LESS_WASM): $(MODULES_DIR)/less/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/less.wasm

# ---------------------------------------------------------------------------
# WASM more module (basic paginated file viewer)
# ---------------------------------------------------------------------------
$(MORE_WASM): $(MODULES_DIR)/more/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/more.wasm

# ---------------------------------------------------------------------------
# WASM join module (join sorted files)
# ---------------------------------------------------------------------------
$(JOIN_WASM): $(MODULES_DIR)/join/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/join.wasm

# ---------------------------------------------------------------------------
# WASM split module (file splitting)
# ---------------------------------------------------------------------------
$(SPLIT_WASM): $(MODULES_DIR)/split/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/split.wasm

# ---------------------------------------------------------------------------
# WASM patch module (apply patches)
# ---------------------------------------------------------------------------
$(PATCH_WASM): $(MODULES_DIR)/patch/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/patch.wasm

# ---------------------------------------------------------------------------
# WASM diff3 module (three-way file comparison)
# ---------------------------------------------------------------------------
$(DIFF3_WASM): $(MODULES_DIR)/diff3/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/diff3.wasm

# ---------------------------------------------------------------------------
# WASM tsort module (topological sorting)
# ---------------------------------------------------------------------------
$(TSORT_WASM): $(MODULES_DIR)/tsort/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/tsort.wasm

# ---------------------------------------------------------------------------
# WASM csplit module (context-based file splitter)
# ---------------------------------------------------------------------------
$(CSPLIT_WASM): $(MODULES_DIR)/csplit/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/csplit.wasm

# ---------------------------------------------------------------------------
# WASM fmt module (text formatting)
# ---------------------------------------------------------------------------
$(FMT_WASM): $(MODULES_DIR)/fmt/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/fmt.wasm

# ---------------------------------------------------------------------------
# WASM pr module (print formatting)
# ---------------------------------------------------------------------------
$(PR_WASM): $(MODULES_DIR)/pr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/pr.wasm $(KERNEL_DIR)/column.wasm $(KERNEL_DIR)/expand.wasm $(KERNEL_DIR)/unexpand.wasm $(KERNEL_DIR)/numfmt.wasm $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm


# ---------------------------------------------------------------------------
# WASM column module (column formatting)
# ---------------------------------------------------------------------------
$(COLUMN_WASM): $(MODULES_DIR)/column/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/column.wasm $(KERNEL_DIR)/expand.wasm $(KERNEL_DIR)/unexpand.wasm $(KERNEL_DIR)/numfmt.wasm $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM expand module (tab expansion)
# ---------------------------------------------------------------------------
$(EXPAND_WASM): $(MODULES_DIR)/expand/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/expand.wasm $(KERNEL_DIR)/unexpand.wasm $(KERNEL_DIR)/numfmt.wasm $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM unexpand module (space-to-tab conversion)
# ---------------------------------------------------------------------------
$(UNEXPAND_WASM): $(MODULES_DIR)/unexpand/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/unexpand.wasm $(KERNEL_DIR)/numfmt.wasm $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM numfmt module (number formatting)
# ---------------------------------------------------------------------------
$(NUMFMT_WASM): $(MODULES_DIR)/numfmt/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/numfmt.wasm $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM nproc module (processor count)
# ---------------------------------------------------------------------------
$(NPROC_WASM): $(MODULES_DIR)/nproc/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM hostid module (host identification)
# ---------------------------------------------------------------------------
$(HOSTID_WASM): $(MODULES_DIR)/hostid/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM sync module (filesystem sync)
# ---------------------------------------------------------------------------
$(SYNC_WASM): $(MODULES_DIR)/sync/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM link module (hard link creator)
# ---------------------------------------------------------------------------
$(LINK_WASM): $(MODULES_DIR)/link/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM unlink module (link removal)
# ---------------------------------------------------------------------------
$(UNLINK_WASM): $(MODULES_DIR)/unlink/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM logname module (login name display)
# ---------------------------------------------------------------------------
$(LOGNAME_WASM): $(MODULES_DIR)/logname/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM arch module (machine architecture)
# ---------------------------------------------------------------------------
$(ARCH_WASM): $(MODULES_DIR)/arch/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM setarch module (architecture setter)
# ---------------------------------------------------------------------------
$(SETARCH_WASM): $(MODULES_DIR)/setarch/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM nice module (process priority)
# ---------------------------------------------------------------------------
$(NICE_WASM): $(MODULES_DIR)/nice/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM renice module (process priority change)
# ---------------------------------------------------------------------------
$(RENICE_WASM): $(MODULES_DIR)/renice/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM timeout module (timed command execution)
# ---------------------------------------------------------------------------
$(TIMEOUT_WASM): $(MODULES_DIR)/timeout/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM stdbuf module (I/O buffer control)
# ---------------------------------------------------------------------------
$(STDBUF_WASM): $(MODULES_DIR)/stdbuf/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM factor module (prime factorization)
# ---------------------------------------------------------------------------
$(FACTOR_WASM): $(MODULES_DIR)/factor/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM seq module (number sequence generation)
# ---------------------------------------------------------------------------
$(SEQ_WASM): $(MODULES_DIR)/seq/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM realpath module (path resolution)
# ---------------------------------------------------------------------------
$(REALPATH_WASM): $(MODULES_DIR)/realpath/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM groups module (user group display)
# ---------------------------------------------------------------------------
$(GROUPS_WASM): $(MODULES_DIR)/groups/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM install module (file installation)
# ---------------------------------------------------------------------------
$(INSTALL_WASM): $(MODULES_DIR)/install/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM pathchk module (path validation)
# ---------------------------------------------------------------------------
$(PATHCHK_WASM): $(MODULES_DIR)/pathchk/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM mktemp module (temp file creation)
# ---------------------------------------------------------------------------
$(MKTEMP_WASM): $(MODULES_DIR)/mktemp/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM truncate module (file truncation)
# ---------------------------------------------------------------------------
$(TRUNCATE_WASM): $(MODULES_DIR)/truncate/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM nohup module (hangup-free execution)
# ---------------------------------------------------------------------------
$(NOHUP_WASM): $(MODULES_DIR)/nohup/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM envsubst module (environment variable substitution)
# ---------------------------------------------------------------------------
$(ENVSUBST_WASM): $(MODULES_DIR)/envsubst/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM dircolors module (color configuration)
# ---------------------------------------------------------------------------
$(DIRCOLORS_WASM): $(MODULES_DIR)/dircolors/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM expr module (expression evaluation)
# ---------------------------------------------------------------------------
$(EXPR_WASM): $(MODULES_DIR)/expr/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM test_cmd module (condition evaluation)
# ---------------------------------------------------------------------------
$(TEST_CMD_WASM): $(MODULES_DIR)/test_cmd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM dd module (data conversion and copy)
# ---------------------------------------------------------------------------
$(DD_WASM): $(MODULES_DIR)/dd/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM hashsum module (hash computation)
# ---------------------------------------------------------------------------
$(HASHSUM_WASM): $(MODULES_DIR)/hashsum/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM watch module (periodic execution)
# ---------------------------------------------------------------------------
$(WATCH_WASM): $(MODULES_DIR)/watch/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm

# ---------------------------------------------------------------------------
# WASM wasm_dis module (WASM disassembler)
# ---------------------------------------------------------------------------
$(WASM_DIS_WASM): $(MODULES_DIR)/wasm_dis/src/main.c
	@echo "  WASM  $<"
	$(CLANG) --target=wasm32-unknown-unknown -Oz -nostdlib -fno-builtin \
	    -Wl,--no-entry -Wl,--export=_start -o $@ $<
	cp $@ $(KERNEL_DIR)/wasm_dis.wasm
# ---------------------------------------------------------------------------
# Full kernel link
# ---------------------------------------------------------------------------
$(TARGET_ELF): $(ALL_OBJS) $(WASM3_LIB) $(INIT_WASM) $(SHELL_WASM) $(TEST_WASM) $(RAMDISK_TAR)
	@echo "  LD    $@"
	$(LD) $(LDFLAGS) -o $@ $(ASM_OBJS) $(C_OBJS) $(WASM3_OBJS)
	@SIZE=$$(wc -c < $@); echo "kernel.elf: $${SIZE} bytes ($$(( SIZE / 1024 ))KB)"

# ---------------------------------------------------------------------------
# Run in QEMU
# ---------------------------------------------------------------------------
run: $(TARGET_ELF)
	@echo "Starting AI-ASM AArch64 v0.5 in QEMU..."
	@echo "Press Ctrl+A then X to exit."
	$(QEMU) \
	    -M virt \
	    -cpu cortex-a53 \
	    -m 128M \
	    -kernel "$<" \
	    -nographic \
	    -no-reboot \
	    -serial mon:stdio

# ---------------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(KERNEL_DIR)/init.wasm $(KERNEL_DIR)/shell.wasm $(KERNEL_DIR)/test.wasm
	rm -f $(KERNEL_DIR)/editor.wasm $(KERNEL_DIR)/calc.wasm $(KERNEL_DIR)/paint.wasm $(KERNEL_DIR)/launcher.wasm
	rm -f $(KERNEL_DIR)/net_test.wasm $(KERNEL_DIR)/browser.wasm $(KERNEL_DIR)/proc_monitor.wasm $(KERNEL_DIR)/syslog.wasm $(KERNEL_DIR)/filemgr.wasm $(KERNEL_DIR)/settings.wasm $(KERNEL_DIR)/user.wasm $(KERNEL_DIR)/devmgr.wasm $(KERNEL_DIR)/httpd.wasm $(KERNEL_DIR)/dns_resolver.wasm $(KERNEL_DIR)/shmem_test.wasm $(KERNEL_DIR)/sem_test.wasm $(KERNEL_DIR)/grep.wasm $(KERNEL_DIR)/cat.wasm $(KERNEL_DIR)/echo.wasm $(KERNEL_DIR)/tee.wasm $(KERNEL_DIR)/wc.wasm $(KERNEL_DIR)/head.wasm $(KERNEL_DIR)/tail.wasm $(KERNEL_DIR)/sort.wasm $(KERNEL_DIR)/uniq.wasm $(KERNEL_DIR)/tr.wasm $(KERNEL_DIR)/cut.wasm $(KERNEL_DIR)/sed.wasm $(KERNEL_DIR)/xargs.wasm $(KERNEL_DIR)/date.wasm $(KERNEL_DIR)/awk.wasm $(KERNEL_DIR)/ls.wasm $(KERNEL_DIR)/pwd.wasm $(KERNEL_DIR)/env.wasm $(KERNEL_DIR)/printf.wasm $(KERNEL_DIR)/sleep.wasm $(KERNEL_DIR)/yes.wasm $(KERNEL_DIR)/rev.wasm $(KERNEL_DIR)/cmp.wasm $(KERNEL_DIR)/diff.wasm $(KERNEL_DIR)/basename.wasm $(KERNEL_DIR)/dirname.wasm $(KERNEL_DIR)/nl.wasm $(KERNEL_DIR)/shuf.wasm $(KERNEL_DIR)/comm.wasm $(KERNEL_DIR)/paste.wasm $(KERNEL_DIR)/fold.wasm $(KERNEL_DIR)/tac.wasm $(KERNEL_DIR)/cksum.wasm $(KERNEL_DIR)/sum.wasm $(KERNEL_DIR)/touch.wasm $(KERNEL_DIR)/true.wasm $(KERNEL_DIR)/false.wasm $(KERNEL_DIR)/id.wasm $(KERNEL_DIR)/uptime.wasm $(KERNEL_DIR)/whoami.wasm $(KERNEL_DIR)/hostname.wasm $(KERNEL_DIR)/uname.wasm $(KERNEL_DIR)/df.wasm $(KERNEL_DIR)/free.wasm $(KERNEL_DIR)/ps.wasm $(KERNEL_DIR)/kill.wasm $(KERNEL_DIR)/mkdir.wasm $(KERNEL_DIR)/rmdir.wasm $(KERNEL_DIR)/ln.wasm $(KERNEL_DIR)/rm.wasm $(KERNEL_DIR)/cp.wasm $(KERNEL_DIR)/mv.wasm $(KERNEL_DIR)/chmod.wasm $(KERNEL_DIR)/tty.wasm $(KERNEL_DIR)/who.wasm $(KERNEL_DIR)/stty.wasm $(KERNEL_DIR)/tput.wasm $(KERNEL_DIR)/readlink.wasm $(KERNEL_DIR)/cal.wasm $(KERNEL_DIR)/find.wasm $(KERNEL_DIR)/du.wasm $(KERNEL_DIR)/mount.wasm $(KERNEL_DIR)/umount.wasm $(KERNEL_DIR)/ifconfig.wasm $(KERNEL_DIR)/ping.wasm $(KERNEL_DIR)/netstat.wasm $(KERNEL_DIR)/route.wasm $(KERNEL_DIR)/traceroute.wasm $(KERNEL_DIR)/nslookup.wasm $(KERNEL_DIR)/wget.wasm $(KERNEL_DIR)/curl.wasm $(KERNEL_DIR)/ssh.wasm $(KERNEL_DIR)/scp.wasm $(KERNEL_DIR)/tar.wasm
	rm -f $(KERNEL_DIR)/gzip.wasm $(KERNEL_DIR)/gunzip.wasm $(KERNEL_DIR)/zcat.wasm $(KERNEL_DIR)/bzip2.wasm $(KERNEL_DIR)/bunzip2.wasm $(KERNEL_DIR)/lzma.wasm $(KERNEL_DIR)/unlzma.wasm $(KERNEL_DIR)/xz.wasm $(KERNEL_DIR)/unxz.wasm $(KERNEL_DIR)/sha256sum.wasm $(KERNEL_DIR)/md5sum.wasm $(KERNEL_DIR)/base64.wasm $(KERNEL_DIR)/base64d.wasm $(KERNEL_DIR)/od.wasm $(KERNEL_DIR)/hexdump.wasm $(KERNEL_DIR)/strings.wasm $(KERNEL_DIR)/file.wasm $(KERNEL_DIR)/xxd.wasm $(KERNEL_DIR)/less.wasm $(KERNEL_DIR)/more.wasm $(KERNEL_DIR)/join.wasm $(KERNEL_DIR)/split.wasm $(KERNEL_DIR)/patch.wasm $(KERNEL_DIR)/diff3.wasm $(KERNEL_DIR)/tsort.wasm $(KERNEL_DIR)/csplit.wasm $(KERNEL_DIR)/fmt.wasm $(KERNEL_DIR)/pr.wasm $(KERNEL_DIR)/column.wasm $(KERNEL_DIR)/expand.wasm $(KERNEL_DIR)/unexpand.wasm $(KERNEL_DIR)/numfmt.wasm $(KERNEL_DIR)/nproc.wasm $(KERNEL_DIR)/hostid.wasm $(KERNEL_DIR)/sync.wasm $(KERNEL_DIR)/link.wasm $(KERNEL_DIR)/unlink.wasm $(KERNEL_DIR)/logname.wasm $(KERNEL_DIR)/arch.wasm $(KERNEL_DIR)/setarch.wasm $(KERNEL_DIR)/nice.wasm $(KERNEL_DIR)/renice.wasm $(KERNEL_DIR)/timeout.wasm $(KERNEL_DIR)/stdbuf.wasm $(KERNEL_DIR)/factor.wasm $(KERNEL_DIR)/seq.wasm $(KERNEL_DIR)/realpath.wasm $(KERNEL_DIR)/groups.wasm $(KERNEL_DIR)/install.wasm $(KERNEL_DIR)/pathchk.wasm $(KERNEL_DIR)/mktemp.wasm $(KERNEL_DIR)/truncate.wasm $(KERNEL_DIR)/nohup.wasm $(KERNEL_DIR)/envsubst.wasm $(KERNEL_DIR)/dircolors.wasm $(KERNEL_DIR)/expr.wasm $(KERNEL_DIR)/test_cmd.wasm $(KERNEL_DIR)/dd.wasm $(KERNEL_DIR)/hashsum.wasm $(KERNEL_DIR)/watch.wasm $(KERNEL_DIR)/wasm_dis.wasm
	rm -f $(KERNEL_DIR)/ramdisk.tar
	rm -f $(MODULES_DIR)/init/init.wasm $(MODULES_DIR)/shell/shell.wasm $(MODULES_DIR)/test/test.wasm
	rm -f $(MODULES_DIR)/editor/editor.wasm $(MODULES_DIR)/calc/calc.wasm $(MODULES_DIR)/paint/paint.wasm $(MODULES_DIR)/launcher/launcher.wasm
	rm -f $(MODULES_DIR)/net_test/net_test.wasm $(MODULES_DIR)/browser/browser.wasm $(MODULES_DIR)/proc_monitor/proc_monitor.wasm $(MODULES_DIR)/syslog/syslog.wasm $(MODULES_DIR)/filemgr/filemgr.wasm $(MODULES_DIR)/settings/settings.wasm $(MODULES_DIR)/user/user.wasm $(MODULES_DIR)/devmgr/devmgr.wasm $(MODULES_DIR)/httpd/httpd.wasm $(MODULES_DIR)/dns_resolver/dns_resolver.wasm $(MODULES_DIR)/shmem_test/shmem_test.wasm $(MODULES_DIR)/sem_test/sem_test.wasm $(MODULES_DIR)/grep/grep.wasm $(MODULES_DIR)/cat/cat.wasm $(MODULES_DIR)/echo/echo.wasm $(MODULES_DIR)/tee/tee.wasm $(MODULES_DIR)/wc/wc.wasm $(MODULES_DIR)/head/head.wasm $(MODULES_DIR)/tail/tail.wasm $(MODULES_DIR)/sort/sort.wasm $(MODULES_DIR)/uniq/uniq.wasm $(MODULES_DIR)/tr/tr.wasm $(MODULES_DIR)/cut/cut.wasm $(MODULES_DIR)/sed/sed.wasm $(MODULES_DIR)/xargs/xargs.wasm $(MODULES_DIR)/date/date.wasm $(MODULES_DIR)/awk/awk.wasm $(MODULES_DIR)/ls/ls.wasm $(MODULES_DIR)/pwd/pwd.wasm $(MODULES_DIR)/env/env.wasm $(MODULES_DIR)/printf/printf.wasm $(MODULES_DIR)/sleep/sleep.wasm $(MODULES_DIR)/yes/yes.wasm $(MODULES_DIR)/rev/rev.wasm $(MODULES_DIR)/cmp/cmp.wasm $(MODULES_DIR)/diff/diff.wasm $(MODULES_DIR)/basename/basename.wasm $(MODULES_DIR)/dirname/dirname.wasm $(MODULES_DIR)/nl/nl.wasm $(MODULES_DIR)/shuf/shuf.wasm $(MODULES_DIR)/comm/comm.wasm $(MODULES_DIR)/paste/paste.wasm $(MODULES_DIR)/fold/fold.wasm $(MODULES_DIR)/tac/tac.wasm $(MODULES_DIR)/cksum/cksum.wasm $(MODULES_DIR)/sum/sum.wasm $(MODULES_DIR)/touch/touch.wasm $(MODULES_DIR)/true/true.wasm $(MODULES_DIR)/false/false.wasm $(MODULES_DIR)/id/id.wasm $(MODULES_DIR)/uptime/uptime.wasm $(MODULES_DIR)/whoami/whoami.wasm $(MODULES_DIR)/df/df.wasm $(MODULES_DIR)/free/free.wasm $(MODULES_DIR)/ps/ps.wasm $(MODULES_DIR)/kill/kill.wasm $(MODULES_DIR)/mkdir/mkdir.wasm $(MODULES_DIR)/rmdir/rmdir.wasm $(MODULES_DIR)/ln/ln.wasm $(MODULES_DIR)/rm/rm.wasm $(MODULES_DIR)/cp/cp.wasm $(MODULES_DIR)/mv/mv.wasm $(MODULES_DIR)/chmod/chmod.wasm $(MODULES_DIR)/tty/tty.wasm $(MODULES_DIR)/who/who.wasm $(MODULES_DIR)/stty/stty.wasm $(MODULES_DIR)/tput/tput.wasm $(MODULES_DIR)/readlink/readlink.wasm $(MODULES_DIR)/cal/cal.wasm $(MODULES_DIR)/gzip/gzip.wasm $(MODULES_DIR)/gunzip/gunzip.wasm $(MODULES_DIR)/zcat/zcat.wasm $(MODULES_DIR)/bzip2/bzip2.wasm $(MODULES_DIR)/bunzip2/bunzip2.wasm $(MODULES_DIR)/lzma/lzma.wasm $(MODULES_DIR)/unlzma/unlzma.wasm $(MODULES_DIR)/xz/xz.wasm $(MODULES_DIR)/column/column.wasm $(MODULES_DIR)/expand/expand.wasm $(MODULES_DIR)/unexpand/unexpand.wasm $(MODULES_DIR)/numfmt/numfmt.wasm $(MODULES_DIR)/nproc/nproc.wasm $(MODULES_DIR)/hostid/hostid.wasm $(MODULES_DIR)/sync/sync.wasm $(MODULES_DIR)/link/link.wasm $(MODULES_DIR)/unlink/unlink.wasm $(MODULES_DIR)/logname/logname.wasm $(MODULES_DIR)/arch/arch.wasm $(MODULES_DIR)/setarch/setarch.wasm $(MODULES_DIR)/nice/nice.wasm $(MODULES_DIR)/renice/renice.wasm $(MODULES_DIR)/timeout/timeout.wasm $(MODULES_DIR)/stdbuf/stdbuf.wasm $(MODULES_DIR)/factor/factor.wasm $(MODULES_DIR)/seq/seq.wasm $(MODULES_DIR)/realpath/realpath.wasm $(MODULES_DIR)/groups/groups.wasm $(MODULES_DIR)/install/install.wasm $(MODULES_DIR)/pathchk/pathchk.wasm $(MODULES_DIR)/mktemp/mktemp.wasm $(MODULES_DIR)/truncate/truncate.wasm $(MODULES_DIR)/nohup/nohup.wasm $(MODULES_DIR)/envsubst/envsubst.wasm $(MODULES_DIR)/dircolors/dircolors.wasm $(MODULES_DIR)/expr/expr.wasm $(MODULES_DIR)/test_cmd/test_cmd.wasm $(MODULES_DIR)/dd/dd.wasm $(MODULES_DIR)/hashsum/hashsum.wasm $(MODULES_DIR)/watch/watch.wasm $(MODULES_DIR)/wasm_dis/wasm_dis.wasm
	@echo "=== Build artifacts removed ==="
