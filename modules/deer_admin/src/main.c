/* deer_admin: Deer (Cervus elaphus) deer farm wildlife management (v1.0)
 * Deer housing, feeding, breeding, veterinary, market
 * Features: body_len_cm, body_wt_kg, antler_cm, coat_idx, fawn_count, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,antl_cm,coat_idx,fwn_ct,age_yr,active;} deer_t;
typedef struct{int n_house,n_feed,n_breed,n_vet,n_mkt,t_ln,t_wt,t_ant,t_co,t_fw;} deer_state_t;
static deer_t dhs[N],dfd[N-2],dbr[N-4],dvt[N-6],dmk[N-6]; static deer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(deer_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int ci,int fc,int ay){if(*cnt>=mx)return -1;deer_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->antl_cm=ac;x->coat_idx=ci;x->fwn_ct=fc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DEER] Deer ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ci=");pi(ci);ps(" fc=");pi(fc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int deer_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_vet=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ant=0;st.t_co=0;st.t_fw=0;for(int i=0;i<N;i++)dhs[i].active=0;for(int i=0;i<N-2;i++)dfd[i].active=0;for(int i=0;i<N-4;i++)dbr[i].active=0;for(int i=0;i<N-6;i++)dvt[i].active=0;for(int i=0;i<N-6;i++)dmk[i].active=0;init=1;ps("[DEER] Deer initialized\n");return 0;}
int deer_housing(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(dhs,&st.n_house,&st.t_ln,N,lc,bl,bw,ac,ci,fc,ay);}
int deer_feeding(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(dfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,ci,fc,ay);}
int deer_breeding(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(dbr,&st.n_breed,&st.t_ant,N-4,lc,bl,bw,ac,ci,fc,ay);}
int deer_veterinary(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(dvt,&st.n_vet,&st.t_co,N-6,lc,bl,bw,ac,ci,fc,ay);}
int deer_market(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(dmk,&st.n_mkt,&st.t_fw,N-6,lc,bl,bw,ac,ci,fc,ay);}
void deer_report(void){ps("[DEER] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ant=");pi(st.t_ant);ps("\nVet: ");pi(st.n_vet);ps(" Co=");pi(st.t_co);ps("\nMkt: ");pi(st.n_mkt);ps(" Fw=");pi(st.t_fw);ps("\n");}
void deer_state(void){ps("[DEER] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Deer Admin Demo ===\n\n");deer_init();
/* 1=enclosure 2=ranch 3=reserve 4=forest 5=market */
ps("Deer housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*10),bw=80+(i*8),ac=30+(i*5),ci=(i%4)+1,fc=(i%3),ay=1+(i%8);deer_housing(lc,bl,bw,ac,ci,fc,ay);}
ps("\nDeer feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*8),bw=85+(i*7),ac=35+(i*4),ci=(i%3)+1,fc=(i%4),ay=2+(i%6);deer_feeding(lc,bl,bw,ac,ci,fc,ay);}
ps("\nDeer breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*7),bw=90+(i*6),ac=40+(i*3),ci=(i%5)+1,fc=1+(i%3),ay=2+(i%5);deer_breeding(lc,bl,bw,ac,ci,fc,ay);}
ps("\nDeer veterinary...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=145+(i*12),bw=75+(i*10),ac=25+(i*6),ci=(i%6)+1,fc=(i%5),ay=3+(i%7);deer_veterinary(lc,bl,bw,ac,ci,fc,ay);}
ps("\nDeer market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*6),bw=95+(i*5),ac=45+(i*3),ci=(i%3)+3,fc=2+(i%2),ay=3+(i%4);deer_market(lc,bl,bw,ac,ci,fc,ay);}
ps("\n");deer_report();deer_state();ps("\n=== Demo Complete ===\n");return 0;}
