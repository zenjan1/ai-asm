/* cat_admin: Cat (Felis catus) domestic feline predator (v1.0)
 * Cat household, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, claw_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,claw_cm,run_sp,fr_idx,age_yr,active;} cat_t;
typedef struct{int n_house,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_claw,t_run,t_fr;} cat_state_t;
static cat_t hsl[N],hsf[N-2],hsb[N-4],hsh[N-6],hsm[N-6]; static cat_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cat_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int cc,int rs,int fi,int ay){if(*cnt>=mx)return -1;cat_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->claw_cm=cc;x->run_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CAT] Cat ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" cc=");pi(cc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cat_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_claw=0;st.t_run=0;st.t_fr=0;for(int i=0;i<N;i++)hsl[i].active=0;for(int i=0;i<N-2;i++)hsf[i].active=0;for(int i=0;i<N-4;i++)hsb[i].active=0;for(int i=0;i<N-6;i++)hsh[i].active=0;for(int i=0;i<N-6;i++)hsm[i].active=0;init=1;ps("[CAT] Cat initialized\n");return 0;}
int cat_household(int lc,int bl,int bw,int cc,int rs,int fi,int ay){return add(hsl,&st.n_house,&st.t_ln,N,lc,bl,bw,cc,rs,fi,ay);}
int cat_feeding(int lc,int bl,int bw,int cc,int rs,int fi,int ay){return add(hsf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,cc,rs,fi,ay);}
int cat_breeding(int lc,int bl,int bw,int cc,int rs,int fi,int ay){return add(hsb,&st.n_breed,&st.t_claw,N-4,lc,bl,bw,cc,rs,fi,ay);}
int cat_health(int lc,int bl,int bw,int cc,int rs,int fi,int ay){return add(hsh,&st.n_health,&st.t_run,N-6,lc,bl,bw,cc,rs,fi,ay);}
int cat_market(int lc,int bl,int bw,int cc,int rs,int fi,int ay){return add(hsm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,cc,rs,fi,ay);}
void cat_report(void){ps("[CAT] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Claw=");pi(st.t_claw);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void cat_state(void){ps("[CAT] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cat Admin Demo ===\n\n");cat_init();
ps("Cat household...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=3+(i*1),cc=1+(i%3),rs=15+(i*3),fi=(i%8)+1,ay=(i%15)+1;cat_household(lc,bl,bw,cc,rs,fi,ay);}
ps("\nCat feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=4+(i*1),cc=1+(i%2),rs=16+(i*2),fi=(i%6)+1,ay=(i%12)+1;cat_feeding(lc,bl,bw,cc,rs,fi,ay);}
ps("\nCat breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=4+(i*1),cc=1+(i%4),rs=14+(i*4),fi=(i%5)+1,ay=(i%10)+1;cat_breeding(lc,bl,bw,cc,rs,fi,ay);}
ps("\nCat health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=38+(i*4),bw=3+(i*2),cc=1+(i%5),rs=12+(i*5),fi=(i%10)+1,ay=(i%8)+1;cat_health(lc,bl,bw,cc,rs,fi,ay);}
ps("\nCat market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*2),bw=5+(i*1),cc=2+(i%3),rs=18+(i*3),fi=(i%4)+1,ay=(i%7)+1;cat_market(lc,bl,bw,cc,rs,fi,ay);}
ps("\n");cat_report();cat_state();ps("\n=== Demo Complete ===\n");return 0;}
