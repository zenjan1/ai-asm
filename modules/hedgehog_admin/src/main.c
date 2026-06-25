/* hedgehog_admin: Hedgehog (Erinaceus europaeus) European spiny mammal (v1.0)
 * Hedgehog garden, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, spine_cm, walk_speed, spine_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,spn_cm,wk_sp,sn_idx,age_yr,active;} hdg_t;
typedef struct{int n_garden,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_spn,t_wk,t_sn;} hdg_state_t;
static hdg_t gdn[N],gdf[N-2],gdb[N-4],gdh[N-6],gdm[N-6]; static hdg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hdg_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ws,int si,int ay){if(*cnt>=mx)return -1;hdg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->spn_cm=sc;x->wk_sp=ws;x->sn_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[HDG] Hedgehog ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ws=");pi(ws);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int hedgehog_init(void){if(init)return -1;st.n_garden=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_spn=0;st.t_wk=0;st.t_sn=0;for(int i=0;i<N;i++)gdn[i].active=0;for(int i=0;i<N-2;i++)gdf[i].active=0;for(int i=0;i<N-4;i++)gdb[i].active=0;for(int i=0;i<N-6;i++)gdh[i].active=0;for(int i=0;i<N-6;i++)gdm[i].active=0;init=1;ps("[HDG] Hedgehog initialized\n");return 0;}
int hedgehog_garden(int lc,int bl,int bw,int sc,int ws,int si,int ay){return add(gdn,&st.n_garden,&st.t_ln,N,lc,bl,bw,sc,ws,si,ay);}
int hedgehog_feeding(int lc,int bl,int bw,int sc,int ws,int si,int ay){return add(gdf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ws,si,ay);}
int hedgehog_breeding(int lc,int bl,int bw,int sc,int ws,int si,int ay){return add(gdb,&st.n_breed,&st.t_spn,N-4,lc,bl,bw,sc,ws,si,ay);}
int hedgehog_health(int lc,int bl,int bw,int sc,int ws,int si,int ay){return add(gdh,&st.n_health,&st.t_wk,N-6,lc,bl,bw,sc,ws,si,ay);}
int hedgehog_market(int lc,int bl,int bw,int sc,int ws,int si,int ay){return add(gdm,&st.n_mkt,&st.t_sn,N-6,lc,bl,bw,sc,ws,si,ay);}
void hedgehog_report(void){ps("[HDG] Garden: ");pi(st.n_garden);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Spn=");pi(st.t_spn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Sn=");pi(st.t_sn);ps("\n");}
void hedgehog_state(void){ps("[HDG] Garden=");pi(st.n_garden);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Hedgehog Admin Demo ===\n\n");hedgehog_init();
ps("Hedgehog garden...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=25+(i*2),bw=600+(i*50),sc=2+(i*1),ws=1+(i*1),si=(i%8)+1,ay=(i%8)+1;hedgehog_garden(lc,bl,bw,sc,ws,si,ay);}
ps("\nHedgehog feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=26+(i*2),bw=620+(i*40),sc=2+(i*1),ws=1+(i*1),si=(i%7)+1,ay=(i%7)+1;hedgehog_feeding(lc,bl,bw,sc,ws,si,ay);}
ps("\nHedgehog breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=27+(i*1),bw=640+(i*30),sc=3+(i*1),ws=1+(i*1),si=(i%6)+1,ay=(i%6)+1;hedgehog_breeding(lc,bl,bw,sc,ws,si,ay);}
ps("\nHedgehog health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=24+(i*3),bw=580+(i*60),sc=2+(i*1),ws=1+(i*1),si=(i%9)+1,ay=(i%5)+1;hedgehog_health(lc,bl,bw,sc,ws,si,ay);}
ps("\nHedgehog market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=28+(i*1),bw=660+(i*25),sc=3+(i*1),ws=2+(i*1),si=(i%5)+1,ay=(i%4)+1;hedgehog_market(lc,bl,bw,sc,ws,si,ay);}
ps("\n");hedgehog_report();hedgehog_state();ps("\n=== Demo Complete ===\n");return 0;}
