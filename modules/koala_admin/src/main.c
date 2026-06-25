/* koala_admin: Koala (Phascolarctos cinereus) tree-dwelling marsupial (v1.0)
 * Koala eucalyptus, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, climb_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,cl_sp,fr_idx,age_yr,active;} koala_t;
typedef struct{int n_eucalypt,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_cl,t_fr;} koala_state_t;
static koala_t euc[N],euf[N-2],eub[N-4],euh[N-6],eum[N-6]; static koala_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(koala_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int cs,int fi,int ay){if(*cnt>=mx)return -1;koala_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->cl_sp=cs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[KOALA] Koala ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" cs=");pi(cs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int koala_init(void){if(init)return -1;st.n_eucalypt=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_cl=0;st.t_fr=0;for(int i=0;i<N;i++)euc[i].active=0;for(int i=0;i<N-2;i++)euf[i].active=0;for(int i=0;i<N-4;i++)eub[i].active=0;for(int i=0;i<N-6;i++)euh[i].active=0;for(int i=0;i<N-6;i++)eum[i].active=0;init=1;ps("[KOALA] Koala initialized\n");return 0;}
int koala_eucalyptus(int lc,int bl,int bw,int ec,int cs,int fi,int ay){return add(euc,&st.n_eucalypt,&st.t_ln,N,lc,bl,bw,ec,cs,fi,ay);}
int koala_feeding(int lc,int bl,int bw,int ec,int cs,int fi,int ay){return add(euf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,cs,fi,ay);}
int koala_breeding(int lc,int bl,int bw,int ec,int cs,int fi,int ay){return add(eub,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,cs,fi,ay);}
int koala_health(int lc,int bl,int bw,int ec,int cs,int fi,int ay){return add(euh,&st.n_health,&st.t_cl,N-6,lc,bl,bw,ec,cs,fi,ay);}
int koala_market(int lc,int bl,int bw,int ec,int cs,int fi,int ay){return add(eum,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,ec,cs,fi,ay);}
void koala_report(void){ps("[KOALA] Eucalypt: ");pi(st.n_eucalypt);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Cl=");pi(st.t_cl);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void koala_state(void){ps("[KOALA] Eucalypt=");pi(st.n_eucalypt);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Koala Admin Demo ===\n\n");koala_init();
ps("Koala eucalyptus...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=70+(i*4),bw=10+(i*2),ec=6+(i*1),cs=1+(i*1),fi=(i%6)+1,ay=(i%20)+1;koala_eucalyptus(lc,bl,bw,ec,cs,fi,ay);}
ps("\nKoala feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=72+(i*3),bw=11+(i*2),ec=6+(i*1),cs=1+(i*1),fi=(i%5)+1,ay=(i%18)+1;koala_feeding(lc,bl,bw,ec,cs,fi,ay);}
ps("\nKoala breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=74+(i*3),bw=12+(i*1),ec=7+(i*1),cs=1+(i*1),fi=(i%4)+1,ay=(i%15)+1;koala_breeding(lc,bl,bw,ec,cs,fi,ay);}
ps("\nKoala health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=68+(i*5),bw=9+(i*2),ec=5+(i*1),cs=0+(i*1),fi=(i%7)+1,ay=(i%12)+1;koala_health(lc,bl,bw,ec,cs,fi,ay);}
ps("\nKoala market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=76+(i*2),bw=13+(i*1),ec=7+(i*1),cs=2+(i*1),fi=(i%3)+1,ay=(i%10)+1;koala_market(lc,bl,bw,ec,cs,fi,ay);}
ps("\n");koala_report();koala_state();ps("\n=== Demo Complete ===\n");return 0;}
