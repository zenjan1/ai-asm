/* peafowl_admin: Peafowl (Pavo cristatus) Indian peafowl ornamental bird (v1.0)
 * Peafowl park, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, tail_cm, fly_speed, pl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,fl_sp,pl_idx,age_yr,active;} pfow_t;
typedef struct{int n_park,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_fl,t_pl;} pfow_state_t;
static pfow_t arr_p[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static pfow_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pfow_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int fs,int pli,int ay){if(*cnt>=mx)return -1;pfow_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->fl_sp=fs;x->pl_idx=pli;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PFOW] Peafowl ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" fs=");pi(fs);ps(" pli=");pi(pli);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int peafowl_init(void){if(init)return -1;st.n_park=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_fl=0;st.t_pl=0;for(int i=0;i<N;i++)arr_p[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PFOW] Peafowl initialized\n");return 0;}
int peafowl_park(int lc,int bl,int bw,int tc,int fs,int pli,int ay){return add(arr_p,&st.n_park,&st.t_ln,N,lc,bl,bw,tc,fs,pli,ay);}
int peafowl_feeding(int lc,int bl,int bw,int tc,int fs,int pli,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,fs,pli,ay);}
int peafowl_breeding(int lc,int bl,int bw,int tc,int fs,int pli,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,fs,pli,ay);}
int peafowl_health(int lc,int bl,int bw,int tc,int fs,int pli,int ay){return add(arr_h,&st.n_health,&st.t_fl,N-6,lc,bl,bw,tc,fs,pli,ay);}
int peafowl_market(int lc,int bl,int bw,int tc,int fs,int pli,int ay){return add(arr_m,&st.n_mkt,&st.t_pl,N-6,lc,bl,bw,tc,fs,pli,ay);}
void peafowl_report(void){ps("[PFOW] Park: ");pi(st.n_park);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Pl=");pi(st.t_pl);ps("\n");}
void peafowl_state(void){ps("[PFOW] Park=");pi(st.n_park);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Peafowl Admin Demo ===\n\n");peafowl_init();
ps("Peafowl park...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=90+(i*5),bw=4+(i*1),tc=120+(i*8),fs=18+(i*3),pli=(i%7)+1,ay=(i%10)+1;peafowl_park(lc,bl,bw,tc,fs,pli,ay);}
ps("\nPeafowl feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=95+(i*4),bw=4+(i*1),tc=125+(i*6),fs=20+(i*2),pli=(i%6)+1,ay=(i%8)+1;peafowl_feeding(lc,bl,bw,tc,fs,pli,ay);}
ps("\nPeafowl breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=100+(i*3),bw=5+(i*1),tc=130+(i*5),fs=16+(i*4),pli=(i%5)+1,ay=(i%6)+1;peafowl_breeding(lc,bl,bw,tc,fs,pli,ay);}
ps("\nPeafowl health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=85+(i*6),bw=3+(i*1),tc=115+(i*9),fs=14+(i*5),pli=(i%8)+1,ay=(i%5)+1;peafowl_health(lc,bl,bw,tc,fs,pli,ay);}
ps("\nPeafowl market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=105+(i*3),bw=5+(i*1),tc=135+(i*4),fs=22+(i*3),pli=(i%4)+1,ay=(i%4)+1;peafowl_market(lc,bl,bw,tc,fs,pli,ay);}
ps("\n");peafowl_report();peafowl_state();ps("\n=== Demo Complete ===\n");return 0;}
