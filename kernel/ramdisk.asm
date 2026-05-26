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
