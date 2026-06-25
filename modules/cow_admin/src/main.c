/* cow_admin: Cow (Bos taurus) domestic cattle bovine (v1.0)
 * Cow pasture, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, walk_speed, hide_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,wk_sp,hd_idx,age_yr,active;} cow_t;
typedef struct{int n_past,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_wk,t_hd;} cow_state_t;
static cow_t ptl[N],ptf[N-2],ptb[N-4],pth[N-6],ptm[N-6]; static cow_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cow_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int hi,int ay){if(*cnt>=mx)return -1;cow_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->wk_sp=ws;x->hd_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[COW] Cow ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int cow_init(void){if(init)return -1;st.n_past=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_wk=0;st.t_hd=0;for(int i=0;i<N;i++)ptl[i].active=0;for(int i=0;i<N-2;i++)ptf[i].active=0;for(int i=0;i<N-4;i++)ptb[i].active=0;for(int i=0;i<N-6;i++)pth[i].active=0;for(int i=0;i<N-6;i++)ptm[i].active=0;init=1;ps("[COW] Cow initialized\n");return 0;}
int cow_pasture(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(ptl,&st.n_past,&st.t_ln,N,lc,bl,bw,hc,ws,hi,ay);}
int cow_feeding(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(ptf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,hi,ay);}
int cow_breeding(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(ptb,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,ws,hi,ay);}
int cow_health(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(pth,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,hi,ay);}
int cow_market(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(ptm,&st.n_mkt,&st.t_hd,N-6,lc,bl,bw,hc,ws,hi,ay);}
void cow_report(void){ps("[COW] Past: ");pi(st.n_past);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hd=");pi(st.t_hd);ps("\n");}
void cow_state(void){ps("[COW] Past=");pi(st.n_past);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cow Admin Demo ===\n\n");cow_init();
ps("Cow pasture...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*8),bw=300+(i*40),hc=20+(i*5),ws=3+(i*2),hi=(i%8)+1,ay=(i%12)+1;cow_pasture(lc,bl,bw,hc,ws,hi,ay);}
ps("\nCow feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=185+(i*6),bw=320+(i*30),hc=22+(i*4),ws=4+(i*1),hi=(i%6)+1,ay=(i%10)+1;cow_feeding(lc,bl,bw,hc,ws,hi,ay);}
ps("\nCow breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=190+(i*5),bw=340+(i*25),hc=24+(i*3),ws=2+(i*3),hi=(i%5)+1,ay=(i%8)+1;cow_breeding(lc,bl,bw,hc,ws,hi,ay);}
ps("\nCow health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=175+(i*10),bw=280+(i*50),hc=18+(i*6),ws=2+(i*4),hi=(i%10)+1,ay=(i%7)+1;cow_health(lc,bl,bw,hc,ws,hi,ay);}
ps("\nCow market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=195+(i*4),bw=360+(i*20),hc=26+(i*3),ws=5+(i*2),hi=(i%4)+1,ay=(i%6)+1;cow_market(lc,bl,bw,hc,ws,hi,ay);}
ps("\n");cow_report();cow_state();ps("\n=== Demo Complete ===\n");return 0;}
