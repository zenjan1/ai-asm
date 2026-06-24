/* dragon_admin: Komodo Dragon (Varanus komodoensis) largest living lizard (v1.0)
 * Dragon housing, feeding, health, shedding, market
 * Features: body_len_cm, body_wt_kg, tail_len_cm, claw_idx, venom_lvl, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_ln,clw_idx,vnm_lv,age_yr,active;} drag_t;
typedef struct{int n_house,n_feed,n_hlth,n_shed,n_mkt,t_ln,t_wt,t_tl,t_cl,t_vn;} drag_state_t;
static drag_t dhs[N],dfd[N-2],dhl[N-4],dsh[N-6],dmk[N-6]; static drag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(drag_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tl,int ci,int vl,int ay){if(*cnt>=mx)return -1;drag_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_ln=tl;x->clw_idx=ci;x->vnm_lv=vl;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DRAG] Dragon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tl=");pi(tl);ps(" ci=");pi(ci);ps(" vl=");pi(vl);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int drag_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_hlth=0;st.n_shed=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tl=0;st.t_cl=0;st.t_vn=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dhl[i].active=0;for(int i=0;i<N-6;i++)dsh[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DRAG] Dragon initialized\n");return 0;}
int drag_housing(int lc,int bl,int bw,int tl,int ci,int vl,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,tl,ci,vl,ay);}
int drag_feeding(int lc,int bl,int bw,int tl,int ci,int vl,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tl,ci,vl,ay);}
int drag_health(int lc,int bl,int bw,int tl,int ci,int vl,int ay){return add(dhl,&st.n_hlth,&st.t_tl,N-4,lc,bl,bw,tl,ci,vl,ay);}
int drag_shedding(int lc,int bl,int bw,int tl,int ci,int vl,int ay){return add(dsh,&st.n_shed,&st.t_cl,N-6,lc,bl,bw,tl,ci,vl,ay);}
int drag_market(int lc,int bl,int bw,int tl,int ci,int vl,int ay){return add(dmk,&st.n_mkt,&st.t_vn,N-6,lc,bl,bw,tl,ci,vl,ay);}
void drag_report(void){ps("[DRAG] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nHlth: ");pi(st.n_hlth);ps(" Tl=");pi(st.t_tl);ps("\nShed: ");pi(st.n_shed);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Vn=");pi(st.t_vn);ps("\n");}
void drag_state(void){ps("[DRAG] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Hlth=");pi(st.n_hlth);ps(" Shed=");pi(st.n_shed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dragon Admin Demo ===\n\n");drag_init();
/* 1=enclosure 2=zoo 3=reserve 4=island 5=market */
ps("Dragon housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=60+(i*8),tl=150+(i*8),ci=(i%6)+1,vl=(i%8)+1,ay=2+(i%8);drag_housing(lc,bl,bw,tl,ci,vl,ay);}
ps("\nDragon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=65+(i*7),tl=160+(i*7),ci=(i%5)+1,vl=(i%7)+1,ay=3+(i%6);drag_feeding(lc,bl,bw,tl,ci,vl,ay);}
ps("\nDragon health...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*7),bw=70+(i*6),tl=170+(i*6),ci=(i%4)+1,vl=(i%6)+1,ay=2+(i%5);drag_health(lc,bl,bw,tl,ci,vl,ay);}
ps("\nDragon shedding...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=55+(i*10),tl=145+(i*9),ci=(i%7)+1,vl=(i%9)+1,ay=4+(i%7);drag_shedding(lc,bl,bw,tl,ci,vl,ay);}
ps("\nDragon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*6),bw=75+(i*5),tl=180+(i*5),ci=(i%3)+4,vl=(i%5)+4,ay=3+(i%4);drag_market(lc,bl,bw,tl,ci,vl,ay);}
ps("\n");drag_report();drag_state();ps("\n=== Demo Complete ===\n");return 0;}
