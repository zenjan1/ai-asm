/* ant_admin: Ant (Formica rufa) red wood ant social insect (v1.0)
 * Ant colony, feeding, breeding, health, market
 * Features: body_len_mm, body_wt_mg, antenna_mm, walk_speed, cl_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,ant_mm,wk_sp,cl_idx,age_yr,active;} ant_t;
typedef struct{int n_colony,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_ant,t_wk,t_cl;} ant_state_t;
static ant_t arr_c[N],arr_fd[N-2],arr_b[N-4],arr_h[N-6],arr_m[N-6]; static ant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ant_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int am,int ws,int cli,int ay){if(*cnt>=mx)return -1;ant_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->ant_mm=am;x->wk_sp=ws;x->cl_idx=cli;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[ANT] Ant ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" am=");pi(am);ps(" ws=");pi(ws);ps(" cli=");pi(cli);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int ant_init(void){if(init)return -1;st.n_colony=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_ant=0;st.t_wk=0;st.t_cl=0;for(int i=0;i<N;i++)arr_c[i].active=0;for(int i=0;i<N-2;i++)arr_fd[i].active=0;for(int i=0;i<N-4;i++)arr_b[i].active=0;for(int i=0;i<N-6;i++)arr_h[i].active=0;for(int i=0;i<N-6;i++)arr_m[i].active=0;init=1;ps("[ANT] Ant initialized\n");return 0;}
int ant_colony(int lc,int bl,int bw,int am,int ws,int cli,int ay){return add(arr_c,&st.n_colony,&st.t_ln,N,lc,bl,bw,am,ws,cli,ay);}
int ant_feeding(int lc,int bl,int bw,int am,int ws,int cli,int ay){return add(arr_fd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,am,ws,cli,ay);}
int ant_breeding(int lc,int bl,int bw,int am,int ws,int cli,int ay){return add(arr_b,&st.n_breed,&st.t_ant,N-4,lc,bl,bw,am,ws,cli,ay);}
int ant_health(int lc,int bl,int bw,int am,int ws,int cli,int ay){return add(arr_h,&st.n_health,&st.t_wk,N-6,lc,bl,bw,am,ws,cli,ay);}
int ant_market(int lc,int bl,int bw,int am,int ws,int cli,int ay){return add(arr_m,&st.n_mkt,&st.t_cl,N-6,lc,bl,bw,am,ws,cli,ay);}
void ant_report(void){ps("[ANT] Colony: ");pi(st.n_colony);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ant=");pi(st.t_ant);ps("\nHealth: ");pi(st.n_health);ps(" Wk=");pi(st.t_wk);ps("\nMkt: ");pi(st.n_mkt);ps(" Cl=");pi(st.t_cl);ps("\n");}
void ant_state(void){ps("[ANT] Colony=");pi(st.n_colony);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Ant Admin Demo ===\n\n");ant_init();
ps("Ant colony...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=5+(i*1),bw=2+(i*1),am=2+(i*1),ws=1+(i%2),cli=(i%6)+1,ay=(i%2)+1;ant_colony(lc,bl,bw,am,ws,cli,ay);}
ps("\nAnt feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=6+(i*1),bw=3+(i*1),am=3+(i*1),ws=1+(i%2),cli=(i%5)+1,ay=(i%2)+1;ant_feeding(lc,bl,bw,am,ws,cli,ay);}
ps("\nAnt breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=7+(i*1),bw=3+(i*1),am=3+(i*1),ws=1+(i%2),cli=(i%4)+1,ay=(i%1)+1;ant_breeding(lc,bl,bw,am,ws,cli,ay);}
ps("\nAnt health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i*1),bw=2+(i*1),am=2+(i*1),ws=1+(i%2),cli=(i%7)+1,ay=(i%1)+1;ant_health(lc,bl,bw,am,ws,cli,ay);}
ps("\nAnt market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=8+(i*1),bw=4+(i*1),am=4+(i*1),ws=2+(i%2),cli=(i%3)+1,ay=(i%1)+1;ant_market(lc,bl,bw,am,ws,cli,ay);}
ps("\n");ant_report();ant_state();ps("\n=== Demo Complete ===\n");return 0;}
