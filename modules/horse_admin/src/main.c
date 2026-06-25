/* horse_admin: Horse (Equus caballus) domestic horse equine mammal (v1.0)
 * Horse stable, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, height_cm, gallop_kmh, hoof_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ht_cm,gl_kh,hf_idx,age_yr,active;} horse_t;
typedef struct{int n_stab,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ht,t_gl,t_hf;} horse_state_t;
static horse_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static horse_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(horse_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int gk,int hi,int ay){if(*cnt>=mx)return -1;horse_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ht_cm=hc;x->gl_kh=gk;x->hf_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HORS] Horse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" gk=");pi(gk);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int horse_init(void){if(init)return -1;st.n_stab=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ht=0;st.t_gl=0;st.t_hf=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[HORS] Horse initialized\n");return 0;}
int horse_stable(int lc,int bl,int bw,int hc,int gk,int hi,int ay){return add(gpl,&st.n_stab,&st.t_ln,N,lc,bl,bw,hc,gk,hi,ay);}
int horse_feeding(int lc,int bl,int bw,int hc,int gk,int hi,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,gk,hi,ay);}
int horse_breeding(int lc,int bl,int bw,int hc,int gk,int hi,int ay){return add(gbr,&st.n_breed,&st.t_ht,N-4,lc,bl,bw,hc,gk,hi,ay);}
int horse_health(int lc,int bl,int bw,int hc,int gk,int hi,int ay){return add(ghl,&st.n_health,&st.t_gl,N-6,lc,bl,bw,hc,gk,hi,ay);}
int horse_market(int lc,int bl,int bw,int hc,int gk,int hi,int ay){return add(gmk,&st.n_mkt,&st.t_hf,N-6,lc,bl,bw,hc,gk,hi,ay);}
void horse_report(void){ps("[HORS] Stab: ");pi(st.n_stab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ht=");pi(st.t_ht);ps("\nHlth: ");pi(st.n_health);ps(" Gl=");pi(st.t_gl);ps("\nMkt: ");pi(st.n_mkt);ps(" Hf=");pi(st.t_hf);ps("\n");}
void horse_state(void){ps("[HORS] Stab=");pi(st.n_stab);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Horse Admin Demo ===\n\n");horse_init();
/* 1=stable 2=paddock 3=pasture 4=arena 5=track */
ps("Horse stable...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=220+(i*8),bw=450+(i*20),hc=150+(i*3),gk=50+(i*4),hi=(i%5)+1,ay=(i%8)+1;horse_stable(lc,bl,bw,hc,gk,hi,ay);}
ps("\nHorse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=225+(i*6),bw=460+(i*18),hc=152+(i*3),gk=52+(i*3),hi=(i%4)+1,ay=(i%6)+1;horse_feeding(lc,bl,bw,hc,gk,hi,ay);}
ps("\nHorse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=215+(i*10),bw=440+(i*25),hc=148+(i*4),gk=48+(i*5),hi=(i%3)+2,ay=(i%5)+1;horse_breeding(lc,bl,bw,hc,gk,hi,ay);}
ps("\nHorse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=230+(i*6),bw=470+(i*15),hc=155+(i*2),gk=55+(i*3),hi=(i%5)+1,ay=(i%6)+1;horse_health(lc,bl,bw,hc,gk,hi,ay);}
ps("\nHorse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=235+(i*5),bw=480+(i*12),hc=158+(i*2),gk=58+(i*2),hi=(i%4)+1,ay=(i%4)+1;horse_market(lc,bl,bw,hc,gk,hi,ay);}
ps("\n");horse_report();horse_state();ps("\n=== Demo Complete ===\n");return 0;}
