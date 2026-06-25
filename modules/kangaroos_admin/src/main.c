/* kangaroos_admin: Kangaroos (Macropus) large macropod marsupials (v1.0)
 * Kangaroos plains, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, hop_m, speed_kmh, kg_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,hop_m,spd_kmh,kg_idx,age_yr,active;} kgrs_t;
typedef struct{int n_plains,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_hop,t_spd,t_kg;} kgrs_state_t;
static kgrs_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static kgrs_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(kgrs_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hm,int sk,int kg,int ay){if(*cnt>=mx)return -1;kgrs_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->hop_m=hm;x->spd_kmh=sk;x->kg_idx=kg;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KGRS] Kangaroo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hm=");pi(hm);ps(" sk=");pi(sk);ps(" kg=");pi(kg);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int kangaroos_init(void){if(init)return -1;st.n_plains=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_hop=0;st.t_spd=0;st.t_kg=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[KGRS] Kangaroos initialized\n");return 0;}
int kangaroos_plains(int lc,int bl,int bw,int hm,int sk,int kg,int ay){return add(svl,&st.n_plains,&st.t_ln,N,lc,bl,bw,hm,sk,kg,ay);}
int kangaroos_feeding(int lc,int bl,int bw,int hm,int sk,int kg,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hm,sk,kg,ay);}
int kangaroos_breeding(int lc,int bl,int bw,int hm,int sk,int kg,int ay){return add(svb,&st.n_breed,&st.t_hop,N-4,lc,bl,bw,hm,sk,kg,ay);}
int kangaroos_health(int lc,int bl,int bw,int hm,int sk,int kg,int ay){return add(svh,&st.n_health,&st.t_spd,N-6,lc,bl,bw,hm,sk,kg,ay);}
int kangaroos_market(int lc,int bl,int bw,int hm,int sk,int kg,int ay){return add(svm,&st.n_mkt,&st.t_kg,N-6,lc,bl,bw,hm,sk,kg,ay);}
void kangaroos_report(void){ps("[KGRS] Plains: ");pi(st.n_plains);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Hop=");pi(st.t_hop);ps("\nHealth: ");pi(st.n_health);ps(" Spd=");pi(st.t_spd);ps("\nMkt: ");pi(st.n_mkt);ps(" Kg=");pi(st.t_kg);ps("\n");}
void kangaroos_state(void){ps("[KGRS] Plains=");pi(st.n_plains);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Kangaroos Admin Demo ===\n\n");kangaroos_init();
ps("Kangaroos plains...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=40+(i*5),hm=3+(i%3),sk=40+(i*3),kg=(i%8)+1,ay=(i%8)+1;kangaroos_plains(lc,bl,bw,hm,sk,kg,ay);}
ps("\nKangaroos feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=125+(i*6),bw=42+(i*4),hm=3+(i%2),sk=42+(i*2),kg=(i%6)+1,ay=(i%7)+1;kangaroos_feeding(lc,bl,bw,hm,sk,kg,ay);}
ps("\nKangaroos breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=130+(i*5),bw=45+(i*3),hm=4+(i%2),sk=44+(i*2),kg=(i%5)+1,ay=(i%6)+1;kangaroos_breeding(lc,bl,bw,hm,sk,kg,ay);}
ps("\nKangaroos health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=115+(i*10),bw=38+(i*6),hm=2+(i%3),sk=38+(i*3),kg=(i%10)+1,ay=(i%7)+1;kangaroos_health(lc,bl,bw,hm,sk,kg,ay);}
ps("\nKangaroos market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=135+(i*4),bw=48+(i*2),hm=4+(i%2),sk=46+(i*2),kg=(i%4)+1,ay=(i%5)+1;kangaroos_market(lc,bl,bw,hm,sk,kg,ay);}
ps("\n");kangaroos_report();kangaroos_state();ps("\n=== Demo Complete ===\n");return 0;}
