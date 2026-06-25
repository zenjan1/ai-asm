/* dugong_admin: Dugong (Dugong dugon) marine herbivorous mammal (v1.0)
 * Dugong coast, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, flipper_cm, swim_speed, skin_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,flip_cm,sw_sp,sk_idx,age_yr,active;} dug_t;
typedef struct{int n_coast,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_flip,t_sw,t_sk;} dug_state_t;
static dug_t csl[N],csf[N-2],csb[N-4],csh[N-6],csm[N-6]; static dug_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(dug_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int fc,int ss,int si,int ay){if(*cnt>=mx)return -1;dug_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->flip_cm=fc;x->sw_sp=ss;x->sk_idx=si;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[DUG] Dugong ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" fc=");pi(fc);ps(" ss=");pi(ss);ps(" si=");pi(si);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int dugong_init(void){if(init)return -1;st.n_coast=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_flip=0;st.t_sw=0;st.t_sk=0;for(int i=0;i<N;i++)csl[i].active=0;for(int i=0;i<N-2;i++)csf[i].active=0;for(int i=0;i<N-4;i++)csb[i].active=0;for(int i=0;i<N-6;i++)csh[i].active=0;for(int i=0;i<N-6;i++)csm[i].active=0;init=1;ps("[DUG] Dugong initialized\n");return 0;}
int dugong_coast(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(csl,&st.n_coast,&st.t_ln,N,lc,bl,bw,fc,ss,si,ay);}
int dugong_feeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(csf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,fc,ss,si,ay);}
int dugong_breeding(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(csb,&st.n_breed,&st.t_flip,N-4,lc,bl,bw,fc,ss,si,ay);}
int dugong_health(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(csh,&st.n_health,&st.t_sw,N-6,lc,bl,bw,fc,ss,si,ay);}
int dugong_market(int lc,int bl,int bw,int fc,int ss,int si,int ay){return add(csm,&st.n_mkt,&st.t_sk,N-6,lc,bl,bw,fc,ss,si,ay);}
void dugong_report(void){ps("[DUG] Coast: ");pi(st.n_coast);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Flip=");pi(st.t_flip);ps("\nHealth: ");pi(st.n_health);ps(" Sw=");pi(st.t_sw);ps("\nMkt: ");pi(st.n_mkt);ps(" Sk=");pi(st.t_sk);ps("\n");}
void dugong_state(void){ps("[DUG] Coast=");pi(st.n_coast);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Dugong Admin Demo ===\n\n");dugong_init();
ps("Dugong coast...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*12),bw=250+(i*40),fc=50+(i*5),ss=4+(i*2),si=(i%8)+1,ay=(i%25)+1;dugong_coast(lc,bl,bw,fc,ss,si,ay);}
ps("\nDugong feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=208+(i*10),bw=270+(i*30),fc=53+(i*4),ss=5+(i*1),si=(i%6)+1,ay=(i%20)+1;dugong_feeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nDugong breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=215+(i*8),bw=290+(i*25),fc=56+(i*3),ss=3+(i*3),si=(i%5)+1,ay=(i%15)+1;dugong_breeding(lc,bl,bw,fc,ss,si,ay);}
ps("\nDugong health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=195+(i*15),bw=240+(i*50),fc=48+(i*6),ss=2+(i*4),si=(i%10)+1,ay=(i%12)+1;dugong_health(lc,bl,bw,fc,ss,si,ay);}
ps("\nDugong market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=220+(i*6),bw=310+(i*20),fc=58+(i*3),ss=6+(i*2),si=(i%4)+1,ay=(i%10)+1;dugong_market(lc,bl,bw,fc,ss,si,ay);}
ps("\n");dugong_report();dugong_state();ps("\n=== Demo Complete ===\n");return 0;}
