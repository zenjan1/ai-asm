/* false: exit with status 1 */
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);

__attribute__((export_name("_start")))
void _start(void)
{
    host_exit(1);
}
