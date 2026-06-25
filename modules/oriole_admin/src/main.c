/* oriole_admin: Oriole (Icterus galbula) Baltimore oriole songbird (v1.0)
 * Oriole canopy, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, wing_cm, song_vol, nest_hgt, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,sg_vl,ns_ht,age_yr,active;} orio_t;
typedef struct{int n_canopy,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_sg,t_ns;} orio_state_t;
static orio_t apl[N],afd[N-2],abd[N-4],ahd[N-6],amd[N-6]; static orio_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(orio_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int sv,int nh,int ay){if(*cnt>=mx)return -1;orio_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->sg_vl=sv;x->ns_ht=nh;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ORIO] Oriole ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" sv=");pi(sv);ps(" nh=");pi(nh);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int orio_init(void){if(init)return -1;st.n_canopy=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_sg=0;st.t_ns=0;for(int i=0;i<N;i++)apl[i].active=0;for(int i=0;i<N-2;i++)afd[i].active=0;for(int i=0;i<N-4;i++)abd[i].active=0;for(int i=0;i<N-6;i++)ahd[i].active=0;for(int i=0;i<N-6;i++)amd[i].active=0;init=1;ps("[ORIO] Oriole initialized\n");return 0;}
int orio_canopy(int lc,int bl,int bw,int wc,int sv,int nh,int ay){return add(apl,&st.n_canopy,&st.t_ln,N,lc,bl,bw,wc,sv,nh,ay);}
int orio_feeding(int lc,int bl,int bw,int wc,int sv,int nh,int ay){return add(afd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,sv,nh,ay);}
int orio_breeding(int lc,int bl,int bw,int wc,int sv,int nh,int ay){return add(abd,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,sv,nh,ay);}
int orio_health(int lc,int bl,int bw,int wc,int sv,int nh,int ay){return add(ahd,&st.n_health,&st.t_sg,N-6,lc,bl,bw,wc,sv,nh,ay);}
int orio_market(int lc,int bl,int bw,int wc,int sv,int nh,int ay){return add(amd,&st.n_mkt,&st.t_ns,N-6,lc,bl,bw,wc,sv,nh,ay);}
void orio_report(void){ps("[ORIO] Canopy: ");pi(st.n_canopy);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Sg=");pi(st.t_sg);ps("\nMkt: ");pi(st.n_mkt);ps(" Ns=");pi(st.t_ns);ps("\n");}
void orio_state(void){ps("[ORIO] Canopy=");pi(st.n_canopy);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Oriole Admin Demo ===\n\n");orio_init();
ps("Oriole canopy...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=16+(i*2),bw=30+(i*5),wc=8+(i*2),sv=40+(i*5),nh=500+(i*50),ay=1+(i%6);orio_canopy(lc,bl,bw,wc,sv,nh,ay);}
ps("\nOriole feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=17+(i*2),bw=32+(i*5),wc=9+(i*2),sv=42+(i*5),nh=520+(i*50),ay=1+(i%5);orio_feeding(lc,bl,bw,wc,sv,nh,ay);}
ps("\nOriole breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=18+(i*2),bw=35+(i*5),wc=9+(i*2),sv=45+(i*5),nh=550+(i*50),ay=2+(i%4);orio_breeding(lc,bl,bw,wc,sv,nh,ay);}
ps("\nOriole health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=15+(i*3),bw=28+(i*6),wc=7+(i*3),sv=38+(i*6),nh=480+(i*60),ay=2+(i%3);orio_health(lc,bl,bw,wc,sv,nh,ay);}
ps("\nOriole market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=20+(i*2),bw=38+(i*5),wc=11+(i*2),sv=50+(i*5),nh=600+(i*50),ay=3+(i%3);orio_market(lc,bl,bw,wc,sv,nh,ay);}
ps("\n");orio_report();orio_state();ps("\n=== Demo Complete ===\n");return 0;}
