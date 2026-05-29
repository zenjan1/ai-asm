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
 * Tty module (terminal identification)
 * --------------------------------------------------------------------------- */
.section .rodata.tty_wasm
.global tty_module_start
tty_module_start:
    .incbin "kernel/tty.wasm"
.global tty_module_end
tty_module_end:
.align 4
.global tty_module_size
tty_module_size:
    .int tty_module_end - tty_module_start

/* ---------------------------------------------------------------------------
 * Who module (logged-in user display)
 * --------------------------------------------------------------------------- */
.section .rodata.who_wasm
.global who_module_start
who_module_start:
    .incbin "kernel/who.wasm"
.global who_module_end
who_module_end:
.align 4
.global who_module_size
who_module_size:
    .int who_module_end - who_module_start

/* ---------------------------------------------------------------------------
 * Stty module (terminal line settings)
 * --------------------------------------------------------------------------- */
.section .rodata.stty_wasm
.global stty_module_start
stty_module_start:
    .incbin "kernel/stty.wasm"
.global stty_module_end
stty_module_end:
.align 4
.global stty_module_size
stty_module_size:
    .int stty_module_end - stty_module_start

/* ---------------------------------------------------------------------------
 * Tput module (terminal capability control)
 * --------------------------------------------------------------------------- */
.section .rodata.tput_wasm
.global tput_module_start
tput_module_start:
    .incbin "kernel/tput.wasm"
.global tput_module_end
tput_module_end:
.align 4
.global tput_module_size
tput_module_size:
    .int tput_module_end - tput_module_start

/* ---------------------------------------------------------------------------
 * Readlink module (symlink target display)
 * --------------------------------------------------------------------------- */
.section .rodata.readlink_wasm
.global readlink_module_start
readlink_module_start:
    .incbin "kernel/readlink.wasm"
.global readlink_module_end
readlink_module_end:
.align 4
.global readlink_module_size
readlink_module_size:
    .int readlink_module_end - readlink_module_start

/* ---------------------------------------------------------------------------
 * WASM module: cal
 * --------------------------------------------------------------------------- */
.section .rodata.cal_wasm
.global cal_module_start
cal_module_start:
    .incbin "kernel/cal.wasm"
.global cal_module_end
cal_module_end:

.global cal_module_size
cal_module_size:
    .int cal_module_end - cal_module_start

/* ---------------------------------------------------------------------------
 * WASM module: find
 * --------------------------------------------------------------------------- */
.section .rodata.find_wasm
.global find_module_start
find_module_start:
    .incbin "kernel/find.wasm"
.global find_module_end
find_module_end:

.global find_module_size
find_module_size:
    .int find_module_end - find_module_start

/* ---------------------------------------------------------------------------
 * WASM module: du
 * --------------------------------------------------------------------------- */
.section .rodata.du_wasm
.global du_module_start
du_module_start:
    .incbin "kernel/du.wasm"
.global du_module_end
du_module_end:

.global du_module_size
du_module_size:
    .int du_module_end - du_module_start

/* ---------------------------------------------------------------------------
 * WASM module: mount
 * --------------------------------------------------------------------------- */
.section .rodata.mount_wasm
.global mount_module_start
mount_module_start:
    .incbin "kernel/mount.wasm"
.global mount_module_end
mount_module_end:

.global mount_module_size
mount_module_size:
    .int mount_module_end - mount_module_start

/* ---------------------------------------------------------------------------
 * WASM module: umount
 * --------------------------------------------------------------------------- */
.section .rodata.umount_wasm
.global umount_module_start
umount_module_start:
    .incbin "kernel/umount.wasm"
.global umount_module_end
umount_module_end:

.global umount_module_size
umount_module_size:
    .int umount_module_end - umount_module_start

/* ---------------------------------------------------------------------------
 * WASM module: ifconfig
 * --------------------------------------------------------------------------- */
.section .rodata.ifconfig_wasm
.global ifconfig_module_start
ifconfig_module_start:
    .incbin "kernel/ifconfig.wasm"
.global ifconfig_module_end
ifconfig_module_end:

.global ifconfig_module_size
ifconfig_module_size:
    .int ifconfig_module_end - ifconfig_module_start

/* ---------------------------------------------------------------------------
 * WASM module: ping
 * --------------------------------------------------------------------------- */
.section .rodata.ping_wasm
.global ping_module_start
ping_module_start:
    .incbin "kernel/ping.wasm"
.global ping_module_end
ping_module_end:

.global ping_module_size
ping_module_size:
    .int ping_module_end - ping_module_start

/* ---------------------------------------------------------------------------
 * WASM module: netstat
 * --------------------------------------------------------------------------- */
.section .rodata.netstat_wasm
.global netstat_module_start
netstat_module_start:
    .incbin "kernel/netstat.wasm"
.global netstat_module_end
netstat_module_end:

.global netstat_module_size
netstat_module_size:
    .int netstat_module_end - netstat_module_start

/* ---------------------------------------------------------------------------
 * WASM module: route
 * --------------------------------------------------------------------------- */
.section .rodata.route_wasm
.global route_module_start
route_module_start:
    .incbin "kernel/route.wasm"
.global route_module_end
route_module_end:

.global route_module_size
route_module_size:
    .int route_module_end - route_module_start

/* ---------------------------------------------------------------------------
 * WASM module: traceroute
 * --------------------------------------------------------------------------- */
.section .rodata.traceroute_wasm
.global traceroute_module_start
traceroute_module_start:
    .incbin "kernel/traceroute.wasm"
.global traceroute_module_end
traceroute_module_end:

.global traceroute_module_size
traceroute_module_size:
    .int traceroute_module_end - traceroute_module_start

/* ---------------------------------------------------------------------------
 * WASM module: nslookup
 * --------------------------------------------------------------------------- */
.section .rodata.nslookup_wasm
.global nslookup_module_start
nslookup_module_start:
    .incbin "kernel/nslookup.wasm"
.global nslookup_module_end
nslookup_module_end:

.global nslookup_module_size
nslookup_module_size:
    .int nslookup_module_end - nslookup_module_start

/* ---------------------------------------------------------------------------
 * WASM module: wget
 * --------------------------------------------------------------------------- */
.section .rodata.wget_wasm
.global wget_module_start
wget_module_start:
    .incbin "kernel/wget.wasm"
.global wget_module_end
wget_module_end:

.global wget_module_size
wget_module_size:
    .int wget_module_end - wget_module_start

/* ---------------------------------------------------------------------------
 * WASM module: curl
 * --------------------------------------------------------------------------- */
.section .rodata.curl_wasm
.global curl_module_start
curl_module_start:
    .incbin "kernel/curl.wasm"
.global curl_module_end
curl_module_end:

.global curl_module_size
curl_module_size:
    .int curl_module_end - curl_module_start

/* ---------------------------------------------------------------------------
 * WASM module: ssh
 * --------------------------------------------------------------------------- */
.section .rodata.ssh_wasm
.global ssh_module_start
ssh_module_start:
    .incbin "kernel/ssh.wasm"
.global ssh_module_end
ssh_module_end:

.global ssh_module_size
ssh_module_size:
    .int ssh_module_end - ssh_module_start

/* ---------------------------------------------------------------------------
 * WASM module: scp
 * --------------------------------------------------------------------------- */
.section .rodata.scp_wasm
.global scp_module_start
scp_module_start:
    .incbin "kernel/scp.wasm"
.global scp_module_end
scp_module_end:

.global scp_module_size
scp_module_size:
    .int scp_module_end - scp_module_start

/* ---------------------------------------------------------------------------
 * WASM module: tar
 * --------------------------------------------------------------------------- */
.section .rodata.tar_wasm
.global tar_module_start
tar_module_start:
    .incbin "kernel/tar.wasm"
.global tar_module_end
tar_module_end:

.global tar_module_size
tar_module_size:
    .int tar_module_end - tar_module_start

/* ---------------------------------------------------------------------------
 * WASM module: gzip
 * --------------------------------------------------------------------------- */
.section .rodata.gzip_wasm
.global gzip_module_start
gzip_module_start:
    .incbin "kernel/gzip.wasm"
.global gzip_module_end
gzip_module_end:

.global gzip_module_size
gzip_module_size:
    .int gzip_module_end - gzip_module_start

/* ---------------------------------------------------------------------------
 * WASM module: gunzip
 * --------------------------------------------------------------------------- */
.section .rodata.gunzip_wasm
.global gunzip_module_start
gunzip_module_start:
    .incbin "kernel/gunzip.wasm"
.global gunzip_module_end
gunzip_module_end:

.global gunzip_module_size
gunzip_module_size:
    .int gunzip_module_end - gunzip_module_start

/* ---------------------------------------------------------------------------
 * WASM module: zcat
 * --------------------------------------------------------------------------- */
.section .rodata.zcat_wasm
.global zcat_module_start
zcat_module_start:
    .incbin "kernel/zcat.wasm"
.global zcat_module_end
zcat_module_end:

.global zcat_module_size
zcat_module_size:
    .int zcat_module_end - zcat_module_start

/* ---------------------------------------------------------------------------
 * WASM module: bzip2
 * --------------------------------------------------------------------------- */
.section .rodata.bzip2_wasm
.global bzip2_module_start
bzip2_module_start:
    .incbin "kernel/bzip2.wasm"
.global bzip2_module_end
bzip2_module_end:

.global bzip2_module_size
bzip2_module_size:
    .int bzip2_module_end - bzip2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: bunzip2
 * --------------------------------------------------------------------------- */
.section .rodata.bunzip2_wasm
.global bunzip2_module_start
bunzip2_module_start:
    .incbin "kernel/bunzip2.wasm"
.global bunzip2_module_end
bunzip2_module_end:

.global bunzip2_module_size
bunzip2_module_size:
    .int bunzip2_module_end - bunzip2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: lzma
 * --------------------------------------------------------------------------- */
.section .rodata.lzma_wasm
.global lzma_module_start
lzma_module_start:
    .incbin "kernel/lzma.wasm"
.global lzma_module_end
lzma_module_end:

.global lzma_module_size
lzma_module_size:
    .int lzma_module_end - lzma_module_start

/* ---------------------------------------------------------------------------
 * WASM module: unlzma
 * --------------------------------------------------------------------------- */
.section .rodata.unlzma_wasm
.global unlzma_module_start
unlzma_module_start:
    .incbin "kernel/unlzma.wasm"
.global unlzma_module_end
unlzma_module_end:

.global unlzma_module_size
unlzma_module_size:
    .int unlzma_module_end - unlzma_module_start

/* ---------------------------------------------------------------------------
 * WASM module: xz (v100.0 milestone)
 * --------------------------------------------------------------------------- */
.section .rodata.xz_wasm
.global xz_module_start
xz_module_start:
    .incbin "kernel/xz.wasm"
.global xz_module_end
xz_module_end:

.global xz_module_size
xz_module_size:
    .int xz_module_end - xz_module_start

.section .rodata.unxz_wasm
.global unxz_module_start
unxz_module_start:
    .incbin "kernel/unxz.wasm"
.global unxz_module_end
unxz_module_end:

.global unxz_module_size
unxz_module_size:
    .int unxz_module_end - unxz_module_start

.section .rodata.sha256sum_wasm
.global sha256sum_module_start
sha256sum_module_start:
    .incbin "kernel/sha256sum.wasm"
.global sha256sum_module_end
sha256sum_module_end:

.global sha256sum_module_size
sha256sum_module_size:
    .int sha256sum_module_end - sha256sum_module_start

.section .rodata.md5sum_wasm
.global md5sum_module_start
md5sum_module_start:
    .incbin "kernel/md5sum.wasm"
.global md5sum_module_end
md5sum_module_end:

.global md5sum_module_size
md5sum_module_size:
    .int md5sum_module_end - md5sum_module_start

.section .rodata.base64_wasm
.global base64_module_start
base64_module_start:
    .incbin "kernel/base64.wasm"
.global base64_module_end
base64_module_end:

.global base64_module_size
base64_module_size:
    .int base64_module_end - base64_module_start

.section .rodata.base64d_wasm
.global base64d_module_start
base64d_module_start:
    .incbin "kernel/base64d.wasm"
.global base64d_module_end
base64d_module_end:

.global base64d_module_size
base64d_module_size:
    .int base64d_module_end - base64d_module_start

.section .rodata.od_wasm
.global od_module_start
od_module_start:
    .incbin "kernel/od.wasm"
.global od_module_end
od_module_end:

.global od_module_size
od_module_size:
    .int od_module_end - od_module_start

.section .rodata.hexdump_wasm
.global hexdump_module_start
hexdump_module_start:
    .incbin "kernel/hexdump.wasm"
.global hexdump_module_end
hexdump_module_end:

.global hexdump_module_size
hexdump_module_size:
    .int hexdump_module_end - hexdump_module_start

.section .rodata.strings_wasm
.global strings_module_start
strings_module_start:
    .incbin "kernel/strings.wasm"
.global strings_module_end
strings_module_end:

.global strings_module_size
strings_module_size:
    .int strings_module_end - strings_module_start

.section .rodata.file_wasm
.global file_module_start
file_module_start:
    .incbin "kernel/file.wasm"
.global file_module_end
file_module_end:

.global file_module_size
file_module_size:
    .int file_module_end - file_module_start

.section .rodata.xxd_wasm
.global xxd_module_start
xxd_module_start:
    .incbin "kernel/xxd.wasm"
.global xxd_module_end
xxd_module_end:

.global xxd_module_size
xxd_module_size:
    .int xxd_module_end - xxd_module_start

.section .rodata.less_wasm
.global less_module_start
less_module_start:
    .incbin "kernel/less.wasm"
.global less_module_end
less_module_end:

.global less_module_size
less_module_size:
    .int less_module_end - less_module_start

.section .rodata.more_wasm
.global more_module_start
more_module_start:
    .incbin "kernel/more.wasm"
.global more_module_end
more_module_end:

.global more_module_size
more_module_size:
    .int more_module_end - more_module_start

.section .rodata.join_wasm
.global join_module_start
join_module_start:
    .incbin "kernel/join.wasm"
.global join_module_end
join_module_end:

.global join_module_size
join_module_size:
    .int join_module_end - join_module_start

.section .rodata.split_wasm
.global split_module_start
split_module_start:
    .incbin "kernel/split.wasm"
.global split_module_end
split_module_end:

.global split_module_size
split_module_size:
    .int split_module_end - split_module_start

.section .rodata.patch_wasm
.global patch_module_start
patch_module_start:
    .incbin "kernel/patch.wasm"
.global patch_module_end
patch_module_end:

.global patch_module_size
patch_module_size:
    .int patch_module_end - patch_module_start

.section .rodata.diff3_wasm
.global diff3_module_start
diff3_module_start:
    .incbin "kernel/diff3.wasm"
.global diff3_module_end
diff3_module_end:

.global diff3_module_size
diff3_module_size:
    .int diff3_module_end - diff3_module_start

.section .rodata.tsort_wasm
.global tsort_module_start
tsort_module_start:
    .incbin "kernel/tsort.wasm"
.global tsort_module_end
tsort_module_end:

.global tsort_module_size
tsort_module_size:
    .int tsort_module_end - tsort_module_start

.section .rodata.csplit_wasm
.global csplit_module_start
csplit_module_start:
    .incbin "kernel/csplit.wasm"
.global csplit_module_end
csplit_module_end:

.global csplit_module_size
csplit_module_size:
    .int csplit_module_end - csplit_module_start

.section .rodata.fmt_wasm
.global fmt_module_start
fmt_module_start:
    .incbin "kernel/fmt.wasm"
.global fmt_module_end
fmt_module_end:

.global fmt_module_size
fmt_module_size:
    .int fmt_module_end - fmt_module_start

.section .rodata.pr_wasm
.global pr_module_start
pr_module_start:
    .incbin "kernel/pr.wasm"
.global pr_module_end
pr_module_end:

.global pr_module_size
pr_module_size:
    .int pr_module_end - pr_module_start

.section .rodata.column_wasm
.global column_module_start
column_module_start:
    .incbin "kernel/column.wasm"
.global column_module_end
column_module_end:

.global column_module_size
column_module_size:
    .int column_module_end - column_module_start

.section .rodata.expand_wasm
.global expand_module_start
expand_module_start:
    .incbin "kernel/expand.wasm"
.global expand_module_end
expand_module_end:

.global expand_module_size
expand_module_size:
    .int expand_module_end - expand_module_start

.section .rodata.unexpand_wasm
.global unexpand_module_start
unexpand_module_start:
    .incbin "kernel/unexpand.wasm"
.global unexpand_module_end
unexpand_module_end:

.global unexpand_module_size
unexpand_module_size:
    .int unexpand_module_end - unexpand_module_start

.section .rodata.numfmt_wasm
.global numfmt_module_start
numfmt_module_start:
    .incbin "kernel/numfmt.wasm"
.global numfmt_module_end
numfmt_module_end:

.global numfmt_module_size
numfmt_module_size:
    .int numfmt_module_end - numfmt_module_start

.section .rodata.nproc_wasm
.global nproc_module_start
nproc_module_start:
    .incbin "kernel/nproc.wasm"
.global nproc_module_end
nproc_module_end:

.global nproc_module_size
nproc_module_size:
    .int nproc_module_end - nproc_module_start

.section .rodata.hostid_wasm
.global hostid_module_start
hostid_module_start:
    .incbin "kernel/hostid.wasm"
.global hostid_module_end
hostid_module_end:

.global hostid_module_size
hostid_module_size:
    .int hostid_module_end - hostid_module_start

.section .rodata.sync_wasm
.global sync_module_start
sync_module_start:
    .incbin "kernel/sync.wasm"
.global sync_module_end
sync_module_end:

.global sync_module_size
sync_module_size:
    .int sync_module_end - sync_module_start

.section .rodata.link_wasm
.global link_module_start
link_module_start:
    .incbin "kernel/link.wasm"
.global link_module_end
link_module_end:

.global link_module_size
link_module_size:
    .int link_module_end - link_module_start

.section .rodata.unlink_wasm
.global unlink_module_start
unlink_module_start:
    .incbin "kernel/unlink.wasm"
.global unlink_module_end
unlink_module_end:

.global unlink_module_size
unlink_module_size:
    .int unlink_module_end - unlink_module_start

.section .rodata.logname_wasm
.global logname_module_start
logname_module_start:
    .incbin "kernel/logname.wasm"
.global logname_module_end
logname_module_end:

.global logname_module_size
logname_module_size:
    .int logname_module_end - logname_module_start

.section .rodata.arch_wasm
.global arch_module_start
arch_module_start:
    .incbin "kernel/arch.wasm"
.global arch_module_end
arch_module_end:

.global arch_module_size
arch_module_size:
    .int arch_module_end - arch_module_start

.section .rodata.setarch_wasm
.global setarch_module_start
setarch_module_start:
    .incbin "kernel/setarch.wasm"
.global setarch_module_end
setarch_module_end:

.global setarch_module_size
setarch_module_size:
    .int setarch_module_end - setarch_module_start

.section .rodata.nice_wasm
.global nice_module_start
nice_module_start:
    .incbin "kernel/nice.wasm"
.global nice_module_end
nice_module_end:

.global nice_module_size
nice_module_size:
    .int nice_module_end - nice_module_start

.section .rodata.renice_wasm
.global renice_module_start
renice_module_start:
    .incbin "kernel/renice.wasm"
.global renice_module_end
renice_module_end:

.global renice_module_size
renice_module_size:
    .int renice_module_end - renice_module_start

.section .rodata.timeout_wasm
.global timeout_module_start
timeout_module_start:
    .incbin "kernel/timeout.wasm"
.global timeout_module_end
timeout_module_end:

.global timeout_module_size
timeout_module_size:
    .int timeout_module_end - timeout_module_start

.section .rodata.stdbuf_wasm
.global stdbuf_module_start
stdbuf_module_start:
    .incbin "kernel/stdbuf.wasm"
.global stdbuf_module_end
stdbuf_module_end:
.global stdbuf_module_size
stdbuf_module_size:
    .int stdbuf_module_end - stdbuf_module_start

.section .rodata.factor_wasm
.global factor_module_start
factor_module_start:
    .incbin "kernel/factor.wasm"
.global factor_module_end
factor_module_end:
.global factor_module_size
factor_module_size:
    .int factor_module_end - factor_module_start

.section .rodata.seq_wasm
.global seq_module_start
seq_module_start:
    .incbin "kernel/seq.wasm"
.global seq_module_end
seq_module_end:
.global seq_module_size
seq_module_size:
    .int seq_module_end - seq_module_start

.section .rodata.realpath_wasm
.global realpath_module_start
realpath_module_start:
    .incbin "kernel/realpath.wasm"
.global realpath_module_end
realpath_module_end:
.global realpath_module_size
realpath_module_size:
    .int realpath_module_end - realpath_module_start

.section .rodata.groups_wasm
.global groups_module_start
groups_module_start:
    .incbin "kernel/groups.wasm"
.global groups_module_end
groups_module_end:
.global groups_module_size
groups_module_size:
    .int groups_module_end - groups_module_start

.section .rodata.install_wasm
.global install_module_start
install_module_start:
    .incbin "kernel/install.wasm"
.global install_module_end
install_module_end:
.global install_module_size
install_module_size:
    .int install_module_end - install_module_start

.section .rodata.pathchk_wasm
.global pathchk_module_start
pathchk_module_start:
    .incbin "kernel/pathchk.wasm"
.global pathchk_module_end
pathchk_module_end:
.global pathchk_module_size
pathchk_module_size:
    .int pathchk_module_end - pathchk_module_start

.section .rodata.mktemp_wasm
.global mktemp_module_start
mktemp_module_start:
    .incbin "kernel/mktemp.wasm"
.global mktemp_module_end
mktemp_module_end:
.global mktemp_module_size
mktemp_module_size:
    .int mktemp_module_end - mktemp_module_start

.section .rodata.truncate_wasm
.global truncate_module_start
truncate_module_start:
    .incbin "kernel/truncate.wasm"
.global truncate_module_end
truncate_module_end:
.global truncate_module_size
truncate_module_size:
    .int truncate_module_end - truncate_module_start

.section .rodata.nohup_wasm
.global nohup_module_start
nohup_module_start:
    .incbin "kernel/nohup.wasm"
.global nohup_module_end
nohup_module_end:
.global nohup_module_size
nohup_module_size:
    .int nohup_module_end - nohup_module_start

.section .rodata.envsubst_wasm
.global envsubst_module_start
envsubst_module_start:
    .incbin "kernel/envsubst.wasm"
.global envsubst_module_end
envsubst_module_end:
.global envsubst_module_size
envsubst_module_size:
    .int envsubst_module_end - envsubst_module_start

.section .rodata.dircolors_wasm
.global dircolors_module_start
dircolors_module_start:
    .incbin "kernel/dircolors.wasm"
.global dircolors_module_end
dircolors_module_end:
.global dircolors_module_size
dircolors_module_size:
    .int dircolors_module_end - dircolors_module_start

.section .rodata.expr_wasm
.global expr_module_start
expr_module_start:
    .incbin "kernel/expr.wasm"
.global expr_module_end
expr_module_end:
.global expr_module_size
expr_module_size:
    .int expr_module_end - expr_module_start

.section .rodata.test_cmd_wasm
.global test_cmd_module_start
test_cmd_module_start:
    .incbin "kernel/test_cmd.wasm"
.global test_cmd_module_end
test_cmd_module_end:
.global test_cmd_module_size
test_cmd_module_size:
    .int test_cmd_module_end - test_cmd_module_start

.section .rodata.dd_wasm
.global dd_module_start
dd_module_start:
    .incbin "kernel/dd.wasm"
.global dd_module_end
dd_module_end:
.global dd_module_size
dd_module_size:
    .int dd_module_end - dd_module_start

.section .rodata.hashsum_wasm
.global hashsum_module_start
hashsum_module_start:
    .incbin "kernel/hashsum.wasm"
.global hashsum_module_end
hashsum_module_end:
.global hashsum_module_size
hashsum_module_size:
    .int hashsum_module_end - hashsum_module_start

.section .rodata.watch_wasm
.global watch_module_start
watch_module_start:
    .incbin "kernel/watch.wasm"
.global watch_module_end
watch_module_end:
.global watch_module_size
watch_module_size:
    .int watch_module_end - watch_module_start

.section .rodata.wasm_dis_wasm
.global wasm_dis_module_start
wasm_dis_module_start:
    .incbin "kernel/wasm_dis.wasm"
.global wasm_dis_module_end
wasm_dis_module_end:
.global wasm_dis_module_size
wasm_dis_module_size:
    .int wasm_dis_module_end - wasm_dis_module_start

.section .rodata.shred_wasm
.global shred_module_start
shred_module_start:
    .incbin "kernel/shred.wasm"
.global shred_module_end
shred_module_end:
.global shred_module_size
shred_module_size:
    .int shred_module_end - shred_module_start

.section .rodata.stat_wasm
.global stat_module_start
stat_module_start:
    .incbin "kernel/stat.wasm"
.global stat_module_end
stat_module_end:
.global stat_module_size
stat_module_size:
    .int stat_module_end - stat_module_start

.section .rodata.mcookie_wasm
.global mcookie_module_start
mcookie_module_start:
    .incbin "kernel/mcookie.wasm"
.global mcookie_module_end
mcookie_module_end:
.global mcookie_module_size
mcookie_module_size:
    .int mcookie_module_end - mcookie_module_start

.section .rodata.lsof_wasm
.global lsof_module_start
lsof_module_start:
    .incbin "kernel/lsof.wasm"
.global lsof_module_end
lsof_module_end:
.global lsof_module_size
lsof_module_size:
    .int lsof_module_end - lsof_module_start

.section .rodata.iostat_wasm
.global iostat_module_start
iostat_module_start:
    .incbin "kernel/iostat.wasm"
.global iostat_module_end
iostat_module_end:
.global iostat_module_size
iostat_module_size:
    .int iostat_module_end - iostat_module_start

.section .rodata.vmstat_wasm
.global vmstat_module_start
vmstat_module_start:
    .incbin "kernel/vmstat.wasm"
.global vmstat_module_end
vmstat_module_end:
.global vmstat_module_size
vmstat_module_size:
    .int vmstat_module_end - vmstat_module_start

.section .rodata.mpstat_wasm
.global mpstat_module_start
mpstat_module_start:
    .incbin "kernel/mpstat.wasm"
.global mpstat_module_end
mpstat_module_end:
.global mpstat_module_size
mpstat_module_size:
    .int mpstat_module_end - mpstat_module_start

.section .rodata.pidof_wasm
.global pidof_module_start
pidof_module_start:
    .incbin "kernel/pidof.wasm"
.global pidof_module_end
pidof_module_end:
.global pidof_module_size
pidof_module_size:
    .int pidof_module_end - pidof_module_start

.section .rodata.pgrep_wasm
.global pgrep_module_start
pgrep_module_start:
    .incbin "kernel/pgrep.wasm"
.global pgrep_module_end
pgrep_module_end:
.global pgrep_module_size
pgrep_module_size:
    .int pgrep_module_end - pgrep_module_start

.section .rodata.pkill_wasm
.global pkill_module_start
pkill_module_start:
    .incbin "kernel/pkill.wasm"
.global pkill_module_end
pkill_module_end:
.global pkill_module_size
pkill_module_size:
    .int pkill_module_end - pkill_module_start

.section .rodata.top_wasm
.global top_module_start
top_module_start:
    .incbin "kernel/top.wasm"
.global top_module_end
top_module_end:
.global top_module_size
top_module_size:
    .int top_module_end - top_module_start

.section .rodata.htop_wasm
.global htop_module_start
htop_module_start:
    .incbin "kernel/htop.wasm"
.global htop_module_end
htop_module_end:
.global htop_module_size
htop_module_size:
    .int htop_module_end - htop_module_start

.section .rodata.strace_wasm
.global strace_module_start
strace_module_start:
    .incbin "kernel/strace.wasm"
.global strace_module_end
strace_module_end:
.global strace_module_size
strace_module_size:
    .int strace_module_end - strace_module_start

.section .rodata.ltrace_wasm
.global ltrace_module_start
ltrace_module_start:
    .incbin "kernel/ltrace.wasm"
.global ltrace_module_end
ltrace_module_end:
.global ltrace_module_size
ltrace_module_size:
    .int ltrace_module_end - ltrace_module_start

.section .rodata.tracepath_wasm
.global tracepath_module_start
tracepath_module_start:
    .incbin "kernel/tracepath.wasm"
.global tracepath_module_end
tracepath_module_end:
.global tracepath_module_size
tracepath_module_size:
    .int tracepath_module_end - tracepath_module_start

.section .rodata.ss_wasm
.global ss_module_start
ss_module_start:
    .incbin "kernel/ss.wasm"
.global ss_module_end
ss_module_end:
.global ss_module_size
ss_module_size:
    .int ss_module_end - ss_module_start

.section .rodata.ip_wasm
.global ip_module_start
ip_module_start:
    .incbin "kernel/ip.wasm"
.global ip_module_end
ip_module_end:
.global ip_module_size
ip_module_size:
    .int ip_module_end - ip_module_start

.section .rodata.killall_wasm
.global killall_module_start
killall_module_start:
    .incbin "kernel/killall.wasm"
.global killall_module_end
killall_module_end:
.global killall_module_size
killall_module_size:
    .int killall_module_end - killall_module_start

.section .rodata.nmap_wasm
.global nmap_module_start
nmap_module_start:
    .incbin "kernel/nmap.wasm"
.global nmap_module_end
nmap_module_end:
.global nmap_module_size
nmap_module_size:
    .int nmap_module_end - nmap_module_start

.section .rodata.dig_wasm
.global dig_module_start
dig_module_start:
    .incbin "kernel/dig.wasm"
.global dig_module_end
dig_module_end:
.global dig_module_size
dig_module_size:
    .int dig_module_end - dig_module_start

.section .rodata.host_cmd_wasm
.global host_cmd_module_start
host_cmd_module_start:
    .incbin "kernel/host.wasm"
.global host_cmd_module_end
host_cmd_module_end:
.global host_cmd_module_size
host_cmd_module_size:
    .int host_cmd_module_end - host_cmd_module_start

.section .rodata.arp_wasm
.global arp_module_start
arp_module_start:
    .incbin "kernel/arp.wasm"
.global arp_module_end
arp_module_end:
.global arp_module_size
arp_module_size:
    .int arp_module_end - arp_module_start

.section .rodata.iwconfig_wasm
.global iwconfig_module_start
iwconfig_module_start:
    .incbin "kernel/iwconfig.wasm"
.global iwconfig_module_end
iwconfig_module_end:
.global iwconfig_module_size
iwconfig_module_size:
    .int iwconfig_module_end - iwconfig_module_start

.section .rodata.ifup_wasm
.global ifup_module_start
ifup_module_start:
    .incbin "kernel/ifup.wasm"
.global ifup_module_end
ifup_module_end:
.global ifup_module_size
ifup_module_size:
    .int ifup_module_end - ifup_module_start

.section .rodata.nc_wasm
.global nc_module_start
nc_module_start:
    .incbin "kernel/nc.wasm"
.global nc_module_end
nc_module_end:
.global nc_module_size
nc_module_size:
    .int nc_module_end - nc_module_start

.section .rodata.iptables_wasm
.global iptables_module_start
iptables_module_start:
    .incbin "kernel/iptables.wasm"
.global iptables_module_end
iptables_module_end:
.global iptables_module_size
iptables_module_size:
    .int iptables_module_end - iptables_module_start

.section .rodata.socat_wasm
.global socat_module_start
socat_module_start:
    .incbin "kernel/socat.wasm"
.global socat_module_end
socat_module_end:
.global socat_module_size
socat_module_size:
    .int socat_module_end - socat_module_start

.section .rodata.tcpdump_wasm
.global tcpdump_module_start
tcpdump_module_start:
    .incbin "kernel/tcpdump.wasm"
.global tcpdump_module_end
tcpdump_module_end:
.global tcpdump_module_size
tcpdump_module_size:
    .int tcpdump_module_end - tcpdump_module_start

.section .rodata.ping6_wasm
.global ping6_module_start
ping6_module_start:
    .incbin "kernel/ping6.wasm"
.global ping6_module_end
ping6_module_end:
.global ping6_module_size
ping6_module_size:
    .int ping6_module_end - ping6_module_start

.section .rodata.ethtool_wasm
.global ethtool_module_start
ethtool_module_start:
    .incbin "kernel/ethtool.wasm"
.global ethtool_module_end
ethtool_module_end:
.global ethtool_module_size
ethtool_module_size:
    .int ethtool_module_end - ethtool_module_start

.section .rodata.arping_wasm
.global arping_module_start
arping_module_start:
    .incbin "kernel/arping.wasm"
.global arping_module_end
arping_module_end:
.global arping_module_size
arping_module_size:
    .int arping_module_end - arping_module_start

.section .rodata.brctl_wasm
.global brctl_module_start
brctl_module_start:
    .incbin "kernel/brctl.wasm"
.global brctl_module_end
brctl_module_end:
.global brctl_module_size
brctl_module_size:
    .int brctl_module_end - brctl_module_start

.section .rodata.ifstat_wasm
.global ifstat_module_start
ifstat_module_start:
    .incbin "kernel/ifstat.wasm"
.global ifstat_module_end
ifstat_module_end:
.global ifstat_module_size
ifstat_module_size:
    .int ifstat_module_end - ifstat_module_start

.section .rodata.mii_tool_wasm
.global mii_tool_module_start
mii_tool_module_start:
    .incbin "kernel/mii_tool.wasm"
.global mii_tool_module_end
mii_tool_module_end:
.global mii_tool_module_size
mii_tool_module_size:
    .int mii_tool_module_end - mii_tool_module_start

.section .rodata.nameif_wasm
.global nameif_module_start
nameif_module_start:
    .incbin "kernel/nameif.wasm"
.global nameif_module_end
nameif_module_end:
.global nameif_module_size
nameif_module_size:
    .int nameif_module_end - nameif_module_start

.section .rodata.plipconfig_wasm
.global plipconfig_module_start
plipconfig_module_start:
    .incbin "kernel/plipconfig.wasm"
.global plipconfig_module_end
plipconfig_module_end:
.global plipconfig_module_size
plipconfig_module_size:
    .int plipconfig_module_end - plipconfig_module_start

.section .rodata.pppd_wasm
.global pppd_module_start
pppd_module_start:
    .incbin "kernel/pppd.wasm"
.global pppd_module_end
pppd_module_end:
.global pppd_module_size
pppd_module_size:
    .int pppd_module_end - pppd_module_start

/* ---------------------------------------------------------------------------
 * WASM module: slattach (v191.0)
 * --------------------------------------------------------------------------- */
.section .rodata.slattach_wasm
.global slattach_module_start
slattach_module_start:
    .incbin "kernel/slattach.wasm"
.global slattach_module_end
slattach_module_end:
.global slattach_module_size
slattach_module_size:
    .int slattach_module_end - slattach_module_start

/* ---------------------------------------------------------------------------
 * WASM module: slipattach (v192.0)
 * --------------------------------------------------------------------------- */
.section .rodata.slipattach_wasm
.global slipattach_module_start
slipattach_module_start:
    .incbin "kernel/slipattach.wasm"
.global slipattach_module_end
slipattach_module_end:
.global slipattach_module_size
slipattach_module_size:
    .int slipattach_module_end - slipattach_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes (v193.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_wasm
.global setkeycodes_module_start
setkeycodes_module_start:
    .incbin "kernel/setkeycodes.wasm"
.global setkeycodes_module_end
setkeycodes_module_end:
.global setkeycodes_module_size
setkeycodes_module_size:
    .int setkeycodes_module_end - setkeycodes_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys (v194.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_wasm
.global loadkeys_module_start
loadkeys_module_start:
    .incbin "kernel/loadkeys.wasm"
.global loadkeys_module_end
loadkeys_module_end:
.global loadkeys_module_size
loadkeys_module_size:
    .int loadkeys_module_end - loadkeys_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys (v195.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_wasm
.global dumpkeys_module_start
dumpkeys_module_start:
    .incbin "kernel/dumpkeys.wasm"
.global dumpkeys_module_end
dumpkeys_module_end:
.global dumpkeys_module_size
dumpkeys_module_size:
    .int dumpkeys_module_end - dumpkeys_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey (v196.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_wasm
.global showkey_module_start
showkey_module_start:
    .incbin "kernel/showkey.wasm"
.global showkey_module_end
showkey_module_end:
.global showkey_module_size
showkey_module_size:
    .int showkey_module_end - showkey_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt (v197.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_wasm
.global chvt_module_start
chvt_module_start:
    .incbin "kernel/chvt.wasm"
.global chvt_module_end
chvt_module_end:
.global chvt_module_size
chvt_module_size:
    .int chvt_module_end - chvt_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt (v198.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_wasm
.global openvt_module_start
openvt_module_start:
    .incbin "kernel/openvt.wasm"
.global openvt_module_end
openvt_module_end:
.global openvt_module_size
openvt_module_size:
    .int openvt_module_end - openvt_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt (v199.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_wasm
.global deallocvt_module_start
deallocvt_module_start:
    .incbin "kernel/deallocvt.wasm"
.global deallocvt_module_end
deallocvt_module_end:
.global deallocvt_module_size
deallocvt_module_size:
    .int deallocvt_module_end - deallocvt_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole (v200.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_wasm
.global fgconsole_module_start
fgconsole_module_start:
    .incbin "kernel/fgconsole.wasm"
.global fgconsole_module_end
fgconsole_module_end:
.global fgconsole_module_size
fgconsole_module_size:
    .int fgconsole_module_end - fgconsole_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont (v201.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_wasm
.global setfont_module_start
setfont_module_start:
    .incbin "kernel/setfont.wasm"
.global setfont_module_end
setfont_module_end:
.global setfont_module_size
setfont_module_size:
    .int setfont_module_end - setfont_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode (v202.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_wasm
.global kbd_mode_module_start
kbd_mode_module_start:
    .incbin "kernel/kbd_mode.wasm"
.global kbd_mode_module_end
kbd_mode_module_end:
.global kbd_mode_module_size
kbd_mode_module_size:
    .int kbd_mode_module_end - kbd_mode_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v2 (v203.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v2_wasm
.global showkey_v2_module_start
showkey_v2_module_start:
    .incbin "kernel/showkey_v2.wasm"
.global showkey_v2_module_end
showkey_v2_module_end:
.global showkey_v2_module_size
showkey_v2_module_size:
    .int showkey_v2_module_end - showkey_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt_v2 (v204.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_v2_wasm
.global chvt_v2_module_start
chvt_v2_module_start:
    .incbin "kernel/chvt_v2.wasm"
.global chvt_v2_module_end
chvt_v2_module_end:
.global chvt_v2_module_size
chvt_v2_module_size:
    .int chvt_v2_module_end - chvt_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v2 (v205.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v2_wasm
.global openvt_v2_module_start
openvt_v2_module_start:
    .incbin "kernel/openvt_v2.wasm"
.global openvt_v2_module_end
openvt_v2_module_end:
.global openvt_v2_module_size
openvt_v2_module_size:
    .int openvt_v2_module_end - openvt_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v2 (v206.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v2_wasm
.global deallocvt_v2_module_start
deallocvt_v2_module_start:
    .incbin "kernel/deallocvt_v2.wasm"
.global deallocvt_v2_module_end
deallocvt_v2_module_end:
.global deallocvt_v2_module_size
deallocvt_v2_module_size:
    .int deallocvt_v2_module_end - deallocvt_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v2 (v207.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v2_wasm
.global fgconsole_v2_module_start
fgconsole_v2_module_start:
    .incbin "kernel/fgconsole_v2.wasm"
.global fgconsole_v2_module_end
fgconsole_v2_module_end:
.global fgconsole_v2_module_size
fgconsole_v2_module_size:
    .int fgconsole_v2_module_end - fgconsole_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v2 (v208.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v2_wasm
.global setfont_v2_module_start
setfont_v2_module_start:
    .incbin "kernel/setfont_v2.wasm"
.global setfont_v2_module_end
setfont_v2_module_end:
.global setfont_v2_module_size
setfont_v2_module_size:
    .int setfont_v2_module_end - setfont_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v2 (v209.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v2_wasm
.global kbd_mode_v2_module_start
kbd_mode_v2_module_start:
    .incbin "kernel/kbd_mode_v2.wasm"
.global kbd_mode_v2_module_end
kbd_mode_v2_module_end:
.global kbd_mode_v2_module_size
kbd_mode_v2_module_size:
    .int kbd_mode_v2_module_end - kbd_mode_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v3 (v210.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v3_wasm
.global showkey_v3_module_start
showkey_v3_module_start:
    .incbin "kernel/showkey_v3.wasm"
.global showkey_v3_module_end
showkey_v3_module_end:
.global showkey_v3_module_size
showkey_v3_module_size:
    .int showkey_v3_module_end - showkey_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt_v3 (v211.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_v3_wasm
.global chvt_v3_module_start
chvt_v3_module_start:
    .incbin "kernel/chvt_v3.wasm"
.global chvt_v3_module_end
chvt_v3_module_end:
.global chvt_v3_module_size
chvt_v3_module_size:
    .int chvt_v3_module_end - chvt_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v3 (v212.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v3_wasm
.global openvt_v3_module_start
openvt_v3_module_start:
    .incbin "kernel/openvt_v3.wasm"
.global openvt_v3_module_end
openvt_v3_module_end:
.global openvt_v3_module_size
openvt_v3_module_size:
    .int openvt_v3_module_end - openvt_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v3 (v213.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v3_wasm
.global deallocvt_v3_module_start
deallocvt_v3_module_start:
    .incbin "kernel/deallocvt_v3.wasm"
.global deallocvt_v3_module_end
deallocvt_v3_module_end:
.global deallocvt_v3_module_size
deallocvt_v3_module_size:
    .int deallocvt_v3_module_end - deallocvt_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v3 (v214.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v3_wasm
.global fgconsole_v3_module_start
fgconsole_v3_module_start:
    .incbin "kernel/fgconsole_v3.wasm"
.global fgconsole_v3_module_end
fgconsole_v3_module_end:
.global fgconsole_v3_module_size
fgconsole_v3_module_size:
    .int fgconsole_v3_module_end - fgconsole_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v3 (v215.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v3_wasm
.global setfont_v3_module_start
setfont_v3_module_start:
    .incbin "kernel/setfont_v3.wasm"
.global setfont_v3_module_end
setfont_v3_module_end:
.global setfont_v3_module_size
setfont_v3_module_size:
    .int setfont_v3_module_end - setfont_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v3 (v216.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v3_wasm
.global kbd_mode_v3_module_start
kbd_mode_v3_module_start:
    .incbin "kernel/kbd_mode_v3.wasm"
.global kbd_mode_v3_module_end
kbd_mode_v3_module_end:
.global kbd_mode_v3_module_size
kbd_mode_v3_module_size:
    .int kbd_mode_v3_module_end - kbd_mode_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v2 (v217.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v2_wasm
.global loadkeys_v2_module_start
loadkeys_v2_module_start:
    .incbin "kernel/loadkeys_v2.wasm"
.global loadkeys_v2_module_end
loadkeys_v2_module_end:
.global loadkeys_v2_module_size
loadkeys_v2_module_size:
    .int loadkeys_v2_module_end - loadkeys_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys_v2 (v218.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_v2_wasm
.global dumpkeys_v2_module_start
dumpkeys_v2_module_start:
    .incbin "kernel/dumpkeys_v2.wasm"
.global dumpkeys_v2_module_end
dumpkeys_v2_module_end:
.global dumpkeys_v2_module_size
dumpkeys_v2_module_size:
    .int dumpkeys_v2_module_end - dumpkeys_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys_v3 (v219.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_v3_wasm
.global dumpkeys_v3_module_start
dumpkeys_v3_module_start:
    .incbin "kernel/dumpkeys_v3.wasm"
.global dumpkeys_v3_module_end
dumpkeys_v3_module_end:
.global dumpkeys_v3_module_size
dumpkeys_v3_module_size:
    .int dumpkeys_v3_module_end - dumpkeys_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v4 (v220.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v4_wasm
.global showkey_v4_module_start
showkey_v4_module_start:
    .incbin "kernel/showkey_v4.wasm"
.global showkey_v4_module_end
showkey_v4_module_end:
.global showkey_v4_module_size
showkey_v4_module_size:
    .int showkey_v4_module_end - showkey_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt_v4 (v221.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_v4_wasm
.global chvt_v4_module_start
chvt_v4_module_start:
    .incbin "kernel/chvt_v4.wasm"
.global chvt_v4_module_end
chvt_v4_module_end:
.global chvt_v4_module_size
chvt_v4_module_size:
    .int chvt_v4_module_end - chvt_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v4 (v222.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v4_wasm
.global openvt_v4_module_start
openvt_v4_module_start:
    .incbin "kernel/openvt_v4.wasm"
.global openvt_v4_module_end
openvt_v4_module_end:
.global openvt_v4_module_size
openvt_v4_module_size:
    .int openvt_v4_module_end - openvt_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v4 (v223.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v4_wasm
.global deallocvt_v4_module_start
deallocvt_v4_module_start:
    .incbin "kernel/deallocvt_v4.wasm"
.global deallocvt_v4_module_end
deallocvt_v4_module_end:
.global deallocvt_v4_module_size
deallocvt_v4_module_size:
    .int deallocvt_v4_module_end - deallocvt_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v4 (v224.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v4_wasm
.global fgconsole_v4_module_start
fgconsole_v4_module_start:
    .incbin "kernel/fgconsole_v4.wasm"
.global fgconsole_v4_module_end
fgconsole_v4_module_end:
.global fgconsole_v4_module_size
fgconsole_v4_module_size:
    .int fgconsole_v4_module_end - fgconsole_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v4 (v225.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v4_wasm
.global setfont_v4_module_start
setfont_v4_module_start:
    .incbin "kernel/setfont_v4.wasm"
.global setfont_v4_module_end
setfont_v4_module_end:
.global setfont_v4_module_size
setfont_v4_module_size:
    .int setfont_v4_module_end - setfont_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v4 (v226.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v4_wasm
.global kbd_mode_v4_module_start
kbd_mode_v4_module_start:
    .incbin "kernel/kbd_mode_v4.wasm"
.global kbd_mode_v4_module_end
kbd_mode_v4_module_end:
.global kbd_mode_v4_module_size
kbd_mode_v4_module_size:
    .int kbd_mode_v4_module_end - kbd_mode_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v3 (v227.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v3_wasm
.global loadkeys_v3_module_start
loadkeys_v3_module_start:
    .incbin "kernel/loadkeys_v3.wasm"
.global loadkeys_v3_module_end
loadkeys_v3_module_end:
.global loadkeys_v3_module_size
loadkeys_v3_module_size:
    .int loadkeys_v3_module_end - loadkeys_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys_v4 (v228.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_v4_wasm
.global dumpkeys_v4_module_start
dumpkeys_v4_module_start:
    .incbin "kernel/dumpkeys_v4.wasm"
.global dumpkeys_v4_module_end
dumpkeys_v4_module_end:
.global dumpkeys_v4_module_size
dumpkeys_v4_module_size:
    .int dumpkeys_v4_module_end - dumpkeys_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes_v2 (v229.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_v2_wasm
.global setkeycodes_v2_module_start
setkeycodes_v2_module_start:
    .incbin "kernel/setkeycodes_v2.wasm"
.global setkeycodes_v2_module_end
setkeycodes_v2_module_end:
.global setkeycodes_v2_module_size
setkeycodes_v2_module_size:
    .int setkeycodes_v2_module_end - setkeycodes_v2_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v5 (v230.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v5_wasm
.global showkey_v5_module_start
showkey_v5_module_start:
    .incbin "kernel/showkey_v5.wasm"
.global showkey_v5_module_end
showkey_v5_module_end:
.global showkey_v5_module_size
showkey_v5_module_size:
    .int showkey_v5_module_end - showkey_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt_v5 (v231.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_v5_wasm
.global chvt_v5_module_start
chvt_v5_module_start:
    .incbin "kernel/chvt_v5.wasm"
.global chvt_v5_module_end
chvt_v5_module_end:
.global chvt_v5_module_size
chvt_v5_module_size:
    .int chvt_v5_module_end - chvt_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v5 (v232.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v5_wasm
.global openvt_v5_module_start
openvt_v5_module_start:
    .incbin "kernel/openvt_v5.wasm"
.global openvt_v5_module_end
openvt_v5_module_end:
.global openvt_v5_module_size
openvt_v5_module_size:
    .int openvt_v5_module_end - openvt_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v5 (v233.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v5_wasm
.global deallocvt_v5_module_start
deallocvt_v5_module_start:
    .incbin "kernel/deallocvt_v5.wasm"
.global deallocvt_v5_module_end
deallocvt_v5_module_end:
.global deallocvt_v5_module_size
deallocvt_v5_module_size:
    .int deallocvt_v5_module_end - deallocvt_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v5 (v234.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v5_wasm
.global fgconsole_v5_module_start
fgconsole_v5_module_start:
    .incbin "kernel/fgconsole_v5.wasm"
.global fgconsole_v5_module_end
fgconsole_v5_module_end:
.global fgconsole_v5_module_size
fgconsole_v5_module_size:
    .int fgconsole_v5_module_end - fgconsole_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v5 (v235.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v5_wasm
.global setfont_v5_module_start
setfont_v5_module_start:
    .incbin "kernel/setfont_v5.wasm"
.global setfont_v5_module_end
setfont_v5_module_end:
.global setfont_v5_module_size
setfont_v5_module_size:
    .int setfont_v5_module_end - setfont_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v5 (v236.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v5_wasm
.global kbd_mode_v5_module_start
kbd_mode_v5_module_start:
    .incbin "kernel/kbd_mode_v5.wasm"
.global kbd_mode_v5_module_end
kbd_mode_v5_module_end:
.global kbd_mode_v5_module_size
kbd_mode_v5_module_size:
    .int kbd_mode_v5_module_end - kbd_mode_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v4 (v237.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v4_wasm
.global loadkeys_v4_module_start
loadkeys_v4_module_start:
    .incbin "kernel/loadkeys_v4.wasm"
.global loadkeys_v4_module_end
loadkeys_v4_module_end:
.global loadkeys_v4_module_size
loadkeys_v4_module_size:
    .int loadkeys_v4_module_end - loadkeys_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys_v5 (v238.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_v5_wasm
.global dumpkeys_v5_module_start
dumpkeys_v5_module_start:
    .incbin "kernel/dumpkeys_v5.wasm"
.global dumpkeys_v5_module_end
dumpkeys_v5_module_end:
.global dumpkeys_v5_module_size
dumpkeys_v5_module_size:
    .int dumpkeys_v5_module_end - dumpkeys_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes_v3 (v239.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_v3_wasm
.global setkeycodes_v3_module_start
setkeycodes_v3_module_start:
    .incbin "kernel/setkeycodes_v3.wasm"
.global setkeycodes_v3_module_end
setkeycodes_v3_module_end:
.global setkeycodes_v3_module_size
setkeycodes_v3_module_size:
    .int setkeycodes_v3_module_end - setkeycodes_v3_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes_v4 (v240.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_v4_wasm
.global setkeycodes_v4_module_start
setkeycodes_v4_module_start:
    .incbin "kernel/setkeycodes_v4.wasm"
.global setkeycodes_v4_module_end
setkeycodes_v4_module_end:
.global setkeycodes_v4_module_size
setkeycodes_v4_module_size:
    .int setkeycodes_v4_module_end - setkeycodes_v4_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v5 (v241.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v5_wasm
.global loadkeys_v5_module_start
loadkeys_v5_module_start:
    .incbin "kernel/loadkeys_v5.wasm"
.global loadkeys_v5_module_end
loadkeys_v5_module_end:
.global loadkeys_v5_module_size
loadkeys_v5_module_size:
    .int loadkeys_v5_module_end - loadkeys_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes_v5 (v242.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_v5_wasm
.global setkeycodes_v5_module_start
setkeycodes_v5_module_start:
    .incbin "kernel/setkeycodes_v5.wasm"
.global setkeycodes_v5_module_end
setkeycodes_v5_module_end:
.global setkeycodes_v5_module_size
setkeycodes_v5_module_size:
    .int setkeycodes_v5_module_end - setkeycodes_v5_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys_v6 (v243.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_v6_wasm
.global dumpkeys_v6_module_start
dumpkeys_v6_module_start:
    .incbin "kernel/dumpkeys_v6.wasm"
.global dumpkeys_v6_module_end
dumpkeys_v6_module_end:
.global dumpkeys_v6_module_size
dumpkeys_v6_module_size:
    .int dumpkeys_v6_module_end - dumpkeys_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v6 (v244.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v6_wasm
.global showkey_v6_module_start
showkey_v6_module_start:
    .incbin "kernel/showkey_v6.wasm"
.global showkey_v6_module_end
showkey_v6_module_end:
.global showkey_v6_module_size
showkey_v6_module_size:
    .int showkey_v6_module_end - showkey_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt_v6 (v245.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_v6_wasm
.global chvt_v6_module_start
chvt_v6_module_start:
    .incbin "kernel/chvt_v6.wasm"
.global chvt_v6_module_end
chvt_v6_module_end:
.global chvt_v6_module_size
chvt_v6_module_size:
    .int chvt_v6_module_end - chvt_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v6 (v246.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v6_wasm
.global openvt_v6_module_start
openvt_v6_module_start:
    .incbin "kernel/openvt_v6.wasm"
.global openvt_v6_module_end
openvt_v6_module_end:
.global openvt_v6_module_size
openvt_v6_module_size:
    .int openvt_v6_module_end - openvt_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v6 (v247.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v6_wasm
.global deallocvt_v6_module_start
deallocvt_v6_module_start:
    .incbin "kernel/deallocvt_v6.wasm"
.global deallocvt_v6_module_end
deallocvt_v6_module_end:
.global deallocvt_v6_module_size
deallocvt_v6_module_size:
    .int deallocvt_v6_module_end - deallocvt_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v6 (v248.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v6_wasm
.global fgconsole_v6_module_start
fgconsole_v6_module_start:
    .incbin "kernel/fgconsole_v6.wasm"
.global fgconsole_v6_module_end
fgconsole_v6_module_end:
.global fgconsole_v6_module_size
fgconsole_v6_module_size:
    .int fgconsole_v6_module_end - fgconsole_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v6 (v249.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v6_wasm
.global setfont_v6_module_start
setfont_v6_module_start:
    .incbin "kernel/setfont_v6.wasm"
.global setfont_v6_module_end
setfont_v6_module_end:
.global setfont_v6_module_size
setfont_v6_module_size:
    .int setfont_v6_module_end - setfont_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v6 (v250.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v6_wasm
.global kbd_mode_v6_module_start
kbd_mode_v6_module_start:
    .incbin "kernel/kbd_mode_v6.wasm"
.global kbd_mode_v6_module_end
kbd_mode_v6_module_end:
.global kbd_mode_v6_module_size
kbd_mode_v6_module_size:
    .int kbd_mode_v6_module_end - kbd_mode_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v6 (v251.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v6_wasm
.global loadkeys_v6_module_start
loadkeys_v6_module_start:
    .incbin "kernel/loadkeys_v6.wasm"
.global loadkeys_v6_module_end
loadkeys_v6_module_end:
.global loadkeys_v6_module_size
loadkeys_v6_module_size:
    .int loadkeys_v6_module_end - loadkeys_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes_v6 (v252.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_v6_wasm
.global setkeycodes_v6_module_start
setkeycodes_v6_module_start:
    .incbin "kernel/setkeycodes_v6.wasm"
.global setkeycodes_v6_module_end
setkeycodes_v6_module_end:
.global setkeycodes_v6_module_size
setkeycodes_v6_module_size:
    .int setkeycodes_v6_module_end - setkeycodes_v6_module_start

/* ---------------------------------------------------------------------------
 * WASM module: unicode_start (v253.0)
 * --------------------------------------------------------------------------- */
.section .rodata.unicode_start_wasm
.global unicode_start_module_start
unicode_start_module_start:
    .incbin "kernel/unicode_start.wasm"
.global unicode_start_module_end
unicode_start_module_end:
.global unicode_start_module_size
unicode_start_module_size:
    .int unicode_start_module_end - unicode_start_module_start

/* ---------------------------------------------------------------------------
 * WASM module: unicode_stop (v254.0)
 * --------------------------------------------------------------------------- */
.section .rodata.unicode_stop_wasm
.global unicode_stop_module_start
unicode_stop_module_start:
    .incbin "kernel/unicode_stop.wasm"
.global unicode_stop_module_end
unicode_stop_module_end:
.global unicode_stop_module_size
unicode_stop_module_size:
    .int unicode_stop_module_end - unicode_stop_module_start

/* ---------------------------------------------------------------------------
 * WASM module: mapscrn (v255.0)
 * --------------------------------------------------------------------------- */
.section .rodata.mapscrn_wasm
.global mapscrn_module_start
mapscrn_module_start:
    .incbin "kernel/mapscrn.wasm"
.global mapscrn_module_end
mapscrn_module_end:
.global mapscrn_module_size
mapscrn_module_size:
    .int mapscrn_module_end - mapscrn_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadunimap (v256.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadunimap_wasm
.global loadunimap_module_start
loadunimap_module_start:
    .incbin "kernel/loadunimap.wasm"
.global loadunimap_module_end
loadunimap_module_end:
.global loadunimap_module_size
loadunimap_module_size:
    .int loadunimap_module_end - loadunimap_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setmetamode (v257.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setmetamode_wasm
.global setmetamode_module_start
setmetamode_module_start:
    .incbin "kernel/setmetamode.wasm"
.global setmetamode_module_end
setmetamode_module_end:
.global setmetamode_module_size
setmetamode_module_size:
    .int setmetamode_module_end - setmetamode_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v7 (v258.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v7_wasm
.global deallocvt_v7_module_start
deallocvt_v7_module_start:
    .incbin "kernel/deallocvt_v7.wasm"
.global deallocvt_v7_module_end
deallocvt_v7_module_end:
.global deallocvt_v7_module_size
deallocvt_v7_module_size:
    .int deallocvt_v7_module_end - deallocvt_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v7 (v259.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v7_wasm
.global fgconsole_v7_module_start
fgconsole_v7_module_start:
    .incbin "kernel/fgconsole_v7.wasm"
.global fgconsole_v7_module_end
fgconsole_v7_module_end:
.global fgconsole_v7_module_size
fgconsole_v7_module_size:
    .int fgconsole_v7_module_end - fgconsole_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v7 (v260.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v7_wasm
.global kbd_mode_v7_module_start
kbd_mode_v7_module_start:
    .incbin "kernel/kbd_mode_v7.wasm"
.global kbd_mode_v7_module_end
kbd_mode_v7_module_end:
.global kbd_mode_v7_module_size
kbd_mode_v7_module_size:
    .int kbd_mode_v7_module_end - kbd_mode_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v7 (v261.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v7_wasm
.global loadkeys_v7_module_start
loadkeys_v7_module_start:
    .incbin "kernel/loadkeys_v7.wasm"
.global loadkeys_v7_module_end
loadkeys_v7_module_end:
.global loadkeys_v7_module_size
loadkeys_v7_module_size:
    .int loadkeys_v7_module_end - loadkeys_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v7 (v262.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v7_wasm
.global openvt_v7_module_start
openvt_v7_module_start:
    .incbin "kernel/openvt_v7.wasm"
.global openvt_v7_module_end
openvt_v7_module_end:
.global openvt_v7_module_size
openvt_v7_module_size:
    .int openvt_v7_module_end - openvt_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v7 (v263.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v7_wasm
.global setfont_v7_module_start
setfont_v7_module_start:
    .incbin "kernel/setfont_v7.wasm"
.global setfont_v7_module_end
setfont_v7_module_end:
.global setfont_v7_module_size
setfont_v7_module_size:
    .int setfont_v7_module_end - setfont_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v7 (v264.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v7_wasm
.global showkey_v7_module_start
showkey_v7_module_start:
    .incbin "kernel/showkey_v7.wasm"
.global showkey_v7_module_end
showkey_v7_module_end:
.global showkey_v7_module_size
showkey_v7_module_size:
    .int showkey_v7_module_end - showkey_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: chvt_v7 (v265.0)
 * --------------------------------------------------------------------------- */
.section .rodata.chvt_v7_wasm
.global chvt_v7_module_start
chvt_v7_module_start:
    .incbin "kernel/chvt_v7.wasm"
.global chvt_v7_module_end
chvt_v7_module_end:
.global chvt_v7_module_size
chvt_v7_module_size:
    .int chvt_v7_module_end - chvt_v7_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v8 (v266.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v8_wasm
.global deallocvt_v8_module_start
deallocvt_v8_module_start:
    .incbin "kernel/deallocvt_v8.wasm"
.global deallocvt_v8_module_end
deallocvt_v8_module_end:
.global deallocvt_v8_module_size
deallocvt_v8_module_size:
    .int deallocvt_v8_module_end - deallocvt_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v8 (v267.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v8_wasm
.global fgconsole_v8_module_start
fgconsole_v8_module_start:
    .incbin "kernel/fgconsole_v8.wasm"
.global fgconsole_v8_module_end
fgconsole_v8_module_end:
.global fgconsole_v8_module_size
fgconsole_v8_module_size:
    .int fgconsole_v8_module_end - fgconsole_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: kbd_mode_v8 (v268.0)
 * --------------------------------------------------------------------------- */
.section .rodata.kbd_mode_v8_wasm
.global kbd_mode_v8_module_start
kbd_mode_v8_module_start:
    .incbin "kernel/kbd_mode_v8.wasm"
.global kbd_mode_v8_module_end
kbd_mode_v8_module_end:
.global kbd_mode_v8_module_size
kbd_mode_v8_module_size:
    .int kbd_mode_v8_module_end - kbd_mode_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v8 (v269.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v8_wasm
.global loadkeys_v8_module_start
loadkeys_v8_module_start:
    .incbin "kernel/loadkeys_v8.wasm"
.global loadkeys_v8_module_end
loadkeys_v8_module_end:
.global loadkeys_v8_module_size
loadkeys_v8_module_size:
    .int loadkeys_v8_module_end - loadkeys_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v8 (v270.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v8_wasm
.global openvt_v8_module_start
openvt_v8_module_start:
    .incbin "kernel/openvt_v8.wasm"
.global openvt_v8_module_end
openvt_v8_module_end:
.global openvt_v8_module_size
openvt_v8_module_size:
    .int openvt_v8_module_end - openvt_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setfont_v8 (v271.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setfont_v8_wasm
.global setfont_v8_module_start
setfont_v8_module_start:
    .incbin "kernel/setfont_v8.wasm"
.global setfont_v8_module_end
setfont_v8_module_end:
.global setfont_v8_module_size
setfont_v8_module_size:
    .int setfont_v8_module_end - setfont_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: showkey_v8 (v272.0)
 * --------------------------------------------------------------------------- */
.section .rodata.showkey_v8_wasm
.global showkey_v8_module_start
showkey_v8_module_start:
    .incbin "kernel/showkey_v8.wasm"
.global showkey_v8_module_end
showkey_v8_module_end:
.global showkey_v8_module_size
showkey_v8_module_size:
    .int showkey_v8_module_end - showkey_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setkeycodes_v8 (v273.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setkeycodes_v8_wasm
.global setkeycodes_v8_module_start
setkeycodes_v8_module_start:
    .incbin "kernel/setkeycodes_v8.wasm"
.global setkeycodes_v8_module_end
setkeycodes_v8_module_end:
.global setkeycodes_v8_module_size
setkeycodes_v8_module_size:
    .int setkeycodes_v8_module_end - setkeycodes_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: dumpkeys_v8 (v274.0)
 * --------------------------------------------------------------------------- */
.section .rodata.dumpkeys_v8_wasm
.global dumpkeys_v8_module_start
dumpkeys_v8_module_start:
    .incbin "kernel/dumpkeys_v8.wasm"
.global dumpkeys_v8_module_end
dumpkeys_v8_module_end:
.global dumpkeys_v8_module_size
dumpkeys_v8_module_size:
    .int dumpkeys_v8_module_end - dumpkeys_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: mapscrn_v8 (v275.0)
 * --------------------------------------------------------------------------- */
.section .rodata.mapscrn_v8_wasm
.global mapscrn_v8_module_start
mapscrn_v8_module_start:
    .incbin "kernel/mapscrn_v8.wasm"
.global mapscrn_v8_module_end
mapscrn_v8_module_end:
.global mapscrn_v8_module_size
mapscrn_v8_module_size:
    .int mapscrn_v8_module_end - mapscrn_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadunimap_v8 (v276.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadunimap_v8_wasm
.global loadunimap_v8_module_start
loadunimap_v8_module_start:
    .incbin "kernel/loadunimap_v8.wasm"
.global loadunimap_v8_module_end
loadunimap_v8_module_end:
.global loadunimap_v8_module_size
loadunimap_v8_module_size:
    .int loadunimap_v8_module_end - loadunimap_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: setmetamode_v8 (v277.0)
 * --------------------------------------------------------------------------- */
.section .rodata.setmetamode_v8_wasm
.global setmetamode_v8_module_start
setmetamode_v8_module_start:
    .incbin "kernel/setmetamode_v8.wasm"
.global setmetamode_v8_module_end
setmetamode_v8_module_end:
.global setmetamode_v8_module_size
setmetamode_v8_module_size:
    .int setmetamode_v8_module_end - setmetamode_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: unicode_start_v8 (v278.0)
 * --------------------------------------------------------------------------- */
.section .rodata.unicode_start_v8_wasm
.global unicode_start_v8_module_start
unicode_start_v8_module_start:
    .incbin "kernel/unicode_start_v8.wasm"
.global unicode_start_v8_module_end
unicode_start_v8_module_end:
.global unicode_start_v8_module_size
unicode_start_v8_module_size:
    .int unicode_start_v8_module_end - unicode_start_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: unicode_stop_v8 (v281.0)
 * --------------------------------------------------------------------------- */
.section .rodata.unicode_stop_v8_wasm
.global unicode_stop_v8_module_start
unicode_stop_v8_module_start:
    .incbin "kernel/unicode_stop_v8.wasm"
.global unicode_stop_v8_module_end
unicode_stop_v8_module_end:
.global unicode_stop_v8_module_size
unicode_stop_v8_module_size:
    .int unicode_stop_v8_module_end - unicode_stop_v8_module_start

/* ---------------------------------------------------------------------------
 * WASM module: loadkeys_v9 (v282.0)
 * --------------------------------------------------------------------------- */
.section .rodata.loadkeys_v9_wasm
.global loadkeys_v9_module_start
loadkeys_v9_module_start:
    .incbin "kernel/loadkeys_v9.wasm"
.global loadkeys_v9_module_end
loadkeys_v9_module_end:
.global loadkeys_v9_module_size
loadkeys_v9_module_size:
    .int loadkeys_v9_module_end - loadkeys_v9_module_start

/* ---------------------------------------------------------------------------
 * WASM module: openvt_v9 (v283.0)
 * --------------------------------------------------------------------------- */
.section .rodata.openvt_v9_wasm
.global openvt_v9_module_start
openvt_v9_module_start:
    .incbin "kernel/openvt_v9.wasm"
.global openvt_v9_module_end
openvt_v9_module_end:
.global openvt_v9_module_size
openvt_v9_module_size:
    .int openvt_v9_module_end - openvt_v9_module_start

/* ---------------------------------------------------------------------------
 * WASM module: deallocvt_v9 (v284.0)
 * --------------------------------------------------------------------------- */
.section .rodata.deallocvt_v9_wasm
.global deallocvt_v9_module_start
deallocvt_v9_module_start:
    .incbin "kernel/deallocvt_v9.wasm"
.global deallocvt_v9_module_end
deallocvt_v9_module_end:
.global deallocvt_v9_module_size
deallocvt_v9_module_size:
    .int deallocvt_v9_module_end - deallocvt_v9_module_start

/* ---------------------------------------------------------------------------
 * WASM module: fgconsole_v9 (v285.0)
 * --------------------------------------------------------------------------- */
.section .rodata.fgconsole_v9_wasm
.global fgconsole_v9_module_start
fgconsole_v9_module_start:
    .incbin "kernel/fgconsole_v9.wasm"
.global fgconsole_v9_module_end
fgconsole_v9_module_end:
.global fgconsole_v9_module_size
fgconsole_v9_module_size:
    .int fgconsole_v9_module_end - fgconsole_v9_module_start

.section .rodata.kbd_mode_v9_wasm
.global kbd_mode_v9_module_start
kbd_mode_v9_module_start:
    .incbin "kernel/kbd_mode_v9.wasm"
.global kbd_mode_v9_module_end
kbd_mode_v9_module_end:
.global kbd_mode_v9_module_size
kbd_mode_v9_module_size:
    .int kbd_mode_v9_module_end - kbd_mode_v9_module_start

.section .rodata.loadkeys_v10_wasm
.global loadkeys_v10_module_start
loadkeys_v10_module_start:
    .incbin "kernel/loadkeys_v10.wasm"
.global loadkeys_v10_module_end
loadkeys_v10_module_end:
.global loadkeys_v10_module_size
loadkeys_v10_module_size:
    .int loadkeys_v10_module_end - loadkeys_v10_module_start

.section .rodata.openvt_v10_wasm
.global openvt_v10_module_start
openvt_v10_module_start:
    .incbin "kernel/openvt_v10.wasm"
.global openvt_v10_module_end
openvt_v10_module_end:
.global openvt_v10_module_size
openvt_v10_module_size:
    .int openvt_v10_module_end - openvt_v10_module_start

.section .rodata.deallocvt_v10_wasm
.global deallocvt_v10_module_start
deallocvt_v10_module_start:
    .incbin "kernel/deallocvt_v10.wasm"
.global deallocvt_v10_module_end
deallocvt_v10_module_end:
.global deallocvt_v10_module_size
deallocvt_v10_module_size:
    .int deallocvt_v10_module_end - deallocvt_v10_module_start

.section .rodata.fgconsole_v10_wasm
.global fgconsole_v10_module_start
fgconsole_v10_module_start:
    .incbin "kernel/fgconsole_v10.wasm"
.global fgconsole_v10_module_end
fgconsole_v10_module_end:
.global fgconsole_v10_module_size
fgconsole_v10_module_size:
    .int fgconsole_v10_module_end - fgconsole_v10_module_start

.section .rodata.setfont_v10_wasm
.global setfont_v10_module_start
setfont_v10_module_start:
    .incbin "kernel/setfont_v10.wasm"
.global setfont_v10_module_end
setfont_v10_module_end:
.global setfont_v10_module_size
setfont_v10_module_size:
    .int setfont_v10_module_end - setfont_v10_module_start

.section .rodata.kbd_mode_v10_wasm
.global kbd_mode_v10_module_start
kbd_mode_v10_module_start:
    .incbin "kernel/kbd_mode_v10.wasm"
.global kbd_mode_v10_module_end
kbd_mode_v10_module_end:
.global kbd_mode_v10_module_size
kbd_mode_v10_module_size:
    .int kbd_mode_v10_module_end - kbd_mode_v10_module_start

.section .rodata.loadkeys_v11_wasm
.global loadkeys_v11_module_start
loadkeys_v11_module_start:
    .incbin "kernel/loadkeys_v11.wasm"
.global loadkeys_v11_module_end
loadkeys_v11_module_end:
.global loadkeys_v11_module_size
loadkeys_v11_module_size:
    .int loadkeys_v11_module_end - loadkeys_v11_module_start

.section .rodata.openvt_v11_wasm
.global openvt_v11_module_start
openvt_v11_module_start:
    .incbin "kernel/openvt_v11.wasm"
.global openvt_v11_module_end
openvt_v11_module_end:
.global openvt_v11_module_size
openvt_v11_module_size:
    .int openvt_v11_module_end - openvt_v11_module_start

.section .rodata.deallocvt_v11_wasm
.global deallocvt_v11_module_start
deallocvt_v11_module_start:
    .incbin "kernel/deallocvt_v11.wasm"
.global deallocvt_v11_module_end
deallocvt_v11_module_end:
.global deallocvt_v11_module_size
deallocvt_v11_module_size:
    .int deallocvt_v11_module_end - deallocvt_v11_module_start

.section .rodata.fgconsole_v11_wasm
.global fgconsole_v11_module_start
fgconsole_v11_module_start:
    .incbin "kernel/fgconsole_v11.wasm"
.global fgconsole_v11_module_end
fgconsole_v11_module_end:
.global fgconsole_v11_module_size
fgconsole_v11_module_size:
    .int fgconsole_v11_module_end - fgconsole_v11_module_start

.section .rodata.setfont_v11_wasm
.global setfont_v11_module_start
setfont_v11_module_start:
    .incbin "kernel/setfont_v11.wasm"
.global setfont_v11_module_end
setfont_v11_module_end:
.global setfont_v11_module_size
setfont_v11_module_size:
    .int setfont_v11_module_end - setfont_v11_module_start

.section .rodata.kbd_mode_v11_wasm
.global kbd_mode_v11_module_start
kbd_mode_v11_module_start:
    .incbin "kernel/kbd_mode_v11.wasm"
.global kbd_mode_v11_module_end
kbd_mode_v11_module_end:
.global kbd_mode_v11_module_size
kbd_mode_v11_module_size:
    .int kbd_mode_v11_module_end - kbd_mode_v11_module_start

.section .rodata.loadkeys_v12_wasm
.global loadkeys_v12_module_start
loadkeys_v12_module_start:
    .incbin "kernel/loadkeys_v12.wasm"
.global loadkeys_v12_module_end
loadkeys_v12_module_end:
.global loadkeys_v12_module_size
loadkeys_v12_module_size:
    .int loadkeys_v12_module_end - loadkeys_v12_module_start

.section .rodata.openvt_v12_wasm
.global openvt_v12_module_start
openvt_v12_module_start:
    .incbin "kernel/openvt_v12.wasm"
.global openvt_v12_module_end
openvt_v12_module_end:
.global openvt_v12_module_size
openvt_v12_module_size:
    .int openvt_v12_module_end - openvt_v12_module_start

.section .rodata.deallocvt_v12_wasm
.global deallocvt_v12_module_start
deallocvt_v12_module_start:
    .incbin "kernel/deallocvt_v12.wasm"
.global deallocvt_v12_module_end
deallocvt_v12_module_end:
.global deallocvt_v12_module_size
deallocvt_v12_module_size:
    .int deallocvt_v12_module_end - deallocvt_v12_module_start

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
