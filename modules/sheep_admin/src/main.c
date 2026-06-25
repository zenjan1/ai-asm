/* sheep_admin: Sheep (Ovis aries) woolly livestock (v1.0)
 * Sheep pasture, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wool_cm, walk_speed, fleece_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wool_cm,wk_sp,fl_idx,age_yr,active;} shep_t;
typedef struct{int n_past,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wool,t_wk,t_fl;} shep_state_t;
static shep_t spl[N],sfd[N-2],sbr[N-4],shl[N-6],smk[N-6]; static shep_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(shep_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int ws,int fi,int ay){if(*cnt>=mx)return -1;shep_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wool_cm=wc;x->wk_sp=ws;x->fl_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SHEP] Sheep ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" ws=");pi(ws);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int shep_init(void){if(init)return -1;st.n_past=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wool=0;st.t_wk=0;st.t_fl=0;for(int i=0;i<N;i++)spl[i].active=0;for(int i=0;i<N-2;i++)sfd[i].active=0;for(int i=0;i<N-4;i++)sbr[i].active=0;for(int i=0;i<N-6;i++)shl[i].active=0;for(int i=0;i<N-6;i++)smk[i].active=0;init=1;ps("[SHEP] Sheep initialized\n");return 0;}
int shep_pasture(int lc,int bl,int bw,int wc,int ws,int fi,int ay){return add(spl,&st.n_past,&st.t_ln,N,lc,bl,bw,wc,ws,fi,ay);}
int shep_feeding(int lc,int bl,int bw,int wc,int ws,int fi,int ay){return add(sfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,ws,fi,ay);}
int shep_breeding(int lc,int bl,int bw,int wc,int ws,int fi,int ay){return add(sbr,&st.n_breed,&st.t_wool,N-4,lc,bl,bw,wc,ws,fi,ay);}
int shep_health(int lc,int bl,int bw,int wc,int ws,int fi,int ay){return add(shl,&st.n_health,&st.t_wk,N-6,lc,bl,bw,wc,ws,fi,ay);}
int shep_market(int lc,int bl,int bw,int wc,int ws,int fi,int ay){return add(smk,&st.n_mkt,&st.t_fl,N-6,lc,bl,bw,wc,ws,fi,ay);}
void shep_report(void){ps("[SHEP] Pasture: ");pi(st.n_past);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wool=");pi(st.t_wool);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Fl=");pi(st.t_fl);ps("\n");}
void shep_state(void){ps("[SHEP] Past=");pi(st.n_past);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Sheep Admin Demo ===\n\n");shep_init();
ps("Sheep pasture...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*8),bw=40+(i*10),wc=5+(i%4),ws=3+(i%3),fi=(i%6)+1,ay=(i%5)+1;shep_pasture(lc,bl,bw,wc,ws,fi,ay);}
ps("\nSheep feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=90+(i*6),bw=50+(i*8),wc=6+(i%3),ws=4+(i%2),fi=(i%5)+1,ay=(i%4)+1;shep_feeding(lc,bl,bw,wc,ws,fi,ay);}
ps("\nSheep breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=70+(i*10),bw=35+(i*12),wc=4+(i%5),ws=2+(i%4),fi=(i%4)+1,ay=(i%3)+1;shep_breeding(lc,bl,bw,wc,ws,fi,ay);}
ps("\nSheep health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=100+(i*5),bw=60+(i*6),wc=7+(i%2),ws=5+(i%2),fi=(i%7)+1,ay=(i%6)+2;shep_health(lc,bl,bw,wc,ws,fi,ay);}
ps("\nSheep market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=110+(i*4),bw=70+(i*5),wc=8+(i%2),ws=2+(i%3),fi=(i%3)+3,ay=(i%4)+3;shep_market(lc,bl,bw,wc,ws,fi,ay);}
ps("\n");shep_report();shep_state();ps("\n=== Demo Complete ===\n");return 0;}
