/* barracuda_admin: Barracuda (Sphyraena barracuda) great predatory fish (v1.0)
 * Barracuda reef, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, fin_cm, swim_speed, tooth_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,fin_cm,sw_sp,tt_idx,age_yr,active;} bcda_t;
typedef struct{int n_reef,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_fin,t_sw,t_tt;} bcda_state_t;
static bcda_t rfl[N],rff[N-2],rfb[N-4],rfh[N-6],rfm[N-6]; static bcda_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bcda_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int ti,int ay){if(*cnt>=mx)return -1;bcda_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->fin_cm=fc;x->sw_sp=ss;x->tt_idx=ti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[BCDA] Barracuda ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" ti=");pi(ti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int barracuda_init(void){if(init)return -1;st.n_reef=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_fin=0;st.t_sw=0;st.t_tt=0;for(int i=0;i<N;i++)rfl[i].active=0;for(int i=0;i<N-2;i++)rff[i].active=0;for(int i=0;i<N-4;i++)rfb[i].active=0;for(int i=0;i<N-6;i++)rfh[i].active=0;for(int i=0;i<N-6;i++)rfm[i].active=0;init=1;ps("[BCDA] Barracuda initialized\n");return 0;}
int barracuda_reef(int lc,int bl,int bw,int fc,int ss,int ti,int ay){return add(rfl,&st.n_reef,&st.t_ln,N,lc,bl,bw,fc,ss,ti,ay);}
int barracuda_feeding(int lc,int bl,int bw,int fc,int ss,int ti,int ay){return add(rff,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,ti,ay);}
int barracuda_breeding(int lc,int bl,int bw,int fc,int ss,int ti,int ay){return add(rfb,&st.n_breed,&st.t_fin,N-4,lc,bl,bw,fc,ss,ti,ay);}
int barracuda_health(int lc,int bl,int bw,int fc,int ss,int ti,int ay){return add(rfh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,ti,ay);}
int barracuda_market(int lc,int bl,int bw,int fc,int ss,int ti,int ay){return add(rfm,&st.n_mkt,&st.t_tt,N-6,lc,bl,bw,fc,ss,ti,ay);}
void barracuda_report(void){ps("[BCDA] Reef: ");pi(st.n_reef);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Fin=");pi(st.t_fin);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Tt=");pi(st.t_tt);ps("\n");}
void barracuda_state(void){ps("[BCDA] Reef=");pi(st.n_reef);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Barracuda Admin Demo ===\n\n");barracuda_init();
ps("Barracuda reef...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*8),bw=15+(i*5),fc=20+(i*3),ss=30+(i*4),tt=(i%6)+1,ay=(i%10)+1;barracuda_reef(lc,bl,bw,fc,ss,tt,ay);}
ps("\nBarracuda feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*7),bw=18+(i*4),fc=22+(i*2),ss=35+(i*3),tt=(i%5)+1,ay=(i%8)+1;barracuda_feeding(lc,bl,bw,fc,ss,tt,ay);}
ps("\nBarracuda breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*6),bw=20+(i*3),fc=24+(i*2),ss=25+(i*5),tt=(i%4)+1,ay=(i%7)+1;barracuda_breeding(lc,bl,bw,fc,ss,tt,ay);}
ps("\nBarracuda health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=98+(i*9),bw=14+(i*6),fc=19+(i*4),ss=20+(i*6),tt=(i%8)+1,ay=(i%6)+1;barracuda_health(lc,bl,bw,fc,ss,tt,ay);}
ps("\nBarracuda market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=115+(i*5),bw=22+(i*3),fc=25+(i*2),ss=40+(i*3),tt=(i%3)+1,ay=(i%5)+1;barracuda_market(lc,bl,bw,fc,ss,tt,ay);}
ps("\n");barracuda_report();barracuda_state();ps("\n=== Demo Complete ===\n");return 0;}
