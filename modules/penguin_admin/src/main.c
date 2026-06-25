/* penguin_admin: Penguin (Aptenodytes forsteri) emperor penguin flightless seabird (v1.0)
 * Penguin ice, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, flipper_cm, dive_m, colony_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,flip_cm,dv_m,cl_idx,age_yr,active;} png_t;
typedef struct{int n_ice,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_flip,t_dv,t_cl;} png_state_t;
static png_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static png_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(png_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int dm,int ci,int ay){if(*cnt>=mx)return -1;png_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->flip_cm=fc;x->dv_m=dm;x->cl_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PNGN] Penguin ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" dm=");pi(dm);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int png_init(void){if(init)return -1;st.n_ice=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_flip=0;st.t_dv=0;st.t_cl=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[PNGN] Penguin initialized\n");return 0;}
int png_ice(int lc,int bl,int bw,int fc,int dm,int ci,int ay){return add(apl,&st.n_ice,&st.t_ln,N,lc,bl,bw,fc,dm,ci,ay);}
int png_feeding(int lc,int bl,int bw,int fc,int dm,int ci,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,dm,ci,ay);}
int png_breeding(int lc,int bl,int bw,int fc,int dm,int ci,int ay){return add(abd,&st.n_breed,&st.t_flip,N-4,lc,bl,bw,fc,dm,ci,ay);}
int png_health(int lc,int bl,int bw,int fc,int dm,int ci,int ay){return add(ahd,&st.n_health,&st.t_dv,N-6,lc,bl,bw,fc,dm,ci,ay);}
int png_market(int lc,int bl,int bw,int fc,int dm,int ci,int ay){return add(amd,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,fc,dm,ci,ay);}
void png_report(void){ps("[PNGN] Ice: ");pi(st.n_ice);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Flip=");pi(st.t_flip);ps("\nHealth: ");pi(st.n_health);ps(" Dv=");pi(st.t_dv);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void png_state(void){ps("[PNGN] Ice=");pi(st.n_ice);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Penguin Admin Demo ===\n\n");png_init();
ps("Penguin ice...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=100+(i*6),bw=30+(i*5),fc=20+(i*3),dm=200+(i*30),ci=(i%8)+1,ay=2+(i%20);png_ice(lc,bl,bw,fc,dm,ci,ay);}
ps("\nPenguin feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=105+(i*6),bw=32+(i*5),fc=21+(i*3),dm=210+(i*30),ci=(i%7)+2,ay=3+(i%18);png_feeding(lc,bl,bw,fc,dm,ci,ay);}
ps("\nPenguin breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=110+(i*6),bw=35+(i*5),fc=22+(i*3),dm=220+(i*30),ci=(i%6)+1,ay=4+(i%15);png_breeding(lc,bl,bw,fc,dm,ci,ay);}
ps("\nPenguin health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=95+(i*8),bw=28+(i*6),fc=19+(i*4),dm=190+(i*35),ci=(i%5)+3,ay=5+(i%12);png_health(lc,bl,bw,fc,dm,ci,ay);}
ps("\nPenguin market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=120+(i*6),bw=38+(i*5),fc=24+(i*3),dm=240+(i*30),ci=(i%4)+4,ay=6+(i%10);png_market(lc,bl,bw,fc,dm,ci,ay);}
ps("\n");png_report();png_state();ps("\n=== Demo Complete ===\n");return 0;}
