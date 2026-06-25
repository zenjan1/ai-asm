/* rock_wallaby_admin: Rock-wallaby (Petrogale) single species (v1.0)
 * Rock-wallaby forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, jump_ht, tail_cm, rwb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,jump_ht,tail_cm,rwb_idx,age_yr,active;} rklw_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_jump,t_tail,t_rw;} rklw_state_t;
static rklw_t svl[N],svf[N-2],svb[N-4],svh[N-6],svm[N-6]; static rklw_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rklw_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int jh,int tc,int rw,int ay){if(*cnt>=mx)return -1;rklw_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->jump_ht=jh;x->tail_cm=tc;x->rwb_idx=rw;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[RKLW] Rock-wallaby ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" jh=");pi(jh);ps(" tc=");pi(tc);ps(" rw=");pi(rw);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int rock_wallaby_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_jump=0;st.t_tail=0;st.t_rw=0;for(int i=0;i<N;i++)svl[i].active=0;for(int i=0;i<N-2;i++)svf[i].active=0;for(int i=0;i<N-4;i++)svb[i].active=0;for(int i=0;i<N-6;i++)svh[i].active=0;for(int i=0;i<N-6;i++)svm[i].active=0;init=1;ps("[RKLW] Rock-wallaby initialized\n");return 0;}
int rock_wallaby_forest(int lc,int bl,int bw,int jh,int tc,int rw,int ay){return add(svl,&st.n_forest,&st.t_ln,N,lc,bl,bw,jh,tc,rw,ay);}
int rock_wallaby_feeding(int lc,int bl,int bw,int jh,int tc,int rw,int ay){return add(svf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,jh,tc,rw,ay);}
int rock_wallaby_breeding(int lc,int bl,int bw,int jh,int tc,int rw,int ay){return add(svb,&st.n_breed,&st.t_jump,N-4,lc,bl,bw,jh,tc,rw,ay);}
int rock_wallaby_health(int lc,int bl,int bw,int jh,int tc,int rw,int ay){return add(svh,&st.n_health,&st.t_tail,N-6,lc,bl,bw,jh,tc,rw,ay);}
int rock_wallaby_market(int lc,int bl,int bw,int jh,int tc,int rw,int ay){return add(svm,&st.n_mkt,&st.t_rw,N-6,lc,bl,bw,jh,tc,rw,ay);}
void rock_wallaby_report(void){ps("[RKLW] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Jump=");pi(st.t_jump);ps("\nHealth: ");pi(st.n_health);ps(" Tail=");pi(st.t_tail);ps("\nMkt: ");pi(st.n_mkt);ps(" Rw=");pi(st.t_rw);ps("\n");}
void rock_wallaby_state(void){ps("[RKLW] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rock-Wallaby Admin Demo ===\n\n");rock_wallaby_init();
ps("Rock-wallaby forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=42+(i*2),bw=3500+(i*250),jh=120+(i*15),tc=38+(i*2),rw=(i%8)+1,ay=(i%5)+1;rock_wallaby_forest(lc,bl,bw,jh,tc,rw,ay);}
ps("\nRock-wallaby feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=44+(i*2),bw=3700+(i*200),jh=130+(i*15),tc=39+(i*2),rw=(i%6)+1,ay=(i%4)+1;rock_wallaby_feeding(lc,bl,bw,jh,tc,rw,ay);}
ps("\nRock-wallaby breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=46+(i*2),bw=3900+(i*150),jh=140+(i*15),tc=40+(i*2),rw=(i%5)+1,ay=(i%3)+1;rock_wallaby_breeding(lc,bl,bw,jh,tc,rw,ay);}
ps("\nRock-wallaby health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3300+(i*300),jh=110+(i*15),tc=37+(i*2),rw=(i%10)+1,ay=(i%5)+1;rock_wallaby_health(lc,bl,bw,jh,tc,rw,ay);}
ps("\nRock-wallaby market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=48+(i*2),bw=4100+(i*150),jh=150+(i*15),tc=41+(i*2),rw=(i%4)+1,ay=(i%3)+1;rock_wallaby_market(lc,bl,bw,jh,tc,rw,ay);}
ps("\n");rock_wallaby_report();rock_wallaby_state();ps("\n=== Demo Complete ===\n");return 0;}
