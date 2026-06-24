/* elk_admin: Elk (Cervus canadensis) elk deer wildlife management (v1.0)
 * Elk habitat, feeding, breeding, veterinary, market
 * Features: body_len_cm, body_wt_kg, antler_cm, coat_idx, fawn_count, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,antl_cm,coat_idx,fwn_ct,age_yr,active;} elk_t;
typedef struct{int n_hab,n_feed,n_breed,n_vet,n_mkt,t_ln,t_wt,t_ant,t_co,t_fw;} elk_state_t;
static elk_t ehab[N],efd[N-2],ebr[N-4],evt[N-6],emk[N-6]; static elk_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(elk_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int ci,int fc,int ay){if(*cnt>=mx)return -1;elk_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->antl_cm=ac;x->coat_idx=ci;x->fwn_ct=fc;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ELK] Elk ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ci=");pi(ci);ps(" fc=");pi(fc);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int elk_init(void){if(init)return -1;st.n_hab=0;st.n_feed=0;st.n_breed=0;st.n_vet=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ant=0;st.t_co=0;st.t_fw=0;for(int i=0;i<N;i++)ehab[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ebr[i].active=0;for(int i=0;i<N-6;i++)evt[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[ELK] Elk initialized\n");return 0;}
int elk_habitat(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(ehab,&st.n_hab,&st.t_ln,N,lc,bl,bw,ac,ci,fc,ay);}
int elk_feeding(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,ci,fc,ay);}
int elk_breeding(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(ebr,&st.n_breed,&st.t_ant,N-4,lc,bl,bw,ac,ci,fc,ay);}
int elk_veterinary(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(evt,&st.n_vet,&st.t_co,N-6,lc,bl,bw,ac,ci,fc,ay);}
int elk_market(int lc,int bl,int bw,int ac,int ci,int fc,int ay){return add(emk,&st.n_mkt,&st.t_fw,N-6,lc,bl,bw,ac,ci,fc,ay);}
void elk_report(void){ps("[ELK] Hab: ");pi(st.n_hab);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ant=");pi(st.t_ant);ps("\nVet: ");pi(st.n_vet);ps(" Co=");pi(st.t_co);ps("\nMkt: ");pi(st.n_mkt);ps(" Fw=");pi(st.t_fw);ps("\n");}
void elk_state(void){ps("[ELK] Hab=");pi(st.n_hab);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Elk Admin Demo ===\n\n");elk_init();
/* 1=forest 2=meadow 3=reserve 4=ranch 5=market */
ps("Elk habitat...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=250+(i*20),ac=60+(i*5),ci=(i%5)+1,fc=(i%3),ay=2+(i%8);elk_habitat(lc,bl,bw,ac,ci,fc,ay);}
ps("\nElk feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=260+(i*15),ac=65+(i*4),ci=(i%4)+1,fc=(i%4),ay=3+(i%6);elk_feeding(lc,bl,bw,ac,ci,fc,ay);}
ps("\nElk breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*7),bw=270+(i*12),ac=70+(i*3),ci=(i%6)+1,fc=1+(i%3),ay=3+(i%5);elk_breeding(lc,bl,bw,ac,ci,fc,ay);}
ps("\nElk veterinary...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*12),bw=240+(i*25),ac=55+(i*6),ci=(i%7)+1,fc=(i%5),ay=4+(i%7);elk_veterinary(lc,bl,bw,ac,ci,fc,ay);}
ps("\nElk market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*6),bw=280+(i*10),ac=75+(i*3),ci=(i%3)+3,fc=2+(i%2),ay=3+(i%4);elk_market(lc,bl,bw,ac,ci,fc,ay);}
ps("\n");elk_report();elk_state();ps("\n=== Demo Complete ===\n");return 0;}
