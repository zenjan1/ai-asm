/* mule_admin: Mule (Equus mulus) domestic mule pack animal (v1.0)
 * Mule stable, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, ear_cm, walk_speed, hr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ear_cm,wk_sp,hr_idx,age_yr,active;} mule_t;
typedef struct{int n_stable,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ear,t_wk,t_hr;} mule_state_t;
static mule_t arr_s[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static mule_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mule_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ec,int ws,int hri,int ay){if(*cnt>=mx)return -1;mule_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ear_cm=ec;x->wk_sp=ws;x->hr_idx=hri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MULE] Mule ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ec=");pi(ec);ps(" ws=");pi(ws);ps(" hri=");pi(hri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mule_init(void){if(init)return -1;st.n_stable=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ear=0;st.t_wk=0;st.t_hr=0;for(int i=0;i<N;i++)arr_s[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[MULE] Mule initialized\n");return 0;}
int mule_stable(int lc,int bl,int bw,int ec,int ws,int hri,int ay){return add(arr_s,&st.n_stable,&st.t_ln,N,lc,bl,bw,ec,ws,hri,ay);}
int mule_feeding(int lc,int bl,int bw,int ec,int ws,int hri,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ec,ws,hri,ay);}
int mule_breeding(int lc,int bl,int bw,int ec,int ws,int hri,int ay){return add(arr_b,&st.n_breed,&st.t_ear,N-4,lc,bl,bw,ec,ws,hri,ay);}
int mule_health(int lc,int bl,int bw,int ec,int ws,int hri,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,ec,ws,hri,ay);}
int mule_market(int lc,int bl,int bw,int ec,int ws,int hri,int ay){return add(arr_m,&st.n_mkt,&st.t_hr,N-6,lc,bl,bw,ec,ws,hri,ay);}
void mule_report(void){ps("[MULE] Stable: ");pi(st.n_stable);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ear=");pi(st.t_ear);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hr=");pi(st.t_hr);ps("\n");}
void mule_state(void){ps("[MULE] Stable=");pi(st.n_stable);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mule Admin Demo ===\n\n");mule_init();
ps("Mule stable...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=160+(i*8),bw=300+(i*25),ec=12+(i*2),ws=4+(i*1),hri=(i%6)+1,ay=(i%15)+1;mule_stable(lc,bl,bw,ec,ws,hri,ay);}
ps("\nMule feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=165+(i*6),bw=310+(i*20),ec=13+(i*1),ws=5+(i*1),hri=(i%5)+1,ay=(i%12)+1;mule_feeding(lc,bl,bw,ec,ws,hri,ay);}
ps("\nMule breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=170+(i*5),bw=320+(i*18),ec=14+(i*1),ws=3+(i*2),hri=(i%4)+1,ay=(i%10)+1;mule_breeding(lc,bl,bw,ec,ws,hri,ay);}
ps("\nMule health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=155+(i*10),bw=290+(i*30),ec=11+(i*2),ws=3+(i*3),hri=(i%7)+1,ay=(i%8)+1;mule_health(lc,bl,bw,ec,ws,hri,ay);}
ps("\nMule market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=175+(i*4),bw=330+(i*15),ec=15+(i*1),ws=6+(i*1),hri=(i%3)+1,ay=(i%6)+1;mule_market(lc,bl,bw,ec,ws,hri,ay);}
ps("\n");mule_report();mule_state();ps("\n=== Demo Complete ===\n");return 0;}
