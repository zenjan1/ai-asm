/* wayland_proto: Wayland display protocol (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define MAX_SURFACES 32
#define MAX_OUTPUTS 4
#define WL_STATE_HIDDEN 0
#define WL_STATE_VISIBLE 1
typedef struct { int id; int x, y, width, height; int state; int buffer_id; char role[16]; } wl_surface_t;
typedef struct { int id; int x, y, width, height; int refresh_rate; char name[16]; } wl_output_t;
static wl_surface_t surfaces[MAX_SURFACES]; static int surface_count = 0;
static wl_output_t outputs[MAX_OUTPUTS]; static int output_count = 0;
static int next_surface_id = 1;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) { int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0'; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
int wl_init_display(void) {
    outputs[0].id = 1; outputs[0].x = 0; outputs[0].y = 0;
    outputs[0].width = 1920; outputs[0].height = 1080;
    outputs[0].refresh_rate = 60;
    my_strncpy(outputs[0].name, "HDMI-1", 15);
    output_count = 1;
    pstr("  Wayland display initialized\n");
    pstr("    Output: "); pstr(outputs[0].name); pstr(" ");
    pint(outputs[0].width); pstr("x"); pint(outputs[0].height);
    pstr("@"); pint(outputs[0].refresh_rate); pstr("Hz\n");
    return 0;
}
int wl_create_surface(int x, int y, int w, int h, const char *role) {
    if(surface_count>=MAX_SURFACES) return -1;
    int idx = surface_count++;
    surfaces[idx].id = next_surface_id++;
    surfaces[idx].x = x; surfaces[idx].y = y;
    surfaces[idx].width = w; surfaces[idx].height = h;
    surfaces[idx].state = WL_STATE_HIDDEN;
    my_strncpy(surfaces[idx].role, role, 15);
    pstr("  Created surface "); pint(surfaces[idx].id);
    pstr(" ("); pstr(role); pstr(") ");
    pint(w); pstr("x"); pint(h); pstr(" at ");
    pint(x); pstr(","); pint(y); pstr("\n");
    return surfaces[idx].id;
}
int wl_show_surface(int surface_id) {
    for(int i=0;i<MAX_SURFACES;i++) if(surfaces[i].id==surface_id) {
        surfaces[i].state = WL_STATE_VISIBLE;
        pstr("  Surface "); pint(surface_id); pstr(" shown\n");
        return 0;
    }
    return -1;
}
int wl_hide_surface(int surface_id) {
    for(int i=0;i<MAX_SURFACES;i++) if(surfaces[i].id==surface_id) {
        surfaces[i].state = WL_STATE_HIDDEN;
        pstr("  Surface "); pint(surface_id); pstr(" hidden\n");
        return 0;
    }
    return -1;
}
int wl_commit(int surface_id) {
    for(int i=0;i<MAX_SURFACES;i++) if(surfaces[i].id==surface_id) {
        pstr("  Surface "); pint(surface_id); pstr(" committed\n");
        return 0;
    }
    return -1;
}
int wl_list_surfaces(void) {
    pstr("  Surfaces:\n");
    for(int i=0;i<MAX_SURFACES;i++) if(surfaces[i].id!=0) {
        pstr("    ["); pint(surfaces[i].id); pstr("] ");
        pstr(surfaces[i].role); pstr(" ");
        pint(surfaces[i].width); pstr("x"); pint(surfaces[i].height);
        pstr(" state="); pint(surfaces[i].state); pstr("\n");
    }
    return surface_count;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("Wayland Protocol v46.0\n");
    if(help) { pstr("Usage: wayland_proto [-h|-t]\n"); return; }
    if(test) {
        pstr("=== Wayland Test ===\n\n");
        pstr("[1] Init display\n"); wl_init_display(); pstr("\n");
        pstr("[2] Create surfaces\n"); int s1=wl_create_surface(100, 100, 800, 600, "window"); int s2=wl_create_surface(200, 200, 400, 300, "popup"); int s3=wl_create_surface(0, 0, 1920, 48, "panel"); pstr("\n");
        pstr("[3] Show\n"); wl_show_surface(s1); wl_show_surface(s3); pstr("\n");
        pstr("[4] List\n"); wl_list_surfaces(); pstr("\n");
        pstr("[5] Commit\n"); wl_commit(s1); wl_commit(s2); pstr("\n");
        pstr("[6] Hide\n"); wl_hide_surface(s2);
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
