/* manatee_admin: Manatee (Trichechus manatus) West Indian manatee sea cow (v1.0)
 * Manatee river, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, flipper_cm, dive_min, scar_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,flip_cm,dv_mn,sc_idx,age_yr,active;} mant_t;
typedef struct{int n_river,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_flip,t_dv,t_sc;} mant_state_t;
static mant_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static mant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mant_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int dm,int si,int ay){if(*cnt>=mx)return -1;mant_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->flip_cm=fc;x->dv_mn=dm;x->sc_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MANT] Manatee ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" dm=");pi(dm);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mant_init(void){if(init)return -1;st.n_river=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_flip=0;st.t_dv=0;st.t_sc=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[MANT] Manatee initialized\n");return 0;}
int mant_river(int lc,int bl,int bw,int fc,int dm,int si,int ay){return add(apl,&st.n_river,&st.t_ln,N,lc,bl,bw,fc,dm,si,ay);}
int mant_feeding(int lc,int bl,int bw,int fc,int dm,int si,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,dm,si,ay);}
int mant_breeding(int lc,int bl,int bw,int fc,int dm,int si,int ay){return add(abd,&st.n_breed,&st.t_flip,N-4,lc,bl,bw,fc,dm,si,ay);}
int mant_health(int lc,int bl,int bw,int fc,int dm,int si,int ay){return add(ahd,&st.n_health,&st.t_dv,N-6,lc,bl,bw,fc,dm,si,ay);}
int mant_market(int lc,int bl,int bw,int fc,int dm,int si,int ay){return add(amd,&st.n_mkt,&st.t_sc,N-6,lc,bl,bw,fc,dm,si,ay);}
void mant_report(void){ps("[MANT] River: ");pi(st.n_river);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Flip=");pi(st.t_flip);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Sc=");pi(st.t_sc);ps("\n");}
void mant_state(void){ps("[MANT] River=");pi(st.n_river);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Manatee Admin Demo ===\n\n");mant_init();
ps("Manatee river...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=250+(i*20),bw=300+(i*40),fc=60+(i*5),dm=10+(i*2),si=(i%8)+1,ay=2+(i%20);mant_river(lc,bl,bw,fc,dm,si,ay);}
ps("\nManatee feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=260+(i*20),bw=320+(i*40),fc=62+(i*5),dm=11+(i*2),si=(i%7)+2,ay=3+(i%18);mant_feeding(lc,bl,bw,fc,dm,si,ay);}
ps("\nManatee breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=270+(i*20),bw=340+(i*40),fc=65+(i*5),dm=12+(i*2),si=(i%6)+1,ay=4+(i%15);mant_breeding(lc,bl,bw,fc,dm,si,ay);}
ps("\nManatee health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=240+(i*25),bw=280+(i*50),fc=58+(i*6),dm=9+(i*3),si=(i%5)+3,ay=5+(i%12);mant_health(lc,bl,bw,fc,dm,si,ay);}
ps("\nManatee market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=290+(i*20),bw=360+(i*40),fc=70+(i*5),dm=14+(i*2),si=(i%4)+4,ay=6+(i%10);mant_market(lc,bl,bw,fc,dm,si,ay);}
ps("\n");mant_report();mant_state();ps("\n=== Demo Complete ===\n");return 0;}
