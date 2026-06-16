/* touch_input: Touch gesture recognition (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define MAX_TOUCH_POINTS 10
#define GESTURE_NONE 0
#define GESTURE_TAP 1
#define GESTURE_SWIPE 2
#define GESTURE_PINCH 3
#define GESTURE_ROTATE 4
#define GESTURE_LONG_PRESS 5
typedef struct { int id; int x, y; int active; unsigned int timestamp; } touch_point_t;
typedef struct { int type; int start_x, start_y; int end_x, end_y; float scale; float angle; } gesture_t;
static touch_point_t touches[MAX_TOUCH_POINTS];
static int active_touches = 0;
static gesture_t last_gesture;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
static void pfloat(float v) { char b[32]; int i=(int)v; int f=(int)((v-i)*100); if(f<0)f=-f; char ib[16]; int p=0; if(i<0){b[p++]='-';i=-i;} if(i==0)ib[p++]='0'; else{int d=0,t=i;while(t>0){d++;t/=10;}p+=d;while(i>0){ib[--d]='0'+(i%10);i/=10;}} for(int j=0;j<p;j++)b[j]=ib[j]; b[p++]='.'; b[p++]='0'+(f/10); b[p++]='0'+(f%10); b[p]='\0'; host_print(b); }
int touch_down(int id, int x, int y) {
    if(id>=MAX_TOUCH_POINTS) return -1;
    touches[id].id = id; touches[id].x = x; touches[id].y = y;
    touches[id].active = 1; touches[id].timestamp = 1000;
    active_touches++;
    pstr("  Touch down: id="); pint(id); pstr(" at ");
    pint(x); pstr(","); pint(y); pstr("\n");
    return 0;
}
int touch_move(int id, int x, int y) {
    if(id>=MAX_TOUCH_POINTS || !touches[id].active) return -1;
    pstr("  Touch move: id="); pint(id); pstr(" to ");
    pint(x); pstr(","); pint(y); pstr("\n");
    touches[id].x = x; touches[id].y = y;
    return 0;
}
int touch_up(int id) {
    if(id>=MAX_TOUCH_POINTS || !touches[id].active) return -1;
    touches[id].active = 0; active_touches--;
    pstr("  Touch up: id="); pint(id); pstr("\n");
    return 0;
}
int gesture_detect_tap(int x, int y) {
    last_gesture.type = GESTURE_TAP;
    last_gesture.start_x = x; last_gesture.start_y = y;
    pstr("  Gesture: TAP at "); pint(x); pstr(","); pint(y); pstr("\n");
    return GESTURE_TAP;
}
int gesture_detect_swipe(int x1, int y1, int x2, int y2) {
    last_gesture.type = GESTURE_SWIPE;
    last_gesture.start_x = x1; last_gesture.start_y = y1;
    last_gesture.end_x = x2; last_gesture.end_y = y2;
    pstr("  Gesture: SWIPE from "); pint(x1); pstr(","); pint(y1);
    pstr(" to "); pint(x2); pstr(","); pint(y2); pstr("\n");
    return GESTURE_SWIPE;
}
int gesture_detect_pinch(float scale) {
    last_gesture.type = GESTURE_PINCH;
    last_gesture.scale = scale;
    pstr("  Gesture: PINCH scale="); pfloat(scale); pstr("\n");
    return GESTURE_PINCH;
}
int gesture_detect_rotate(float angle) {
    last_gesture.type = GESTURE_ROTATE;
    last_gesture.angle = angle;
    pstr("  Gesture: ROTATE angle="); pfloat(angle); pstr("\n");
    return GESTURE_ROTATE;
}
int gesture_stats(void) {
    pstr("  Touch stats:\n");
    pstr("    Active touches: "); pint(active_touches); pstr("\n");
    pstr("    Max points: "); pint(MAX_TOUCH_POINTS); pstr("\n");
    pstr("    Last gesture: "); pint(last_gesture.type); pstr("\n");
    return active_touches;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("Touch Input v46.0 - Gesture Recognition\n");
    if(help) { pstr("Usage: touch_input [-h|-t]\n"); return; }
    if(test) {
        pstr("=== Touch Input Test ===\n\n");
        pstr("[1] Single tap\n"); touch_down(0, 100, 200); gesture_detect_tap(100, 200); touch_up(0); pstr("\n");
        pstr("[2] Swipe\n"); touch_down(0, 100, 100); touch_move(0, 200, 100); touch_move(0, 300, 100); gesture_detect_swipe(100, 100, 300, 100); touch_up(0); pstr("\n");
        pstr("[3] Multi-touch\n"); touch_down(0, 100, 100); touch_down(1, 200, 200); pstr("\n");
        pstr("[4] Pinch\n"); gesture_detect_pinch(1.5); touch_up(0); touch_up(1); pstr("\n");
        pstr("[5] Rotate\n"); touch_down(0, 150, 150); touch_down(1, 250, 250); gesture_detect_rotate(45.0); touch_up(0); touch_up(1); pstr("\n");
        pstr("[6] Stats\n"); gesture_stats();
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
