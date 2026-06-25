/* ox_admin: Ox (Bos taurus) domestic ox draft animal (v1.0)
 * Ox pasture, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, walk_speed, hr_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,wk_sp,hr_idx,age_yr,active;} ox_t;
typedef struct{int n_pasture,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_wk,t_hr;} ox_state_t;
static ox_t arr_p[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static ox_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ox_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int hri,int ay){if(*cnt>=mx)return -1;ox_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->wk_sp=ws;x->hr_idx=hri;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[OX] Ox ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" hri=");pi(hri);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ox_init(void){if(init)return -1;st.n_pasture=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_wk=0;st.t_hr=0;for(int i=0;i<N;i++)arr_p[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[OX] Ox initialized\n");return 0;}
int ox_pasture(int lc,int bl,int bw,int hc,int ws,int hri,int ay){return add(arr_p,&st.n_pasture,&st.t_ln,N,lc,bl,bw,hc,ws,hri,ay);}
int ox_feeding(int lc,int bl,int bw,int hc,int ws,int hri,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,hri,ay);}
int ox_breeding(int lc,int bl,int bw,int hc,int ws,int hri,int ay){return add(arr_b,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,ws,hri,ay);}
int ox_health(int lc,int bl,int bw,int hc,int ws,int hri,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,hri,ay);}
int ox_market(int lc,int bl,int bw,int hc,int ws,int hri,int ay){return add(arr_m,&st.n_mkt,&st.t_hr,N-6,lc,bl,bw,hc,ws,hri,ay);}
void ox_report(void){ps("[OX] Pasture: ");pi(st.n_pasture);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hr=");pi(st.t_hr);ps("\n");}
void ox_state(void){ps("[OX] Pasture=");pi(st.n_pasture);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ox Admin Demo ===\n\n");ox_init();
ps("Ox pasture...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=200+(i*10),bw=400+(i*30),hc=25+(i*3),ws=3+(i*1),hri=(i%6)+1,ay=(i%12)+1;ox_pasture(lc,bl,bw,hc,ws,hri,ay);}
ps("\nOx feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=210+(i*8),bw=420+(i*25),hc=27+(i*2),ws=4+(i*1),hri=(i%5)+1,ay=(i%10)+1;ox_feeding(lc,bl,bw,hc,ws,hri,ay);}
ps("\nOx breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=220+(i*6),bw=440+(i*20),hc=29+(i*2),ws=2+(i*2),hri=(i%4)+1,ay=(i%8)+1;ox_breeding(lc,bl,bw,hc,ws,hri,ay);}
ps("\nOx health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=190+(i*12),bw=380+(i*35),hc=23+(i*3),ws=2+(i*3),hri=(i%7)+1,ay=(i%6)+1;ox_health(lc,bl,bw,hc,ws,hri,ay);}
ps("\nOx market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=230+(i*5),bw=460+(i*15),hc=31+(i*2),ws=5+(i*1),hri=(i%3)+1,ay=(i%5)+1;ox_market(lc,bl,bw,hc,ws,hri,ay);}
ps("\n");ox_report();ox_state();ps("\n=== Demo Complete ===\n");return 0;}
