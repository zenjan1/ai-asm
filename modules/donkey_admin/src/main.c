/* donkey_admin: Donkey (Equus asinus) domestic equine (v1.0)
 * Donkey farm, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, walk_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,wk_sp,hd_idx,age_yr,active;} dnky_t;
typedef struct{int n_farm,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_wk,t_hd;} dnky_state_t;
static dnky_t fml[N],fmf[N-2],fmb[N-4],fmh[N-6],fmm[N-6]; static dnky_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dnky_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ws,int hi,int ay){if(*cnt>=mx)return -1;dnky_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->wk_sp=ws;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DNKY] Donkey ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ws=");pi(ws);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int donkey_init(void){if(init)return -1;st.n_farm=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_wk=0;st.t_hd=0;for(int i=0;i<N;i++)fml[i].active=0;for(int i=0;i<N-2;i++)fmf[i].active=0;for(int i=0;i<N-4;i++)fmb[i].active=0;for(int i=0;i<N-6;i++)fmh[i].active=0;for(int i=0;i<N-6;i++)fmm[i].active=0;init=1;ps("[DNKY] Donkey initialized\n");return 0;}
int donkey_farm(int lc,int bl,int bw,int ec,int ws,int hi,int ay){return add(fml,&st.n_farm,&st.t_ln,N,lc,bl,bw,ec,ws,hi,ay);}
int donkey_feeding(int lc,int bl,int bw,int ec,int ws,int hi,int ay){return add(fmf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ws,hi,ay);}
int donkey_breeding(int lc,int bl,int bw,int ec,int ws,int hi,int ay){return add(fmb,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ws,hi,ay);}
int donkey_health(int lc,int bl,int bw,int ec,int ws,int hi,int ay){return add(fmh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,ec,ws,hi,ay);}
int donkey_market(int lc,int bl,int bw,int ec,int ws,int hi,int ay){return add(fmm,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,ec,ws,hi,ay);}
void donkey_report(void){ps("[DNKY] Farm: ");pi(st.n_farm);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void donkey_state(void){ps("[DNKY] Farm=");pi(st.n_farm);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Donkey Admin Demo ===\n\n");donkey_init();
ps("Donkey farm...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=120+(i*8),bw=150+(i*20),ec=12+(i*2),ws=4+(i*2),hi=(i%8)+1,ay=(i%20)+1;donkey_farm(lc,bl,bw,ec,ws,hi,ay);}
ps("\nDonkey feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=124+(i*6),bw=160+(i*15),ec=13+(i%3),ws=5+(i*1),hi=(i%6)+1,ay=(i%18)+1;donkey_feeding(lc,bl,bw,ec,ws,hi,ay);}
ps("\nDonkey breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=128+(i*5),bw=170+(i*12),ec=14+(i%2),ws=3+(i*3),hi=(i%5)+1,ay=(i%15)+1;donkey_breeding(lc,bl,bw,ec,ws,hi,ay);}
ps("\nDonkey health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=118+(i*10),bw=145+(i*25),ec=11+(i*3),ws=2+(i*4),hi=(i%10)+1,ay=(i%12)+1;donkey_health(lc,bl,bw,ec,ws,hi,ay);}
ps("\nDonkey market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=130+(i*4),bw=180+(i*10),ec=15+(i%2),ws=6+(i*2),hi=(i%4)+1,ay=(i%10)+1;donkey_market(lc,bl,bw,ec,ws,hi,ay);}
ps("\n");donkey_report();donkey_state();ps("\n=== Demo Complete ===\n");return 0;}
