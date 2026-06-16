/* huge_pages: Huge page memory support (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define PAGE_SIZE_4K    4096
#define PAGE_SIZE_2M    (2*1024*1024)
#define PAGE_SIZE_1G    (1024*1024*1024)
#define MAX_HUGE_PAGES  64
#define HP_STATE_FREE   0
#define HP_STATE_ALLOC  1
typedef struct { unsigned int size; unsigned int addr; int state; int order; } huge_page_t;
static huge_page_t pages[MAX_HUGE_PAGES]; static int alloc_count = 0;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
static void phex(unsigned int v) { char b[16]; const char *h="0123456789ABCDEF"; int p=0; b[p++]='0'; b[p++]='x'; for(int i=7;i>=0;i--) b[p++]=h[(v>>(i*4))&0xF]; b[p]='\0'; host_print(b); }
int hp_init(void) {
    for(int i=0;i<MAX_HUGE_PAGES;i++) { pages[i].size = PAGE_SIZE_2M; pages[i].addr = 0x10000000 + i*PAGE_SIZE_2M; pages[i].state = HP_STATE_FREE; pages[i].order = 9; }
    pstr("  Huge pages initialized: "); pint(MAX_HUGE_PAGES); pstr(" x 2MB\n"); return 0;
}
int hp_alloc(int order) {
    unsigned int size = PAGE_SIZE_4K;
    for(int i=0;i<order;i++) size *= 2;
    for(int i=0;i<MAX_HUGE_PAGES;i++) if(pages[i].state==HP_STATE_FREE && pages[i].size>=size) {
        pages[i].state = HP_STATE_ALLOC; pages[i].order = order; alloc_count++;
        pstr("  Allocated: "); phex(pages[i].addr); pstr(" size="); pint(size); pstr(" (order "); pint(order); pstr(")\n"); return pages[i].addr;
    }
    return 0;
}
int hp_free(unsigned int addr) {
    for(int i=0;i<MAX_HUGE_PAGES;i++) if(pages[i].addr==addr && pages[i].state==HP_STATE_ALLOC) {
        pages[i].state = HP_STATE_FREE; alloc_count--;
        pstr("  Freed: "); phex(addr); pstr("\n"); return 0;
    }
    return -1;
}
int hp_stats(void) {
    int free=MAX_HUGE_PAGES-alloc_count;
    pstr("  Huge page stats:\n");
    pstr("    Total: "); pint(MAX_HUGE_PAGES); pstr("\n");
    pstr("    Allocated: "); pint(alloc_count); pstr("\n");
    pstr("    Free: "); pint(free); pstr("\n");
    pstr("    Page size: 2MB\n");
    pstr("    Total memory: "); pint(MAX_HUGE_PAGES*2); pstr("MB\n"); return 0;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("Huge Pages v46.0 - 2MB/1GB Page Support\n");
    if(help) { pstr("Usage: huge_pages [-h|-t]\n"); return; }
    if(test) {
        pstr("=== Huge Pages Test ===\n\n");
        pstr("[1] Init\n"); hp_init(); pstr("\n");
        pstr("[2] Alloc\n"); unsigned int a1=hp_alloc(9); unsigned int a2=hp_alloc(9); unsigned int a3=hp_alloc(10); pstr("\n");
        pstr("[3] Stats\n"); hp_stats(); pstr("\n");
        pstr("[4] Free\n"); hp_free(a1); hp_free(a2); pstr("\n");
        pstr("[5] Final\n"); hp_stats();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
