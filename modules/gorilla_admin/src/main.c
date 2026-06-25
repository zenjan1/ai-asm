/* gorilla_admin: Gorilla (Gorilla gorilla) western great ape (v1.0)
 * Gorilla jungle, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, arm_cm, walk_speed, chest_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,arm_cm,wk_sp,ch_idx,age_yr,active;} grl_t;
typedef struct{int n_jungle,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_arm,t_wk,t_ch;} grl_state_t;
static grl_t jng[N],jnf[N-2],jnb[N-4],jnh[N-6],jnm[N-6]; static grl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grl_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int ws,int ci,int ay){if(*cnt>=mx)return -1;grl_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->arm_cm=ac;x->wk_sp=ws;x->ch_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GRL] Gorilla ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" ws=");pi(ws);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int gorilla_init(void){if(init)return -1;st.n_jungle=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_arm=0;st.t_wk=0;st.t_ch=0;for(int i=0;i<N;i++)jng[i].active=0;for(int i=0;i<N-2;i++)jnf[i].active=0;for(int i=0;i<N-4;i++)jnb[i].active=0;for(int i=0;i<N-6;i++)jnh[i].active=0;for(int i=0;i<N-6;i++)jnm[i].active=0;init=1;ps("[GRL] Gorilla initialized\n");return 0;}
int gorilla_jungle(int lc,int bl,int bw,int ac,int ws,int ci,int ay){return add(jng,&st.n_jungle,&st.t_ln,N,lc,bl,bw,ac,ws,ci,ay);}
int gorilla_feeding(int lc,int bl,int bw,int ac,int ws,int ci,int ay){return add(jnf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,ws,ci,ay);}
int gorilla_breeding(int lc,int bl,int bw,int ac,int ws,int ci,int ay){return add(jnb,&st.n_breed,&st.t_arm,N-4,lc,bl,bw,ac,ws,ci,ay);}
int gorilla_health(int lc,int bl,int bw,int ac,int ws,int ci,int ay){return add(jnh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,ac,ws,ci,ay);}
int gorilla_market(int lc,int bl,int bw,int ac,int ws,int ci,int ay){return add(jnm,&st.n_mkt,&st.t_ch,N-6,lc,bl,bw,ac,ws,ci,ay);}
void gorilla_report(void){ps("[GRL] Jungle: ");pi(st.n_jungle);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Arm=");pi(st.t_arm);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Ch=");pi(st.t_ch);ps("\n");}
void gorilla_state(void){ps("[GRL] Jungle=");pi(st.n_jungle);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Gorilla Admin Demo ===\n\n");gorilla_init();
ps("Gorilla jungle...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=160+(i*8),bw=150+(i*15),ac=130+(i*8),ws=3+(i*1),ci=(i%10)+1,ay=(i%30)+1;gorilla_jungle(lc,bl,bw,ac,ws,ci,ay);}
ps("\nGorilla feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=165+(i*6),bw=155+(i*12),ac=132+(i*6),ws=4+(i*1),ci=(i%9)+1,ay=(i%25)+1;gorilla_feeding(lc,bl,bw,ac,ws,ci,ay);}
ps("\nGorilla breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=170+(i*5),bw=160+(i*10),ac=134+(i*5),ws=3+(i*2),ci=(i%8)+1,ay=(i%20)+1;gorilla_breeding(lc,bl,bw,ac,ws,ci,ay);}
ps("\nGorilla health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=158+(i*9),bw=148+(i*18),ac=128+(i*9),ws=2+(i*3),ci=(i%11)+1,ay=(i%15)+1;gorilla_health(lc,bl,bw,ac,ws,ci,ay);}
ps("\nGorilla market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=175+(i*4),bw=165+(i*8),ac=136+(i*4),ws=4+(i*1),ci=(i%6)+1,ay=(i%12)+1;gorilla_market(lc,bl,bw,ac,ws,ci,ay);}
ps("\n");gorilla_report();gorilla_state();ps("\n=== Demo Complete ===\n");return 0;}
