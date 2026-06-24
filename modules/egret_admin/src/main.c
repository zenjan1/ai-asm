/* egret_admin: Egret (Ardea alba) great egret wading bird management (v1.0)
 * Egret habitat, feeding, nesting, grooming, market
 * Features: body_len_cm, body_wt_g, neck_len_cm, beak_cm, plumage_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,neck_ln,bk_cm,plm_idx,age_yr,active;} egre_t;
typedef struct{int n_hab,n_feed,n_nest,n_groom,n_mkt,t_ln,t_wt,t_nk,t_bk,t_pl;} egre_state_t;
static egre_t ehab[N],efd[N-2],ens[N-4],egr[N-6],emk[N-6]; static egre_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(egre_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int nl,int bm,int px,int ay){if(*cnt>=mx)return -1;egre_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->neck_ln=nl;x->bk_cm=bm;x->plm_idx=px;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EGRE] Egret ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" nl=");pi(nl);ps(" bm=");pi(bm);ps(" px=");pi(px);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int egre_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_nest=0;st.n_groom=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_nk=0;st.t_bk=0;st.t_pl=0;for(int i=0;i<N;i++)ehab[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ens[i].active=0;for(int i=0;i<N-6;i++)egr[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[EGRE] Egret initialized\n");return 0;}
int egre_habitat(int lc,int bl,int bw,int nl,int bm,int px,int ay){return add(ehab,&st.n_hab,&st.t_ln,N,lc,bl,bw,nl,bm,px,ay);}
int egre_feeding(int lc,int bl,int bw,int nl,int bm,int px,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,nl,bm,px,ay);}
int egre_nesting(int lc,int bl,int bw,int nl,int bm,int px,int ay){return add(ens,&st.n_nest,&st.t_nk,N-4,lc,bl,bw,nl,bm,px,ay);}
int egre_grooming(int lc,int bl,int bw,int nl,int bm,int px,int ay){return add(egr,&st.n_groom,&st.t_bk,N-6,lc,bl,bw,nl,bm,px,ay);}
int egre_market(int lc,int bl,int bw,int nl,int bm,int px,int ay){return add(emk,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,nl,bm,px,ay);}
void egre_report(void){ps("[EGRE] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nNest: ");pi(st.n_nest);ps(" Nk=");pi(st.t_nk);ps("\nGroom: ");pi(st.n_groom);ps(" Bk=");pi(st.t_bk);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void egre_state(void){ps("[EGRE] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Nest=");pi(st.n_nest);ps(" Groom=");pi(st.n_groom);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Egret Admin Demo ===\n\n");egre_init();
/* 1=wetland 2=marsh 3=lake 4=coast 5=market */
ps("Egret habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*6),bw=50+(i*5),nl=40+(i*5),bm=12+(i*2),px=(i%5)+1,ay=1+(i%8);egre_habitat(lc,bl,bw,nl,bm,px,ay);}
ps("\nEgret feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=95+(i*5),bw=55+(i*4),nl=45+(i*4),bm=13+(i%3),px=(i%4)+1,ay=2+(i%6);egre_feeding(lc,bl,bw,nl,bm,px,ay);}
ps("\nEgret nesting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=85+(i*7),bw=48+(i*6),nl=35+(i*6),bm=11+(i*3),px=(i%6)+1,ay=2+(i%5);egre_nesting(lc,bl,bw,nl,bm,px,ay);}
ps("\nEgret grooming...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=100+(i*5),bw=60+(i*4),nl=50+(i*4),bm=14+(i%2),px=(i%7)+1,ay=3+(i%7);egre_grooming(lc,bl,bw,nl,bm,px,ay);}
ps("\nEgret market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=105+(i*4),bw=65+(i*3),nl=55+(i*3),bm=15+(i%2),px=(i%3)+4,ay=3+(i%4);egre_market(lc,bl,bw,nl,bm,px,ay);}
ps("\n");egre_report();egre_state();ps("\n=== Demo Complete ===\n");return 0;}
