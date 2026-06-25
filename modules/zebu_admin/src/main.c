/* zebu_admin: Zebu (Bos indicus) humped cattle (v1.0)
 * Zebu grassland, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_kg, horn_cm, walk_speed, hump_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,horn_cm,wk_sp,hm_idx,age_yr,active;} zebu_t;
typedef struct{int n_grass,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_horn,t_wk,t_hm;} zebu_state_t;
static zebu_t zpl[N],zfd[N-2],zbr[N-4],zhl[N-6],zmk[N-6]; static zebu_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zebu_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int hc,int ws,int hi,int ay){if(*cnt>=mx)return -1;zebu_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->horn_cm=hc;x->wk_sp=ws;x->hm_idx=hi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ZEBU] Zebu ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" hc=");pi(hc);ps(" ws=");pi(ws);ps(" hi=");pi(hi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int zebu_init(void){if(init)return -1;st.n_grass=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_horn=0;st.t_wk=0;st.t_hm=0;for(int i=0;i<N;i++)zpl[i].active=0;for(int i=0;i<N-2;i++)zfd[i].active=0;for(int i=0;i<N-4;i++)zbr[i].active=0;for(int i=0;i<N-6;i++)zhl[i].active=0;for(int i=0;i<N-6;i++)zmk[i].active=0;init=1;ps("[ZEBU] Zebu initialized\n");return 0;}
int zebu_grassland(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(zpl,&st.n_grass,&st.t_ln,N,lc,bl,bw,hc,ws,hi,ay);}
int zebu_feeding(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(zfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,hc,ws,hi,ay);}
int zebu_breeding(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(zbr,&st.n_breed,&st.t_horn,N-4,lc,bl,bw,hc,ws,hi,ay);}
int zebu_health(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(zhl,&st.n_health,&st.t_wk,N-6,lc,bl,bw,hc,ws,hi,ay);}
int zebu_market(int lc,int bl,int bw,int hc,int ws,int hi,int ay){return add(zmk,&st.n_mkt,&st.t_hm,N-6,lc,bl,bw,hc,ws,hi,ay);}
void zebu_report(void){ps("[ZEBU] Grass: ");pi(st.n_grass);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Horn=");pi(st.t_horn);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hm=");pi(st.t_hm);ps("\n");}
void zebu_state(void){ps("[ZEBU] Grass=");pi(st.n_grass);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Zebu Admin Demo ===\n\n");zebu_init();
ps("Zebu grassland...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=180+(i*10),bw=400+(i*40),hc=50+(i*6),ws=4+(i*2),hm=(i%6)+1,ay=(i%5)+1;zebu_grassland(lc,bl,bw,hc,ws,hm,ay);}
ps("\nZebu feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=190+(i*8),bw=430+(i*35),hc=55+(i*5),ws=5+(i),hm=(i%5)+1,ay=(i%4)+1;zebu_feeding(lc,bl,bw,hc,ws,hm,ay);}
ps("\nZebu breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=170+(i*12),bw=370+(i*45),hc=45+(i*7),ws=3+(i*3),hm=(i%4)+1,ay=(i%3)+1;zebu_breeding(lc,bl,bw,hc,ws,hm,ay);}
ps("\nZebu health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=200+(i*6),bw=460+(i*25),hc=58+(i*4),ws=6+(i),hm=(i%7)+1,ay=(i%6)+2;zebu_health(lc,bl,bw,hc,ws,hm,ay);}
ps("\nZebu market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=220+(i*5),bw=500+(i*20),hc=62+(i*3),ws=3+(i*2),hm=(i%3)+3,ay=(i%4)+3;zebu_market(lc,bl,bw,hc,ws,hm,ay);}
ps("\n");zebu_report();zebu_state();ps("\n=== Demo Complete ===\n");return 0;}
