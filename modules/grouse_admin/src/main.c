/* grouse_admin: Grouse (Lagopus lagopus) willow grouse game bird (v1.0)
 * Grouse habitat, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_span_cm, feather_idx, brood_ct, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_sp,fthr_idx,br_ct,age_yr,active;} grouse_t;
typedef struct{int n_habit,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fthr,t_br;} grouse_state_t;
static grouse_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static grouse_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grouse_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ws,int fi,int bc,int ay){if(*cnt>=mx)return -1;grouse_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_sp=ws;x->fthr_idx=fi;x->br_ct=bc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GROU] Grouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ws=");pi(ws);ps(" fi=");pi(fi);ps(" bc=");pi(bc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int grouse_init(void){if(init)return -1;st.n_habit=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fthr=0;st.t_br=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GROU] Grouse initialized\n");return 0;}
int grouse_habitat(int lc,int bl,int bw,int ws,int fi,int bc,int ay){return add(gpl,&st.n_habit,&st.t_ln,N,lc,bl,bw,ws,fi,bc,ay);}
int grouse_feeding(int lc,int bl,int bw,int ws,int fi,int bc,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ws,fi,bc,ay);}
int grouse_breeding(int lc,int bl,int bw,int ws,int fi,int bc,int ay){return add(gbr,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,ws,fi,bc,ay);}
int grouse_health(int lc,int bl,int bw,int ws,int fi,int bc,int ay){return add(ghl,&st.n_health,&st.t_fthr,N-6,lc,bl,bw,ws,fi,bc,ay);}
int grouse_market(int lc,int bl,int bw,int ws,int fi,int bc,int ay){return add(gmk,&st.n_mkt,&st.t_br,N-6,lc,bl,bw,ws,fi,bc,ay);}
void grouse_report(void){ps("[GROU] Habit: ");pi(st.n_habit);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHlth: ");pi(st.n_health);ps(" Fthr=");pi(st.t_fthr);ps("\nMkt: ");pi(st.n_mkt);ps(" Br=");pi(st.t_br);ps("\n");}
void grouse_state(void){ps("[GROU] Habit=");pi(st.n_habit);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Grouse Admin Demo ===\n\n");grouse_init();
/* 1=moor 2=heath 3=forest 4=tundra 5=aviary */
ps("Grouse habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*3),bw=1+(i*2),ws=50+(i*4),fi=(i%5)+1,bc=(i%4)+1,ay=(i%4)+1;grouse_habitat(lc,bl,bw,ws,fi,bc,ay);}
ps("\nGrouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=37+(i*2),bw=1+(i*2),ws=52+(i*3),fi=(i%4)+1,bc=(i%3)+1,ay=(i%3)+1;grouse_feeding(lc,bl,bw,ws,fi,bc,ay);}
ps("\nGrouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=33+(i*4),bw=2+(i*2),ws=48+(i*4),fi=(i%3)+2,bc=(i%3)+2,ay=(i%3)+1;grouse_breeding(lc,bl,bw,ws,fi,bc,ay);}
ps("\nGrouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=40+(i*2),bw=2+(i*2),ws=55+(i*3),fi=(i%5)+1,bc=(i%4)+1,ay=(i%4)+1;grouse_health(lc,bl,bw,ws,fi,bc,ay);}
ps("\nGrouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=42+(i*2),bw=3+(i*2),ws=58+(i*3),fi=(i%4)+1,bc=(i%3)+2,ay=(i%3)+1;grouse_market(lc,bl,bw,ws,fi,bc,ay);}
ps("\n");grouse_report();grouse_state();ps("\n=== Demo Complete ===\n");return 0;}
