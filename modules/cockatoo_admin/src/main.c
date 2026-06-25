/* cockatoo_admin: Cockatoo (Cacatua galerita) sulphur-crested parrot (v1.0)
 * Cockatoo aviary, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, beak_cm, fly_speed, crest_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,beak_cm,fl_sp,cr_idx,age_yr,active;} ckto_t;
typedef struct{int n_aviary,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_beak,t_fl,t_cr;} ckto_state_t;
static ckto_t avl[N],avf[N-2],avb[N-4],avh[N-6],avm[N-6]; static ckto_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ckto_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int bc,int fs,int ci,int ay){if(*cnt>=mx)return -1;ckto_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->beak_cm=bc;x->fl_sp=fs;x->cr_idx=ci;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[CKTO] Cockatoo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" fs=");pi(fs);ps(" ci=");pi(ci);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cockatoo_init(void){if(init)return -1;st.n_aviary=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_beak=0;st.t_fl=0;st.t_cr=0;for(int i=0;i<N;i++)avl[i].active=0;for(int i=0;i<N-2;i++)avf[i].active=0;for(int i=0;i<N-4;i++)avb[i].active=0;for(int i=0;i<N-6;i++)avh[i].active=0;for(int i=0;i<N-6;i++)avm[i].active=0;init=1;ps("[CKTO] Cockatoo initialized\n");return 0;}
int cockatoo_aviary(int lc,int bl,int bw,int bc,int fs,int ci,int ay){return add(avl,&st.n_aviary,&st.t_ln,N,lc,bl,bw,bc,fs,ci,ay);}
int cockatoo_feeding(int lc,int bl,int bw,int bc,int fs,int ci,int ay){return add(avf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,bc,fs,ci,ay);}
int cockatoo_breeding(int lc,int bl,int bw,int bc,int fs,int ci,int ay){return add(avb,&st.n_breed,&st.t_beak,N-4,lc,bl,bw,bc,fs,ci,ay);}
int cockatoo_health(int lc,int bl,int bw,int bc,int fs,int ci,int ay){return add(avh,&st.n_health,&st.t_fl,N-6,lc,bl,bw,bc,fs,ci,ay);}
int cockatoo_market(int lc,int bl,int bw,int bc,int fs,int ci,int ay){return add(avm,&st.n_mkt,&st.t_cr,N-6,lc,bl,bw,bc,fs,ci,ay);}
void cockatoo_report(void){ps("[CKTO] Aviary: ");pi(st.n_aviary);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Beak=");pi(st.t_beak);ps("\nHealth: ");pi(st.n_health);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Cr=");pi(st.t_cr);ps("\n");}
void cockatoo_state(void){ps("[CKTO] Aviary=");pi(st.n_aviary);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cockatoo Admin Demo ===\n\n");cockatoo_init();
ps("Cockatoo aviary...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=35+(i*3),bw=300+(i*40),bc=4+(i*1),fs=15+(i*3),ci=(i%8)+1,ay=(i%20)+1;cockatoo_aviary(lc,bl,bw,bc,fs,ci,ay);}
ps("\nCockatoo feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=37+(i*2),bw=320+(i*30),bc=4+(i%3),fs=17+(i*2),ci=(i%6)+1,ay=(i%18)+1;cockatoo_feeding(lc,bl,bw,bc,fs,ci,ay);}
ps("\nCockatoo breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=39+(i*2),bw=340+(i*25),bc=5+(i%2),fs=13+(i*4),ci=(i%5)+1,ay=(i%15)+1;cockatoo_breeding(lc,bl,bw,bc,fs,ci,ay);}
ps("\nCockatoo health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=34+(i*4),bw=290+(i*50),bc=3+(i%4),fs=10+(i*5),ci=(i%10)+1,ay=(i%12)+1;cockatoo_health(lc,bl,bw,bc,fs,ci,ay);}
ps("\nCockatoo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=40+(i*2),bw=360+(i*20),bc=5+(i%2),fs=20+(i*3),ci=(i%4)+1,ay=(i%10)+1;cockatoo_market(lc,bl,bw,bc,fs,ci,ay);}
ps("\n");cockatoo_report();cockatoo_state();ps("\n=== Demo Complete ===\n");return 0;}
