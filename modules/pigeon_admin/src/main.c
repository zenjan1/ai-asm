/* pigeon_admin: Pigeon (Columba livia) rock dove pigeon bird (v1.0)
 * Pigeon rooftop, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, fly_speed, ft_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,fl_sp,ft_idx,age_yr,active;} pgon_t;
typedef struct{int n_rooftop,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_fl,t_ft;} pgon_state_t;
static pgon_t arr_r[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static pgon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(pgon_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int fi,int ay){if(*cnt>=mx)return -1;pgon_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->fl_sp=fs;x->ft_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[PGON] Pigeon ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int pigeon_init(void){if(init)return -1;st.n_rooftop=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_fl=0;st.t_ft=0;for(int i=0;i<N;i++)arr_r[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[PGON] Pigeon initialized\n");return 0;}
int pigeon_rooftop(int lc,int bl,int bw,int bc,int fs,int fi,int ay){return add(arr_r,&st.n_rooftop,&st.t_ln,N,lc,bl,bw,bc,fs,fi,ay);}
int pigeon_feeding(int lc,int bl,int bw,int bc,int fs,int fi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,fi,ay);}
int pigeon_breeding(int lc,int bl,int bw,int bc,int fs,int fi,int ay){return add(arr_b,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,fs,fi,ay);}
int pigeon_health(int lc,int bl,int bw,int bc,int fs,int fi,int ay){return add(arr_h,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,fi,ay);}
int pigeon_market(int lc,int bl,int bw,int bc,int fs,int fi,int ay){return add(arr_m,&st.n_mkt,&st.t_ft,N-6,lc,bl,bw,bc,fs,fi,ay);}
void pigeon_report(void){ps("[PGON] Rooftop: ");pi(st.n_rooftop);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Ft=");pi(st.t_ft);ps("\n");}
void pigeon_state(void){ps("[PGON] Rooftop=");pi(st.n_rooftop);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Pigeon Admin Demo ===\n\n");pigeon_init();
ps("Pigeon rooftop...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=30+(i*2),bw=250+(i*20),bc=2+(i*1),fs=50+(i*5),fi=(i%6)+1,ay=(i%5)+1;pigeon_rooftop(lc,bl,bw,bc,fs,fi,ay);}
ps("\nPigeon feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=32+(i*1),bw=260+(i*15),bc=3+(i*1),fs=52+(i*4),fi=(i%5)+1,ay=(i%4)+1;pigeon_feeding(lc,bl,bw,bc,fs,fi,ay);}
ps("\nPigeon breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=34+(i*1),bw=270+(i*12),bc=3+(i*1),fs=55+(i*3),fi=(i%4)+1,ay=(i%3)+1;pigeon_breeding(lc,bl,bw,bc,fs,fi,ay);}
ps("\nPigeon health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=28+(i*3),bw=240+(i*25),bc=2+(i*1),fs=48+(i*6),fi=(i%7)+1,ay=(i%3)+1;pigeon_health(lc,bl,bw,bc,fs,fi,ay);}
ps("\nPigeon market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=36+(i*1),bw=280+(i*10),bc=4+(i*1),fs=58+(i*3),fi=(i%3)+1,ay=(i%2)+1;pigeon_market(lc,bl,bw,bc,fs,fi,ay);}
ps("\n");pigeon_report();pigeon_state();ps("\n=== Demo Complete ===\n");return 0;}
