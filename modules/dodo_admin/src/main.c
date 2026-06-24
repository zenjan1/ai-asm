/* dodo_admin: Dodo (Raphus cucullatus) extinct bird de-extinction research (v1.0)
 * Dodo research, habitat, breeding, health, museum
 * Features: body_len_cm, body_wt_kg, beak_cm, feather_idx, egg_wt_g, gene_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,bk_cm,fthr_idx,egg_wt,gene_idx,active;} dodo_t;
typedef struct{int n_res,n_hab,n_breed,n_hlth,n_mus,t_ln,t_wt,t_bk,t_egg,t_gn;} dodo_state_t;
static dodo_t dre[N],dhab[N-2],dbr[N-4],dhl[N-6],dmu[N-6]; static dodo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dodo_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bk,int fi,int ew,int gi){if(*cnt>=mx)return -1;dodo_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->bk_cm=bk;x->fthr_idx=fi;x->egg_wt=ew;x->gene_idx=gi;x->active=1;*sum+=bl;(*cnt)++;ps("[DODO] Dodo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bk=");pi(bk);ps(" fi=");pi(fi);ps(" ew=");pi(ew);ps(" gi=");pi(gi);ps("\n");return *cnt-1;}
int dodo_init(void){if(init)return -1;st.n_res=0;st.n_hab=0;st.n_breed=0;st.n_hlth=0;st.n_mus=0;st.t_ln=0;st.t_wt=0;st.t_bk=0;st.t_egg=0;st.t_gn=0;for(int i=0;i<N;i++)dre[i].active=0;for(int i=0;i<N-2;i++)dhab[i].active=0;for(int i=0;i<N-4;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dhl[i].active=0;for(int i=0;i<N-6;i++)dmu[i].active=0;init=1;ps("[DODO] Dodo initialized\n");return 0;}
int dodo_research(int lc,int bl,int bw,int bk,int fi,int ew,int gi){return add(dre,&st.n_res,&st.t_ln,N,lc,bl,bw,bk,fi,ew,gi);}
int dodo_habitat(int lc,int bl,int bw,int bk,int fi,int ew,int gi){return add(dhab,&st.n_hab,&st.t_wt,N-2,lc,bl,bw,bk,fi,ew,gi);}
int dodo_breeding(int lc,int bl,int bw,int bk,int fi,int ew,int gi){return add(dbr,&st.n_breed,&st.t_bk,N-4,lc,bl,bw,bk,fi,ew,gi);}
int dodo_health(int lc,int bl,int bw,int bk,int fi,int ew,int gi){return add(dhl,&st.n_hlth,&st.t_egg,N-6,lc,bl,bw,bk,fi,ew,gi);}
int dodo_museum(int lc,int bl,int bw,int bk,int fi,int ew,int gi){return add(dmu,&st.n_mus,&st.t_gn,N-6,lc,bl,bw,bk,fi,ew,gi);}
void dodo_report(void){ps("[DODO] Res: ");pi(st.n_res);ps(" Ln=");pi(st.t_ln);ps("\nHab: ");pi(st.n_hab);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Bk=");pi(st.t_bk);ps("\nHlth: ");pi(st.n_hlth);ps(" Egg=");pi(st.t_egg);ps("\nMus: ");pi(st.n_mus);ps(" Gn=");pi(st.t_gn);ps("\n");}
void dodo_state(void){ps("[DODO] Res=");pi(st.n_res);ps(" Hab=");pi(st.n_hab);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_hlth);ps(" Mus=");pi(st.n_mus);ps("\n");}
int main(void){
ps("=== Dodo Admin Demo ===\n\n");dodo_init();
/* 1=lab 2=habitat 3=reserve 4=museum 5=archive */
ps("Dodo research...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=50+(i*4),bw=15+(i*3),bk=10+(i*2),fi=(i%6)+1,ew=20+(i*5),gi=(i%8)+1;dodo_research(lc,bl,bw,bk,fi,ew,gi);}
ps("\nDodo habitat...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=55+(i*3),bw=18+(i*2),bk=12+(i*2),fi=(i%5)+1,ew=22+(i*4),gi=(i%7)+1;dodo_habitat(lc,bl,bw,bk,fi,ew,gi);}
ps("\nDodo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=48+(i*5),bw=14+(i*4),bk=9+(i*3),fi=(i%4)+1,ew=18+(i*6),gi=(i%6)+1;dodo_breeding(lc,bl,bw,bk,fi,ew,gi);}
ps("\nDodo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=52+(i*4),bw=16+(i*3),bk=11+(i*2),fi=(i%7)+1,ew=25+(i*4),gi=(i%5)+1;dodo_health(lc,bl,bw,bk,fi,ew,gi);}
ps("\nDodo museum...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=60+(i*3),bw=20+(i*2),bk=13+(i*2),fi=(i%3)+4,ew=28+(i*3),gi=(i%4)+4;dodo_museum(lc,bl,bw,bk,fi,ew,gi);}
ps("\n");dodo_report();dodo_state();ps("\n=== Demo Complete ===\n");return 0;}
