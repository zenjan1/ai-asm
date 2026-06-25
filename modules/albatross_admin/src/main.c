/* albatross_admin: Albatross (Diomedea exulans) wandering oceanic seabird (v1.0)
 * Albatross ocean, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, wing_cm, fly_speed, feather_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,wing_cm,fl_sp,ft_idx,age_yr,active;} albt_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_wing,t_fl,t_ft;} albt_state_t;
static albt_t ocl[N],ocf[N-2],ocb[N-4],och[N-6],ocm[N-6]; static albt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(albt_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int wc,int fs,int fi,int ay){if(*cnt>=mx)return -1;albt_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->wing_cm=wc;x->fl_sp=fs;x->ft_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ALBT] Albatross ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" wc=");pi(wc);ps(" fs=");pi(fs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int albatross_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_wing=0;st.t_fl=0;st.t_ft=0;for(int i=0;i<N;i++)ocl[i].active=0;for(int i=0;i<N-2;i++)ocf[i].active=0;for(int i=0;i<N-4;i++)ocb[i].active=0;for(int i=0;i<N-6;i++)och[i].active=0;for(int i=0;i<N-6;i++)ocm[i].active=0;init=1;ps("[ALBT] Albatross initialized\n");return 0;}
int albatross_ocean(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ocl,&st.n_ocean,&st.t_ln,N,lc,bl,bw,wc,fs,fi,ay);}
int albatross_feeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ocf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,wc,fs,fi,ay);}
int albatross_breeding(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ocb,&st.n_breed,&st.t_wing,N-4,lc,bl,bw,wc,fs,fi,ay);}
int albatross_health(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(och,&st.n_health,&st.t_fl,N-6,lc,bl,bw,wc,fs,fi,ay);}
int albatross_market(int lc,int bl,int bw,int wc,int fs,int fi,int ay){return add(ocm,&st.n_mkt,&st.t_ft,N-6,lc,bl,bw,wc,fs,fi,ay);}
void albatross_report(void){ps("[ALBT] Ocean: ");pi(st.n_ocean);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Wing=");pi(st.t_wing);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ft=");pi(st.t_ft);ps("\n");}
void albatross_state(void){ps("[ALBT] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Albatross Admin Demo ===\n\n");albatross_init();
ps("Albatross ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=80+(i*5),bw=400+(i*50),wc=250+(i*10),fs=50+(i*3),fi=(i%12)+1,ay=(i%20)+1;albatross_ocean(lc,bl,bw,wc,fs,fi,ay);}
ps("\nAlbatross feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=85+(i*4),bw=420+(i*40),wc=260+(i*8),fs=55+(i*2),fi=(i%10)+1,ay=(i%18)+1;albatross_feeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nAlbatross breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=90+(i*3),bw=450+(i*30),wc=270+(i*6),fs=45+(i*4),fi=(i%8)+1,ay=(i%15)+1;albatross_breeding(lc,bl,bw,wc,fs,fi,ay);}
ps("\nAlbatross health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=78+(i*6),bw=380+(i*60),wc=240+(i*12),fs=40+(i*5),fi=(i%14)+1,ay=(i%12)+1;albatross_health(lc,bl,bw,wc,fs,fi,ay);}
ps("\nAlbatross market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=95+(i*3),bw=500+(i*25),wc=280+(i*5),fs=60+(i*2),fi=(i%6)+1,ay=(i%10)+1;albatross_market(lc,bl,bw,wc,fs,fi,ay);}
ps("\n");albatross_report();albatross_state();ps("\n=== Demo Complete ===\n");return 0;}
