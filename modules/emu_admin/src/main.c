/* emu_admin: Emu (Dromaius novaehollandiae) emu flightless bird farming (v1.0)
 * Emu housing, feeding, breeding, harvest, market
 * Features: body_len_cm, body_wt_kg, leg_len_cm, feather_idx, egg_wt_g, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,leg_ln,fthr_idx,egg_wt,age_yr,active;} emu_t;
typedef struct{int n_house,n_feed,n_breed,n_harv,n_mkt,t_ln,t_wt,t_leg,t_ft,t_eg;} emu_state_t;
static emu_t ehs[N],efd[N-2],ebr[N-4],ehv[N-6],emk[N-6]; static emu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(emu_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ll,int fi,int ew,int ay){if(*cnt>=mx)return -1;emu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->leg_ln=ll;x->fthr_idx=fi;x->egg_wt=ew;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[EMU] Emu ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ll=");pi(ll);ps(" fi=");pi(fi);ps(" ew=");pi(ew);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int emu_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_harv=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_leg=0;st.t_ft=0;st.t_eg=0;for(int i=0;i<N;i++)ehs[i].active=0;for(int i=0;i<N-2;i++)efd[i].active=0;for(int i=0;i<N-4;i++)ebr[i].active=0;for(int i=0;i<N-6;i++)ehv[i].active=0;for(int i=0;i<N-6;i++)emk[i].active=0;init=1;ps("[EMU] Emu initialized\n");return 0;}
int emu_housing(int lc,int bl,int bw,int ll,int fi,int ew,int ay){return add(ehs,&st.n_house,&st.t_ln,N,lc,bl,bw,ll,fi,ew,ay);}
int emu_feeding(int lc,int bl,int bw,int ll,int fi,int ew,int ay){return add(efd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ll,fi,ew,ay);}
int emu_breeding(int lc,int bl,int bw,int ll,int fi,int ew,int ay){return add(ebr,&st.n_breed,&st.t_leg,N-4,lc,bl,bw,ll,fi,ew,ay);}
int emu_harvest(int lc,int bl,int bw,int ll,int fi,int ew,int ay){return add(ehv,&st.n_harv,&st.t_ft,N-6,lc,bl,bw,ll,fi,ew,ay);}
int emu_market(int lc,int bl,int bw,int ll,int fi,int ew,int ay){return add(emk,&st.n_mkt,&st.t_eg,N-6,lc,bl,bw,ll,fi,ew,ay);}
void emu_report(void){ps("[EMU] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Leg=");pi(st.t_leg);ps("\nHarv: ");pi(st.n_harv);ps(" Ft=");pi(st.t_ft);ps("\nMkt: ");pi(st.n_mkt);ps(" Eg=");pi(st.t_eg);ps("\n");}
void emu_state(void){ps("[EMU] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Emu Admin Demo ===\n\n");emu_init();
/* 1=farm 2=paddock 3=ranch 4=field 5=market */
ps("Emu housing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=150+(i*8),bw=35+(i*3),ll=45+(i*4),fi=(i%6)+1,ew=500+(i*30),ay=1+(i%8);emu_housing(lc,bl,bw,ll,fi,ew,ay);}
ps("\nEmu feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=155+(i*7),bw=38+(i*3),ll=48+(i*3),fi=(i%5)+1,ew=530+(i*25),ay=2+(i%6);emu_feeding(lc,bl,bw,ll,fi,ew,ay);}
ps("\nEmu breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=160+(i*6),bw=40+(i*3),ll=50+(i*3),fi=(i%4)+1,ew=560+(i*20),ay=2+(i%5);emu_breeding(lc,bl,bw,ll,fi,ew,ay);}
ps("\nEmu harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=145+(i*9),bw=33+(i*4),ll=43+(i*5),fi=(i%7)+1,ew=480+(i*35),ay=3+(i%7);emu_harvest(lc,bl,bw,ll,fi,ew,ay);}
ps("\nEmu market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=165+(i*5),bw=42+(i*2),ll=52+(i*3),fi=(i%3)+4,ew=590+(i*15),ay=3+(i%4);emu_market(lc,bl,bw,ll,fi,ew,ay);}
ps("\n");emu_report();emu_state();ps("\n=== Demo Complete ===\n");return 0;}
