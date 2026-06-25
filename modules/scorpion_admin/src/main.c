/* scorpion_admin: Scorpion (Heterometrus spinifer) Asian forest scorpion arachnid (v1.0)
 * Scorpion desert, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, walk_speed, st_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,wk_sp,st_idx,age_yr,active;} scor_t;
typedef struct{int n_desert,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_wk,t_st;} scor_state_t;
static scor_t arr_d[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static scor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(scor_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int ws,int sti,int ay){if(*cnt>=mx)return -1;scor_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->wk_sp=ws;x->st_idx=sti;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[SCOR] Scorpion ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ws=");pi(ws);ps(" sti=");pi(sti);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int scorpion_init(void){if(init)return -1;st.n_desert=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_wk=0;st.t_st=0;for(int i=0;i<N;i++)arr_d[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[SCOR] Scorpion initialized\n");return 0;}
int scorpion_desert(int lc,int bl,int bw,int tc,int ws,int sti,int ay){return add(arr_d,&st.n_desert,&st.t_ln,N,lc,bl,bw,tc,ws,sti,ay);}
int scorpion_feeding(int lc,int bl,int bw,int tc,int ws,int sti,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,ws,sti,ay);}
int scorpion_breeding(int lc,int bl,int bw,int tc,int ws,int sti,int ay){return add(arr_b,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,ws,sti,ay);}
int scorpion_health(int lc,int bl,int bw,int tc,int ws,int sti,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,tc,ws,sti,ay);}
int scorpion_market(int lc,int bl,int bw,int tc,int ws,int sti,int ay){return add(arr_m,&st.n_mkt,&st.t_st,N-6,lc,bl,bw,tc,ws,sti,ay);}
void scorpion_report(void){ps("[SCOR] Desert: ");pi(st.n_desert);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void scorpion_state(void){ps("[SCOR] Desert=");pi(st.n_desert);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Scorpion Admin Demo ===\n\n");scorpion_init();
ps("Scorpion desert...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=10+(i*1),bw=15+(i*2),tc=8+(i*1),ws=1+(i*1),sti=(i%6)+1,ay=(i%6)+1;scorpion_desert(lc,bl,bw,tc,ws,sti,ay);}
ps("\nScorpion feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=11+(i*1),bw=17+(i*2),tc=9+(i*1),ws=2+(i*1),sti=(i%5)+1,ay=(i%5)+1;scorpion_feeding(lc,bl,bw,tc,ws,sti,ay);}
ps("\nScorpion breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=12+(i*1),bw=19+(i*1),tc=10+(i*1),ws=1+(i*1),sti=(i%4)+1,ay=(i%4)+1;scorpion_breeding(lc,bl,bw,tc,ws,sti,ay);}
ps("\nScorpion health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=9+(i*1),bw=13+(i*3),tc=7+(i*1),ws=1+(i*2),sti=(i%7)+1,ay=(i%3)+1;scorpion_health(lc,bl,bw,tc,ws,sti,ay);}
ps("\nScorpion market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=13+(i*1),bw=21+(i*1),tc=11+(i*1),ws=3+(i*1),sti=(i%3)+1,ay=(i%3)+1;scorpion_market(lc,bl,bw,tc,ws,sti,ay);}
ps("\n");scorpion_report();scorpion_state();ps("\n=== Demo Complete ===\n");return 0;}
