/* spider_admin: Spider (Araneus diadematus) European garden spider arachnid (v1.0)
 * Spider garden, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, leg_span_cm, walk_speed, wb_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,lg_sp,wk_sp,wb_idx,age_yr,active;} spdr_t;
typedef struct{int n_garden,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_lg,t_wk,t_wb;} spdr_state_t;
static spdr_t arr_g[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static spdr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(spdr_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int ls,int ws,int wbi,int ay){if(*cnt>=mx)return -1;spdr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->lg_sp=ls;x->wk_sp=ws;x->wb_idx=wbi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SPDR] Spider ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" ls=");pi(ls);ps(" ws=");pi(ws);ps(" wbi=");pi(wbi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int spider_init(void){if(init)return -1;st.n_garden=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_lg=0;st.t_wk=0;st.t_wb=0;for(int i=0;i<N;i++)arr_g[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[SPDR] Spider initialized\n");return 0;}
int spider_garden(int lc,int bl,int bw,int ls,int ws,int wbi,int ay){return add(arr_g,&st.n_garden,&st.t_ln,N,lc,bl,bw,ls,ws,wbi,ay);}
int spider_feeding(int lc,int bl,int bw,int ls,int ws,int wbi,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,ls,ws,wbi,ay);}
int spider_breeding(int lc,int bl,int bw,int ls,int ws,int wbi,int ay){return add(arr_b,&st.n_breed,&st.t_lg,N-4,lc,bl,bw,ls,ws,wbi,ay);}
int spider_health(int lc,int bl,int bw,int ls,int ws,int wbi,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,ls,ws,wbi,ay);}
int spider_market(int lc,int bl,int bw,int ls,int ws,int wbi,int ay){return add(arr_m,&st.n_mkt,&st.t_wb,N-6,lc,bl,bw,ls,ws,wbi,ay);}
void spider_report(void){ps("[SPDR] Garden: ");pi(st.n_garden);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Lg=");pi(st.t_lg);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Wb=");pi(st.t_wb);ps("\n");}
void spider_state(void){ps("[SPDR] Garden=");pi(st.n_garden);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Spider Admin Demo ===\n\n");spider_init();
ps("Spider garden...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=1+(i*1),bw=10+(i*2),ls=3+(i*1),ws=1+(i*1),wbi=(i%6)+1,ay=(i%2)+1;spider_garden(lc,bl,bw,ls,ws,wbi,ay);}
ps("\nSpider feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=2+(i*1),bw=12+(i*2),ls=4+(i*1),ws=1+(i*1),wbi=(i%5)+1,ay=(i%2)+1;spider_feeding(lc,bl,bw,ls,ws,wbi,ay);}
ps("\nSpider breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i*1),bw=14+(i*1),ls=4+(i*1),ws=1+(i*1),wbi=(i%4)+1,ay=(i%1)+1;spider_breeding(lc,bl,bw,ls,ws,wbi,ay);}
ps("\nSpider health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=1+(i*1),bw=8+(i*3),ls=2+(i*1),ws=1+(i*2),wbi=(i%7)+1,ay=(i%1)+1;spider_health(lc,bl,bw,ls,ws,wbi,ay);}
ps("\nSpider market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=3+(i*1),bw=16+(i*1),ls=5+(i*1),ws=2+(i*1),wbi=(i%3)+1,ay=(i%1)+1;spider_market(lc,bl,bw,ls,ws,wbi,ay);}
ps("\n");spider_report();spider_state();ps("\n=== Demo Complete ===\n");return 0;}
