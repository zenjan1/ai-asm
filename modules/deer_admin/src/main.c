/* deer_admin: Deer (Cervus elaphus) red deer forest ungulate (v1.0)
 * Deer forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, antler_cm, run_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,antler_cm,run_sp,hd_idx,age_yr,active;} deer_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_antl,t_run,t_hd;} deer_state_t;
static deer_t frl[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static deer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(deer_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ac,int rs,int hi,int ay){if(*cnt>=mx)return -1;deer_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->antler_cm=ac;x->run_sp=rs;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DEER] Deer ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ac=");pi(ac);ps(" rs=");pi(rs);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int deer_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_antl=0;st.t_run=0;st.t_hd=0;for(int i=0;i<N;i++)frl[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[DEER] Deer initialized\n");return 0;}
int deer_forest(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(frl,&st.n_forest,&st.t_ln,N,lc,bl,bw,ac,rs,hi,ay);}
int deer_feeding(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ac,rs,hi,ay);}
int deer_breeding(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(frb,&st.n_breed,&st.t_antl,N-4,lc,bl,bw,ac,rs,hi,ay);}
int deer_health(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(frh,&st.n_health,&st.t_run,N-6,lc,bl,bw,ac,rs,hi,ay);}
int deer_market(int lc,int bl,int bw,int ac,int rs,int hi,int ay){return add(frm,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,ac,rs,hi,ay);}
void deer_report(void){ps("[DEER] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Antl=");pi(st.t_antl);ps("\nHealth: ");pi(st.n_health);ps(" Run=");pi(st.t_run);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void deer_state(void){ps("[DEER] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Deer Admin Demo ===\n\n");deer_init();
ps("Deer forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*8),bw=100+(i*20),ac=30+(i*5),rs=35+(i*4),hi=(i%8)+1,ay=(i%15)+1;deer_forest(lc,bl,bw,ac,rs,hi,ay);}
ps("\nDeer feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*6),bw=110+(i*15),ac=33+(i*4),rs=37+(i*3),hi=(i%6)+1,ay=(i%12)+1;deer_feeding(lc,bl,bw,ac,rs,hi,ay);}
ps("\nDeer breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*5),bw=120+(i*12),ac=36+(i*3),rs=33+(i*5),hi=(i%5)+1,ay=(i%10)+1;deer_breeding(lc,bl,bw,ac,rs,hi,ay);}
ps("\nDeer health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=145+(i*10),bw=95+(i*25),ac=28+(i*6),rs=30+(i*6),hi=(i%10)+1,ay=(i%8)+1;deer_health(lc,bl,bw,ac,rs,hi,ay);}
ps("\nDeer market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*4),bw=130+(i*10),ac=38+(i*3),rs=40+(i*3),hi=(i%4)+1,ay=(i%7)+1;deer_market(lc,bl,bw,ac,rs,hi,ay);}
ps("\n");deer_report();deer_state();ps("\n=== Demo Complete ===\n");return 0;}
