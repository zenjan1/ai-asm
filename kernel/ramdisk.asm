/*
 * aiasm-aarch64/kernel/ramdisk.asm
 * Embeds WASM modules (init, shell) and RAM disk TAR into kernel binary.
 * Provides symbols for each embedded resource.
 */
.arch armv8-a

/* ---------------------------------------------------------------------------
 * Init module
 * --------------------------------------------------------------------------- */
.section .rodata.init_wasm
.global init_module_start
init_module_start:
    .incbin "kernel/init.wasm"
.global init_module_end
init_module_end:
.align 4
.global init_module_size
init_module_size:
    .int init_module_end - init_module_start

/* ---------------------------------------------------------------------------
 * Shell module
 * --------------------------------------------------------------------------- */
.section .rodata.shell_wasm
.global shell_module_start
shell_module_start:
    .incbin "kernel/shell.wasm"
.global shell_module_end
shell_module_end:
.align 4
.global shell_module_size
shell_module_size:
    .int shell_module_end - shell_module_start

/* ---------------------------------------------------------------------------
 * Test module (integration test)
 * --------------------------------------------------------------------------- */
.section .rodata.test_wasm
.global test_module_start
test_module_start:
    .incbin "kernel/test.wasm"
.global test_module_end
test_module_end:
.align 4
.global test_module_size
test_module_size:
    .int test_module_end - test_module_start

/* ---------------------------------------------------------------------------
 * Proc monitor module
 * --------------------------------------------------------------------------- */
.section .rodata.proc_monitor_wasm
.global proc_monitor_module_start
proc_monitor_module_start:
    .incbin "kernel/proc_monitor.wasm"
.global proc_monitor_module_end
proc_monitor_module_end:
.align 4
.global proc_monitor_module_size
proc_monitor_module_size:
    .int proc_monitor_module_end - proc_monitor_module_start

/* ---------------------------------------------------------------------------
 * Syslog module
 * --------------------------------------------------------------------------- */
.section .rodata.syslog_wasm
.global syslog_module_start
syslog_module_start:
    .incbin "kernel/syslog.wasm"
.global syslog_module_end
syslog_module_end:
.align 4
.global syslog_module_size
syslog_module_size:
    .int syslog_module_end - syslog_module_start

/* ---------------------------------------------------------------------------
 * File manager module
 * --------------------------------------------------------------------------- */
.section .rodata.filemgr_wasm
.global filemgr_module_start
filemgr_module_start:
    .incbin "kernel/filemgr.wasm"
.global filemgr_module_end
filemgr_module_end:
.align 4
.global filemgr_module_size
filemgr_module_size:
    .int filemgr_module_end - filemgr_module_start

/* ---------------------------------------------------------------------------
 * Settings module
 * --------------------------------------------------------------------------- */
.section .rodata.settings_wasm
.global settings_module_start
settings_module_start:
    .incbin "kernel/settings.wasm"
.global settings_module_end
settings_module_end:
.align 4
.global settings_module_size
settings_module_size:
    .int settings_module_end - settings_module_start

/* ---------------------------------------------------------------------------
 * User authentication module
 * --------------------------------------------------------------------------- */
.section .rodata.user_wasm
.global user_module_start
user_module_start:
    .incbin "kernel/user.wasm"
.global user_module_end
user_module_end:
.align 4
.global user_module_size
user_module_size:
    .int user_module_end - user_module_start

/* ---------------------------------------------------------------------------
 * Device manager module
 * --------------------------------------------------------------------------- */
.section .rodata.devmgr_wasm
.global devmgr_module_start
devmgr_module_start:
    .incbin "kernel/devmgr.wasm"
.global devmgr_module_end
devmgr_module_end:
.align 4
.global devmgr_module_size
devmgr_module_size:
    .int devmgr_module_end - devmgr_module_start

/* ---------------------------------------------------------------------------
 * HTTP server module
 * --------------------------------------------------------------------------- */
.section .rodata.httpd_wasm
.global httpd_module_start
httpd_module_start:
    .incbin "kernel/httpd.wasm"
.global httpd_module_end
httpd_module_end:
.align 4
.global httpd_module_size
httpd_module_size:
    .int httpd_module_end - httpd_module_start

/* ---------------------------------------------------------------------------
 * DNS resolver module
 * --------------------------------------------------------------------------- */
.section .rodata.dns_resolver_wasm
.global dns_resolver_module_start
dns_resolver_module_start:
    .incbin "kernel/dns_resolver.wasm"
.global dns_resolver_module_end
dns_resolver_module_end:
.align 4
.global dns_resolver_module_size
dns_resolver_module_size:
    .int dns_resolver_module_end - dns_resolver_module_start

/* ---------------------------------------------------------------------------
 * Shared memory test module
 * --------------------------------------------------------------------------- */
.section .rodata.shmem_test_wasm
.global shmem_test_module_start
shmem_test_module_start:
    .incbin "kernel/shmem_test.wasm"
.global shmem_test_module_end
shmem_test_module_end:
.align 4
.global shmem_test_module_size
shmem_test_module_size:
    .int shmem_test_module_end - shmem_test_module_start

/* ---------------------------------------------------------------------------
 * Grep module (simple string search utility)
 * --------------------------------------------------------------------------- */
.section .rodata.grep_wasm
.global grep_module_start
grep_module_start:
    .incbin "kernel/grep.wasm"
.global grep_module_end
grep_module_end:
.align 4
.global grep_module_size
grep_module_size:
    .int grep_module_end - grep_module_start

/* ---------------------------------------------------------------------------
 * Cat module
 * --------------------------------------------------------------------------- */
.section .rodata.cat_wasm
.global cat_module_start
cat_module_start:
    .incbin "kernel/cat.wasm"
.global cat_module_end
cat_module_end:
.align 4
.global cat_module_size
cat_module_size:
    .int cat_module_end - cat_module_start

/* ---------------------------------------------------------------------------
 * Echo module
 * --------------------------------------------------------------------------- */
.section .rodata.echo_wasm
.global echo_module_start
echo_module_start:
    .incbin "kernel/echo.wasm"
.global echo_module_end
echo_module_end:
.align 4
.global echo_module_size
echo_module_size:
    .int echo_module_end - echo_module_start

/* ---------------------------------------------------------------------------
 * Tee module
 * --------------------------------------------------------------------------- */
.section .rodata.tee_wasm
.global tee_module_start
tee_module_start:
    .incbin "kernel/tee.wasm"
.global tee_module_end
tee_module_end:
.align 4
.global tee_module_size
tee_module_size:
    .int tee_module_end - tee_module_start

/* ---------------------------------------------------------------------------
 * Wc module
 * --------------------------------------------------------------------------- */
.section .rodata.wc_wasm
.global wc_module_start
wc_module_start:
    .incbin "kernel/wc.wasm"
.global wc_module_end
wc_module_end:
.align 4
.global wc_module_size
wc_module_size:
    .int wc_module_end - wc_module_start

/* ---------------------------------------------------------------------------
 * Head module
 * --------------------------------------------------------------------------- */
.section .rodata.head_wasm
.global head_module_start
head_module_start:
    .incbin "kernel/head.wasm"
.global head_module_end
head_module_end:
.align 4
.global head_module_size
head_module_size:
    .int head_module_end - head_module_start

/* ---------------------------------------------------------------------------
 * Tail module
 * --------------------------------------------------------------------------- */
.section .rodata.tail_wasm
.global tail_module_start
tail_module_start:
    .incbin "kernel/tail.wasm"
.global tail_module_end
tail_module_end:
.align 4
.global tail_module_size
tail_module_size:
    .int tail_module_end - tail_module_start

/* ---------------------------------------------------------------------------
 * Sort module
 * --------------------------------------------------------------------------- */
.section .rodata.sort_wasm
.global sort_module_start
sort_module_start:
    .incbin "kernel/sort.wasm"
.global sort_module_end
sort_module_end:
.align 4
.global sort_module_size
sort_module_size:
    .int sort_module_end - sort_module_start

/* ---------------------------------------------------------------------------
 * Uniq module
 * --------------------------------------------------------------------------- */
.section .rodata.uniq_wasm
.global uniq_module_start
uniq_module_start:
    .incbin "kernel/uniq.wasm"
.global uniq_module_end
uniq_module_end:
.align 4
.global uniq_module_size
uniq_module_size:
    .int uniq_module_end - uniq_module_start

/* ---------------------------------------------------------------------------
 * Tr module
 * --------------------------------------------------------------------------- */
.section .rodata.tr_wasm
.global tr_module_start
tr_module_start:
    .incbin "kernel/tr.wasm"
.global tr_module_end
tr_module_end:
.align 4
.global tr_module_size
tr_module_size:
    .int tr_module_end - tr_module_start

/* ---------------------------------------------------------------------------
 * Cut module
 * --------------------------------------------------------------------------- */
.section .rodata.cut_wasm
.global cut_module_start
cut_module_start:
    .incbin "kernel/cut.wasm"
.global cut_module_end
cut_module_end:
.align 4
.global cut_module_size
cut_module_size:
    .int cut_module_end - cut_module_start

/* ---------------------------------------------------------------------------
 * Sed module
 * --------------------------------------------------------------------------- */
.section .rodata.sed_wasm
.global sed_module_start
sed_module_start:
    .incbin "kernel/sed.wasm"
.global sed_module_end
sed_module_end:
.align 4
.global sed_module_size
sed_module_size:
    .int sed_module_end - sed_module_start

/* ---------------------------------------------------------------------------
 * Xargs module
 * --------------------------------------------------------------------------- */
.section .rodata.xargs_wasm
.global xargs_module_start
xargs_module_start:
    .incbin "kernel/xargs.wasm"
.global xargs_module_end
xargs_module_end:
.align 4
.global xargs_module_size
xargs_module_size:
    .int xargs_module_end - xargs_module_start

/* ---------------------------------------------------------------------------
 * Date module
 * --------------------------------------------------------------------------- */
.section .rodata.date_wasm
.global date_module_start
date_module_start:
    .incbin "kernel/date.wasm"
.global date_module_end
date_module_end:
.align 4
.global date_module_size
date_module_size:
    .int date_module_end - date_module_start

/* ---------------------------------------------------------------------------
 * Awk module
 * --------------------------------------------------------------------------- */
.section .rodata.awk_wasm
.global awk_module_start
awk_module_start:
    .incbin "kernel/awk.wasm"
.global awk_module_end
awk_module_end:
.align 4
.global awk_module_size
awk_module_size:
    .int awk_module_end - awk_module_start

/* ---------------------------------------------------------------------------
 * Ls module
 * --------------------------------------------------------------------------- */
.section .rodata.ls_wasm
.global ls_module_start
ls_module_start:
    .incbin "kernel/ls.wasm"
.global ls_module_end
ls_module_end:
.align 4
.global ls_module_size
ls_module_size:
    .int ls_module_end - ls_module_start

/* ---------------------------------------------------------------------------
 * Pwd module
 * --------------------------------------------------------------------------- */
.section .rodata.pwd_wasm
.global pwd_module_start
pwd_module_start:
    .incbin "kernel/pwd.wasm"
.global pwd_module_end
pwd_module_end:
.align 4
.global pwd_module_size
pwd_module_size:
    .int pwd_module_end - pwd_module_start

/* ---------------------------------------------------------------------------
 * Env module
 * --------------------------------------------------------------------------- */
.section .rodata.env_wasm
.global env_module_start
env_module_start:
    .incbin "kernel/env.wasm"
.global env_module_end
env_module_end:
.align 4
.global env_module_size
env_module_size:
    .int env_module_end - env_module_start

/* ---------------------------------------------------------------------------
 * Printf module
 * --------------------------------------------------------------------------- */
.section .rodata.printf_wasm
.global printf_module_start
printf_module_start:
    .incbin "kernel/printf.wasm"
.global printf_module_end
printf_module_end:
.align 4
.global printf_module_size
printf_module_size:
    .int printf_module_end - printf_module_start

/* ---------------------------------------------------------------------------
 * Sleep module
 * --------------------------------------------------------------------------- */
.section .rodata.sleep_wasm
.global sleep_module_start
sleep_module_start:
    .incbin "kernel/sleep.wasm"
.global sleep_module_end
sleep_module_end:
.align 4
.global sleep_module_size
sleep_module_size:
    .int sleep_module_end - sleep_module_start

/* ---------------------------------------------------------------------------
 * Yes module
 * --------------------------------------------------------------------------- */
.section .rodata.yes_wasm
.global yes_module_start
yes_module_start:
    .incbin "kernel/yes.wasm"
.global yes_module_end
yes_module_end:
.align 4
.global yes_module_size
yes_module_size:
    .int yes_module_end - yes_module_start

/* ---------------------------------------------------------------------------
 * Rev module (line reversal)
 * --------------------------------------------------------------------------- */
.section .rodata.rev_wasm
.global rev_module_start
rev_module_start:
    .incbin "kernel/rev.wasm"
.global rev_module_end
rev_module_end:
.align 4
.global rev_module_size
rev_module_size:
    .int rev_module_end - rev_module_start

/* ---------------------------------------------------------------------------
 * Cmp module (file comparison)
 * --------------------------------------------------------------------------- */
.section .rodata.cmp_wasm
.global cmp_module_start
cmp_module_start:
    .incbin "kernel/cmp.wasm"
.global cmp_module_end
cmp_module_end:
.align 4
.global cmp_module_size
cmp_module_size:
    .int cmp_module_end - cmp_module_start

/* ---------------------------------------------------------------------------
 * Diff module (line-by-line file comparison)
 * --------------------------------------------------------------------------- */
.section .rodata.diff_wasm
.global diff_module_start
diff_module_start:
    .incbin "kernel/diff.wasm"
.global diff_module_end
diff_module_end:
.align 4
.global diff_module_size
diff_module_size:
    .int diff_module_end - diff_module_start

/* ---------------------------------------------------------------------------
 * Basename module (path processing)
 * --------------------------------------------------------------------------- */
.section .rodata.basename_wasm
.global basename_module_start
basename_module_start:
    .incbin "kernel/basename.wasm"
.global basename_module_end
basename_module_end:
.align 4
.global basename_module_size
basename_module_size:
    .int basename_module_end - basename_module_start

/* ---------------------------------------------------------------------------
 * Dirname module (directory path extraction)
 * --------------------------------------------------------------------------- */
.section .rodata.dirname_wasm
.global dirname_module_start
dirname_module_start:
    .incbin "kernel/dirname.wasm"
.global dirname_module_end
dirname_module_end:
.align 4
.global dirname_module_size
dirname_module_size:
    .int dirname_module_end - dirname_module_start

/* ---------------------------------------------------------------------------
 * Nl module (line numbering)
 * --------------------------------------------------------------------------- */
.section .rodata.nl_wasm
.global nl_module_start
nl_module_start:
    .incbin "kernel/nl.wasm"
.global nl_module_end
nl_module_end:
.align 4
.global nl_module_size
nl_module_size:
    .int nl_module_end - nl_module_start

/* ---------------------------------------------------------------------------
 * Shuf module (random line shuffling)
 * --------------------------------------------------------------------------- */
.section .rodata.shuf_wasm
.global shuf_module_start
shuf_module_start:
    .incbin "kernel/shuf.wasm"
.global shuf_module_end
shuf_module_end:
.align 4
.global shuf_module_size
shuf_module_size:
    .int shuf_module_end - shuf_module_start

/* ---------------------------------------------------------------------------
 * Comm module (compare sorted files)
 * --------------------------------------------------------------------------- */
.section .rodata.comm_wasm
.global comm_module_start
comm_module_start:
    .incbin "kernel/comm.wasm"
.global comm_module_end
comm_module_end:
.align 4
.global comm_module_size
comm_module_size:
    .int comm_module_end - comm_module_start

/* ---------------------------------------------------------------------------
 * Paste module (merge lines)
 * --------------------------------------------------------------------------- */
.section .rodata.paste_wasm
.global paste_module_start
paste_module_start:
    .incbin "kernel/paste.wasm"
.global paste_module_end
paste_module_end:
.align 4
.global paste_module_size
paste_module_size:
    .int paste_module_end - paste_module_start

/* ---------------------------------------------------------------------------
 * Fold module (line wrapping)
 * --------------------------------------------------------------------------- */
.section .rodata.fold_wasm
.global fold_module_start
fold_module_start:
    .incbin "kernel/fold.wasm"
.global fold_module_end
fold_module_end:
.align 4
.global fold_module_size
fold_module_size:
    .int fold_module_end - fold_module_start

/* ---------------------------------------------------------------------------
 * Tac module (reverse file output)
 * --------------------------------------------------------------------------- */
.section .rodata.tac_wasm
.global tac_module_start
tac_module_start:
    .incbin "kernel/tac.wasm"
.global tac_module_end
tac_module_end:
.align 4
.global tac_module_size
tac_module_size:
    .int tac_module_end - tac_module_start

/* ---------------------------------------------------------------------------
 * Cksum module (file checksum)
 * --------------------------------------------------------------------------- */
.section .rodata.cksum_wasm
.global cksum_module_start
cksum_module_start:
    .incbin "kernel/cksum.wasm"
.global cksum_module_end
cksum_module_end:
.align 4
.global cksum_module_size
cksum_module_size:
    .int cksum_module_end - cksum_module_start

/* ---------------------------------------------------------------------------
 * Sum module (simple checksum)
 * --------------------------------------------------------------------------- */
.section .rodata.sum_wasm
.global sum_module_start
sum_module_start:
    .incbin "kernel/sum.wasm"
.global sum_module_end
sum_module_end:
.align 4
.global sum_module_size
sum_module_size:
    .int sum_module_end - sum_module_start

/* ---------------------------------------------------------------------------
 * Touch module (create empty files)
 * --------------------------------------------------------------------------- */
.section .rodata.touch_wasm
.global touch_module_start
touch_module_start:
    .incbin "kernel/touch.wasm"
.global touch_module_end
touch_module_end:
.align 4
.global touch_module_size
touch_module_size:
    .int touch_module_end - touch_module_start

/* ---------------------------------------------------------------------------
 * True module (always succeeds)
 * --------------------------------------------------------------------------- */
.section .rodata.true_wasm
.global true_module_start
true_module_start:
    .incbin "kernel/true.wasm"
.global true_module_end
true_module_end:
.align 4
.global true_module_size
true_module_size:
    .int true_module_end - true_module_start

/* ---------------------------------------------------------------------------
 * False module (always fails)
 * --------------------------------------------------------------------------- */
.section .rodata.false_wasm
.global false_module_start
false_module_start:
    .incbin "kernel/false.wasm"
.global false_module_end
false_module_end:
.align 4
.global false_module_size
false_module_size:
    .int false_module_end - false_module_start

/* ---------------------------------------------------------------------------
 * Id module (user identification)
 * --------------------------------------------------------------------------- */
.section .rodata.id_wasm
.global id_module_start
id_module_start:
    .incbin "kernel/id.wasm"
.global id_module_end
id_module_end:
.align 4
.global id_module_size
id_module_size:
    .int id_module_end - id_module_start

/* ---------------------------------------------------------------------------
 * Uptime module (system uptime)
 * --------------------------------------------------------------------------- */
.section .rodata.uptime_wasm
.global uptime_module_start
uptime_module_start:
    .incbin "kernel/uptime.wasm"
.global uptime_module_end
uptime_module_end:
.align 4
.global uptime_module_size
uptime_module_size:
    .int uptime_module_end - uptime_module_start

/* ---------------------------------------------------------------------------
 * Whoami module (current user display)
 * --------------------------------------------------------------------------- */
.section .rodata.whoami_wasm
.global whoami_module_start
whoami_module_start:
    .incbin "kernel/whoami.wasm"
.global whoami_module_end
whoami_module_end:
.align 4
.global whoami_module_size
whoami_module_size:
    .int whoami_module_end - whoami_module_start

/* ---------------------------------------------------------------------------
 * Hostname module (host identification)
 * --------------------------------------------------------------------------- */
.section .rodata.hostname_wasm
.global hostname_module_start
hostname_module_start:
    .incbin "kernel/hostname.wasm"
.global hostname_module_end
hostname_module_end:
.align 4
.global hostname_module_size
hostname_module_size:
    .int hostname_module_end - hostname_module_start

/* ---------------------------------------------------------------------------
 * Uname module (system identification)
 * --------------------------------------------------------------------------- */
.section .rodata.uname_wasm
.global uname_module_start
uname_module_start:
    .incbin "kernel/uname.wasm"
.global uname_module_end
uname_module_end:
.align 4
.global uname_module_size
uname_module_size:
    .int uname_module_end - uname_module_start

/* ---------------------------------------------------------------------------
 * Df module (disk usage display)
 * --------------------------------------------------------------------------- */
.section .rodata.df_wasm
.global df_module_start
df_module_start:
    .incbin "kernel/df.wasm"
.global df_module_end
df_module_end:
.align 4
.global df_module_size
df_module_size:
    .int df_module_end - df_module_start

/* ---------------------------------------------------------------------------
 * Free module (memory usage display)
 * --------------------------------------------------------------------------- */
.section .rodata.free_wasm
.global free_module_start
free_module_start:
    .incbin "kernel/free.wasm"
.global free_module_end
free_module_end:
.align 4
.global free_module_size
free_module_size:
    .int free_module_end - free_module_start

/* ---------------------------------------------------------------------------
 * Ps module (process status)
 * --------------------------------------------------------------------------- */
.section .rodata.ps_wasm
.global ps_module_start
ps_module_start:
    .incbin "kernel/ps.wasm"
.global ps_module_end
ps_module_end:
.align 4
.global ps_module_size
ps_module_size:
    .int ps_module_end - ps_module_start

/* ---------------------------------------------------------------------------
 * Kill module (send signal to process)
 * --------------------------------------------------------------------------- */
.section .rodata.kill_wasm
.global kill_module_start
kill_module_start:
    .incbin "kernel/kill.wasm"
.global kill_module_end
kill_module_end:
.align 4
.global kill_module_size
kill_module_size:
    .int kill_module_end - kill_module_start

/* ---------------------------------------------------------------------------
 * Mkdir module (directory creation)
 * --------------------------------------------------------------------------- */
.section .rodata.mkdir_wasm
.global mkdir_module_start
mkdir_module_start:
    .incbin "kernel/mkdir.wasm"
.global mkdir_module_end
mkdir_module_end:
.align 4
.global mkdir_module_size
mkdir_module_size:
    .int mkdir_module_end - mkdir_module_start

/* ---------------------------------------------------------------------------
 * Rmdir module (directory removal)
 * --------------------------------------------------------------------------- */
.section .rodata.rmdir_wasm
.global rmdir_module_start
rmdir_module_start:
    .incbin "kernel/rmdir.wasm"
.global rmdir_module_end
rmdir_module_end:
.align 4
.global rmdir_module_size
rmdir_module_size:
    .int rmdir_module_end - rmdir_module_start

/* ---------------------------------------------------------------------------
 * Ln module (file link creation)
 * --------------------------------------------------------------------------- */
.section .rodata.ln_wasm
.global ln_module_start
ln_module_start:
    .incbin "kernel/ln.wasm"
.global ln_module_end
ln_module_end:
.align 4
.global ln_module_size
ln_module_size:
    .int ln_module_end - ln_module_start

/* ---------------------------------------------------------------------------
 * Rm module (file deletion)
 * --------------------------------------------------------------------------- */
.section .rodata.rm_wasm
.global rm_module_start
rm_module_start:
    .incbin "kernel/rm.wasm"
.global rm_module_end
rm_module_end:
.align 4
.global rm_module_size
rm_module_size:
    .int rm_module_end - rm_module_start

/* ---------------------------------------------------------------------------
 * Cp module (file copying)
 * --------------------------------------------------------------------------- */
.section .rodata.cp_wasm
.global cp_module_start
cp_module_start:
    .incbin "kernel/cp.wasm"
.global cp_module_end
cp_module_end:
.align 4
.global cp_module_size
cp_module_size:
    .int cp_module_end - cp_module_start

/* ---------------------------------------------------------------------------
 * Mv module (file moving)
 * --------------------------------------------------------------------------- */
.section .rodata.mv_wasm
.global mv_module_start
mv_module_start:
    .incbin "kernel/mv.wasm"
.global mv_module_end
mv_module_end:
.align 4
.global mv_module_size
mv_module_size:
    .int mv_module_end - mv_module_start

/* ---------------------------------------------------------------------------
 * Chmod module (file permission changes)
 * --------------------------------------------------------------------------- */
.section .rodata.chmod_wasm
.global chmod_module_start
chmod_module_start:
    .incbin "kernel/chmod.wasm"
.global chmod_module_end
chmod_module_end:
.align 4
.global chmod_module_size
chmod_module_size:
    .int chmod_module_end - chmod_module_start

/* ---------------------------------------------------------------------------
 * RAM disk (USTAR TAR archive)
 * --------------------------------------------------------------------------- */
.section .rodata.ramdisk
.global ramdisk_start
ramdisk_start:
    .incbin "kernel/ramdisk.tar"
.global ramdisk_end
ramdisk_end:
.align 4
.global ramdisk_size
ramdisk_size:
    .int ramdisk_end - ramdisk_start
