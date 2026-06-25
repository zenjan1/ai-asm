/* boar_admin: Boar (Sus scrofa) wild boar swine (v1.0)
 * Boar forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tusk_cm, run_speed, bristle_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tusk_cm,run_sp,br_idx,age_yr,active;} boar_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tusk,t_run,t_br;} boar_state_t;
static boar_t frl[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static boar_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(boar_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int bi,int ay){if(*cnt>=mx)return -1;boar_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tusk_cm=tc;x->run_sp=rs;x->br_idx=bi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BOAR] Boar ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" bi=");pi(bi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int boar_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tusk=0;st.t_run=0;st.t_br=0;for(int i=0;i<N;i++)frl[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[BOAR] Boar initialized\n");return 0;}
int boar_forest(int lc,int bl,int bw,int tc,int rs,int bi,int ay){return add(frl,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,rs,bi,ay);}
int boar_feeding(int lc,int bl,int bw,int tc,int rs,int bi,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,bi,ay);}
int boar_breeding(int lc,int bl,int bw,int tc,int rs,int bi,int ay){return add(frb,&st.n_breed,&st.t_tusk,N-4,lc,bl,bw,tc,rs,bi,ay);}
int boar_health(int lc,int bl,int bw,int tc,int rs,int bi,int ay){return add(frh,&st.n_health,&st.t_run,N-6,lc,bl,bw,tc,rs,bi,ay);}
int boar_market(int lc,int bl,int bw,int tc,int rs,int bi,int ay){return add(frm,&st.n_mkt,&st.t_br,N-6,lc,bl,bw,tc,rs,bi,ay);}
void boar_report(void){ps("[BOAR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tusk=");pi(st.t_tusk);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Br=");pi(st.t_br);ps("\n");}
void boar_state(void){ps("[BOAR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Boar Admin Demo ===\n\n");boar_init();
ps("Boar forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*8),bw=50+(i*15),tc=10+(i*3),rs=20+(i*4),bi=(i%8)+1,ay=(i%12)+1;boar_forest(lc,bl,bw,tc,rs,bi,ay);}
ps("\nBoar feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*7),bw=55+(i*12),tc=12+(i*2),rs=22+(i*3),bi=(i%6)+1,ay=(i%10)+1;boar_feeding(lc,bl,bw,tc,rs,bi,ay);}
ps("\nBoar breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*6),bw=60+(i*10),tc=14+(i*2),rs=18+(i*5),bi=(i%5)+1,ay=(i%8)+1;boar_breeding(lc,bl,bw,tc,rs,bi,ay);}
ps("\nBoar health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*9),bw=48+(i*18),tc=9+(i*4),rs=15+(i*6),bi=(i%10)+1,ay=(i%7)+1;boar_health(lc,bl,bw,tc,rs,bi,ay);}
ps("\nBoar market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=115+(i*5),bw=65+(i*8),tc=15+(i*2),rs=25+(i*3),bi=(i%4)+1,ay=(i%6)+1;boar_market(lc,bl,bw,tc,rs,bi,ay);}
ps("\n");boar_report();boar_state();ps("\n=== Demo Complete ===\n");return 0;}
