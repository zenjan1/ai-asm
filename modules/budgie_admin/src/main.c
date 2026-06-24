/* budgie_admin: Budgie (Melopsittacus undulatus) aviary management (v1.0)
 * Budgie breeding, feeding, health check, training, market
 * Features: flock size, cage volume, seed grams, plumage color, song freq, molt week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flock_sz,cage_vol,seed_gm,plum_col,song_hz,molt_wk,active;} budg_t;
typedef struct{int n_breed,n_feed,n_health,n_train,n_mkt,t_flock,t_cage,t_seed,t_plum,t_song;} budg_state_t;
static budg_t budgps[N],budgfs[N-2],budghs[N-4],budgts[N-6],budgms[N-6]; static budg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(budg_t*a,int*cnt,int*sum,int mx,int lc,int fs,int cv,int sg,int pc,int sh,int mw){if(*cnt>=mx)return -1;budg_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flock_sz=fs;x->cage_vol=cv;x->seed_gm=sg;x->plum_col=pc;x->song_hz=sh;x->molt_wk=mw;x->active=1;*sum+=fs;(*cnt)++;ps("[BUDG] Budgie ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fs=");pi(fs);ps(" cv=");pi(cv);ps(" sg=");pi(sg);ps(" pc=");pi(pc);ps(" sh=");pi(sh);ps(" mw=");pi(mw);ps("\n");return *cnt-1;}
int budg_init(void){if(init)return -1;st.n_breed=0;st.n_feed=0;st.n_health=0;st.n_train=0;st.n_mkt=0;st.t_flock=0;st.t_cage=0;st.t_seed=0;st.t_plum=0;st.t_song=0;for(int i=0;i<N;i++)budgps[i].active=0;for(int i=0;i<N-2;i++)budgfs[i].active=0;for(int i=0;i<N-4;i++)budghs[i].active=0;for(int i=0;i<N-6;i++)budgts[i].active=0;for(int i=0;i<N-6;i++)budgms[i].active=0;init=1;ps("[BUDG] Budgie initialized\n");return 0;}
int budg_breeding(int lc,int fs,int cv,int sg,int pc,int sh,int mw){return add(budgps,&st.n_breed,&st.t_flock,N,lc,fs,cv,sg,pc,sh,mw);}
int budg_feeding(int lc,int fs,int cv,int sg,int pc,int sh,int mw){return add(budgfs,&st.n_feed,&st.t_cage,N-2,lc,fs,cv,sg,pc,sh,mw);}
int budg_health(int lc,int fs,int cv,int sg,int pc,int sh,int mw){return add(budghs,&st.n_health,&st.t_seed,N-4,lc,fs,cv,sg,pc,sh,mw);}
int budg_training(int lc,int fs,int cv,int sg,int pc,int sh,int mw){return add(budgts,&st.n_train,&st.t_plum,N-6,lc,fs,cv,sg,pc,sh,mw);}
int budg_market(int lc,int fs,int cv,int sg,int pc,int sh,int mw){return add(budgms,&st.n_mkt,&st.t_song,N-6,lc,fs,cv,sg,pc,sh,mw);}
void budg_report(void){ps("[BUDG] Breed: ");pi(st.n_breed);ps(" Flock=");pi(st.t_flock);ps("\nFeed: ");pi(st.n_feed);ps(" Cage=");pi(st.t_cage);ps("\nHealth: ");pi(st.n_health);ps(" Seed=");pi(st.t_seed);ps("\nTrain: ");pi(st.n_train);ps(" Plum=");pi(st.t_plum);ps("\nMkt: ");pi(st.n_mkt);ps(" Song=");pi(st.t_song);ps("\n");}
void budg_state(void){ps("[BUDG] Breed=");pi(st.n_breed);ps(" Feed=");pi(st.n_feed);ps(" Health=");pi(st.n_health);ps(" Train=");pi(st.n_train);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Budgie Admin Demo ===\n\n");budg_init();
/* 1=indoor 2=outdoor 3=aviary 4=breeding_room 5=show */
ps("Budgie breeding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,fs=3+(i*2),cv=50+(i*10),sg=20+(i*5),pc=(i%6)+1,sh=2000+(i*500),mw=4+(i%8);budg_breeding(lc,fs,cv,sg,pc,sh,mw);}
ps("\nBudgie feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,fs=4+(i*2),cv=60+(i*8),sg=25+(i*4),pc=(i%5)+1,sh=2200+(i*400),mw=5+(i%6);budg_feeding(lc,fs,cv,sg,pc,sh,mw);}
ps("\nBudgie health...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,fs=5+(i*2),cv=70+(i*6),sg=30+(i*3),pc=(i%4)+1,sh=2500+(i*300),mw=6+(i%4);budg_health(lc,fs,cv,sg,pc,sh,mw);}
ps("\nBudgie training...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,fs=2+(i*3),cv=40+(i*12),sg=18+(i*6),pc=(i%6)+1,sh=1800+(i*600),mw=3+(i%7);budg_training(lc,fs,cv,sg,pc,sh,mw);}
ps("\nBudgie market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,fs=6+(i*2),cv=80+(i*5),sg=35+(i*3),pc=(i%3)+4,sh=3000+(i*200),mw=7+(i%3);budg_market(lc,fs,cv,sg,pc,sh,mw);}
ps("\n");budg_report();budg_state();ps("\n=== Demo Complete ===\n");return 0;}
