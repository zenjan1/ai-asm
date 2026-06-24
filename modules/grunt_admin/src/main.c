/* grunt_admin: Grunt (Haemulidae) grunt fish perciform (v1.0)
 * Grunt tank, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, fin_ct, scale_idx, grunt_vol, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_ct,sc_idx,gr_vl,age_yr,active;} grunt_t;
typedef struct{int n_tank,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sc,t_gr;} grunt_state_t;
static grunt_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static grunt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grunt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int si,int gv,int ay){if(*cnt>=mx)return -1;grunt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_ct=fc;x->sc_idx=si;x->gr_vl=gv;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GRNT] Grunt ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" si=");pi(si);ps(" gv=");pi(gv);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int grunt_init(void){if(init)return -1;st.n_tank=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sc=0;st.t_gr=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GRNT] Grunt initialized\n");return 0;}
int grunt_tank(int lc,int bl,int bw,int fc,int si,int gv,int ay){return add(gpl,&st.n_tank,&st.t_ln,N,lc,bl,bw,fc,si,gv,ay);}
int grunt_feeding(int lc,int bl,int bw,int fc,int si,int gv,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,si,gv,ay);}
int grunt_breeding(int lc,int bl,int bw,int fc,int si,int gv,int ay){return add(gbr,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,si,gv,ay);}
int grunt_health(int lc,int bl,int bw,int fc,int si,int gv,int ay){return add(ghl,&st.n_health,&st.t_sc,N-6,lc,bl,bw,fc,si,gv,ay);}
int grunt_market(int lc,int bl,int bw,int fc,int si,int gv,int ay){return add(gmk,&st.n_mkt,&st.t_gr,N-6,lc,bl,bw,fc,si,gv,ay);}
void grunt_report(void){ps("[GRNT] Tank: ");pi(st.n_tank);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHlth: ");pi(st.n_health);ps(" Sc=");pi(st.t_sc);ps("\nMkt: ");pi(st.n_mkt);ps(" Gr=");pi(st.t_gr);ps("\n");}
void grunt_state(void){ps("[GRNT] Tank=");pi(st.n_tank);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Grunt Admin Demo ===\n\n");grunt_init();
/* 1=reef 2=shore 3=estuary 4=rock 5=aquarium */
ps("Grunt tank...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=20+(i*3),bw=50+(i*10),fc=6+(i%4),si=(i%5)+1,gv=30+(i*8),ay=(i%4)+1;grunt_tank(lc,bl,bw,fc,si,gv,ay);}
ps("\nGrunt feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=22+(i*2),bw=55+(i*8),fc=7+(i%3),si=(i%4)+1,gv=35+(i*6),ay=(i%3)+1;grunt_feeding(lc,bl,bw,fc,si,gv,ay);}
ps("\nGrunt breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=18+(i*4),bw=45+(i*12),fc=5+(i%5),si=(i%3)+2,gv=25+(i*7),ay=(i%3)+1;grunt_breeding(lc,bl,bw,fc,si,gv,ay);}
ps("\nGrunt health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=25+(i*2),bw=60+(i*7),fc=8+(i%3),si=(i%5)+1,gv=40+(i*5),ay=(i%4)+1;grunt_health(lc,bl,bw,fc,si,gv,ay);}
ps("\nGrunt market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=28+(i*2),bw=65+(i*6),fc=9+(i%2),si=(i%4)+1,gv=45+(i*4),ay=(i%3)+1;grunt_market(lc,bl,bw,fc,si,gv,ay);}
ps("\n");grunt_report();grunt_state();ps("\n=== Demo Complete ===\n");return 0;}
