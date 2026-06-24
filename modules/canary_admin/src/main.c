/* canary_admin: Canary (Serinus canaria) songbird aviary management (v1.0)
 * Canary breeding, song training, health, exhibition, market
 * Features: song length, plumage color, cage volume, seed gram, nest count, molt week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,song_len,plum_col,cage_vol,seed_gm,nest_ct,molt_wk,active;} can_t;
typedef struct{int n_breed,n_song,n_health,n_exhibit,n_mkt,t_song,t_plum,t_cage,t_seed,t_nest;} can_state_t;
static can_t canps[N],canst[N-2],canhs[N-4],canex[N-6],canms[N-6]; static can_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(can_t*a,int*cnt,int*sum,int mx,int lc,int sl,int pc,int cv,int sg,int nc,int mw){if(*cnt>=mx)return -1;can_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->song_len=sl;x->plum_col=pc;x->cage_vol=cv;x->seed_gm=sg;x->nest_ct=nc;x->molt_wk=mw;x->active=1;*sum+=sl;(*cnt)++;ps("[CAN] Canary ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sl=");pi(sl);ps(" pc=");pi(pc);ps(" cv=");pi(cv);ps(" sg=");pi(sg);ps(" nc=");pi(nc);ps(" mw=");pi(mw);ps("\n");return *cnt-1;}
int can_init(void){if(init)return -1;st.n_breed=0;st.n_song=0;st.n_health=0;st.n_exhibit=0;st.n_mkt=0;st.t_song=0;st.t_plum=0;st.t_cage=0;st.t_seed=0;st.t_nest=0;for(int i=0;i<N;i++)canps[i].active=0;for(int i=0;i<N-2;i++)canst[i].active=0;for(int i=0;i<N-4;i++)canhs[i].active=0;for(int i=0;i<N-6;i++)canex[i].active=0;for(int i=0;i<N-6;i++)canms[i].active=0;init=1;ps("[CAN] Canary initialized\n");return 0;}
int can_breeding(int lc,int sl,int pc,int cv,int sg,int nc,int mw){return add(canps,&st.n_breed,&st.t_song,N,lc,sl,pc,cv,sg,nc,mw);}
int can_song_training(int lc,int sl,int pc,int cv,int sg,int nc,int mw){return add(canst,&st.n_song,&st.t_plum,N-2,lc,sl,pc,cv,sg,nc,mw);}
int can_health(int lc,int sl,int pc,int cv,int sg,int nc,int mw){return add(canhs,&st.n_health,&st.t_cage,N-4,lc,sl,pc,cv,sg,nc,mw);}
int can_exhibition(int lc,int sl,int pc,int cv,int sg,int nc,int mw){return add(canex,&st.n_exhibit,&st.t_seed,N-6,lc,sl,pc,cv,sg,nc,mw);}
int can_market(int lc,int sl,int pc,int cv,int sg,int nc,int mw){return add(canms,&st.n_mkt,&st.t_nest,N-6,lc,sl,pc,cv,sg,nc,mw);}
void can_report(void){ps("[CAN] Breed: ");pi(st.n_breed);ps(" Song=");pi(st.t_song);ps("\nSong: ");pi(st.n_song);ps(" Plum=");pi(st.t_plum);ps("\nHealth: ");pi(st.n_health);ps(" Cage=");pi(st.t_cage);ps("\nExhibit: ");pi(st.n_exhibit);ps(" Seed=");pi(st.t_seed);ps("\nMkt: ");pi(st.n_mkt);ps(" Nest=");pi(st.t_nest);ps("\n");}
void can_state(void){ps("[CAN] Breed=");pi(st.n_breed);ps(" Song=");pi(st.n_song);ps(" Health=");pi(st.n_health);ps(" Exhibit=");pi(st.n_exhibit);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Canary Admin Demo ===\n\n");can_init();
/* 1=aviary 2=indoor 3=show_room 4=breeding 5=home */
ps("Canary breeding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sl=30+(i*10),pc=(i%6)+1,cv=20+(i*5),sg=10+(i*3),nc=1+(i%4),mw=6+(i%8);can_breeding(lc,sl,pc,cv,sg,nc,mw);}
ps("\nCanary song training...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sl=35+(i*8),pc=(i%5)+1,cv=25+(i*4),sg=12+(i*2),nc=2+(i%3),mw=8+(i%6);can_song_training(lc,sl,pc,cv,sg,nc,mw);}
ps("\nCanary health...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sl=40+(i*6),pc=(i%4)+1,cv=30+(i*3),sg=15+(i*2),nc=3+(i%2),mw=10+(i%4);can_health(lc,sl,pc,cv,sg,nc,mw);}
ps("\nCanary exhibition...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sl=25+(i*12),pc=(i%6)+1,cv=18+(i*6),sg=8+(i*4),nc=1+(i%4),mw=4+(i%9);can_exhibition(lc,sl,pc,cv,sg,nc,mw);}
ps("\nCanary market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sl=45+(i*5),pc=(i%3)+4,cv=35+(i*3),sg=18+(i*2),nc=4+(i%2),mw=12+(i%3);can_market(lc,sl,pc,cv,sg,nc,mw);}
ps("\n");can_report();can_state();ps("\n=== Demo Complete ===\n");return 0;}
