/* allium_admin: Allium (Ornamental Onion) globe-shaped flower and bulb plant management (v1.0)
 * Allium planning, planting, evaluation, bulb division, market
 * Features: flower globe diameter, stem height, bloom color, bulb count, pollinator attraction
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,globe_dia,stem_ht,bloom_color,bulb_ct,pollinator_at,drought_tol,active;} ali_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_globe,t_stem,t_color,t_bulb,t_pollinator;} ali_state_t;
static ali_t alips[N],alies[N-2],alivs[N-4],alidv[N-6],alims[N-6]; static ali_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ali_t*a,int*cnt,int*sum,int mx,int lc,int gd,int sh,int bc,int bct,int pa,int dt){if(*cnt>=mx)return -1;ali_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->globe_dia=gd;x->stem_ht=sh;x->bloom_color=bc;x->bulb_ct=bct;x->pollinator_at=pa;x->drought_tol=dt;x->active=1;*sum+=gd;(*cnt)++;ps("[ALI] Allium ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" gd=");pi(gd);ps(" sh=");pi(sh);ps(" bc=");pi(bc);ps(" bct=");pi(bct);ps(" pa=");pi(pa);ps("\n");return *cnt-1;}
int ali_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_globe=0;st.t_stem=0;st.t_color=0;st.t_bulb=0;st.t_pollinator=0;for(int i=0;i<N;i++)alips[i].active=0;for(int i=0;i<N-2;i++)alies[i].active=0;for(int i=0;i<N-4;i++)alivs[i].active=0;for(int i=0;i<N-6;i++)alidv[i].active=0;for(int i=0;i<N-6;i++)alims[i].active=0;init=1;ps("[ALI] Allium (ornamental onion) initialized\n");return 0;}
/* 1=border 2=rock_garden 3=prairie 4=cutting_garden 5=container */
int ali_planning(int lc,int gd,int sh,int bc,int bct,int pa,int dt){return add(alips,&st.n_plan,&st.t_globe,N,lc,gd,sh,bc,bct,pa,dt);}
int ali_execution(int lc,int gd,int sh,int bc,int bct,int pa,int dt){return add(alies,&st.n_exec,&st.t_stem,N-2,lc,gd,sh,bc,bct,pa,dt);}
int ali_evaluation(int lc,int gd,int sh,int bc,int bct,int pa,int dt){return add(alivs,&st.n_eval,&st.t_color,N-4,lc,gd,sh,bc,bct,pa,dt);}
int ali_division(int lc,int gd,int sh,int bc,int bct,int pa,int dt){return add(alidv,&st.n_div,&st.t_bulb,N-6,lc,gd,sh,bc,bct,pa,dt);}
int ali_market(int lc,int gd,int sh,int bc,int bct,int pa,int dt){return add(alims,&st.n_mkt,&st.t_pollinator,N-6,lc,gd,sh,bc,bct,pa,dt);}
void ali_report(void){ps("[ALI] Plan: ");pi(st.n_plan);ps(" globe=");pi(st.t_globe);ps("\nExec: ");pi(st.n_exec);ps(" stem=");pi(st.t_stem);ps("\nEval: ");pi(st.n_eval);ps(" color=");pi(st.t_color);ps("\nDiv: ");pi(st.n_div);ps(" bulb=");pi(st.t_bulb);ps("\nMkt: ");pi(st.n_mkt);ps(" poll=");pi(st.t_pollinator);ps("\n");}
void ali_state(void){ps("[ALI] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Allium (Ornamental Onion) Admin Demo ===\n\n");ali_init();
ps("Allium planning (globe flower layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;ali_planning(lc,10+(i*3),60+(i*10),(i%6)+1,5+(i*2),15+(i*4),(i%5)+1);}
ps("\nAllium execution (bulb planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;ali_execution(lc,12+(i*2),65+(i*8),(i%6)+1,6+(i*2),18+(i*3),(i%5)+1);}
ps("\nAllium evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;ali_evaluation(lc,14+(i*2),70+(i*7),(i%5)+2,7+(i*2),20+(i*3),(i%4)+2);}
ps("\nAllium bulb division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;ali_division(lc,8+(i*2),55+(i*6),(i%4)+1,4+(i*2),12+(i*3),(i%5)+1);}
ps("\nAllium ornamental bulb market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;ali_market(lc,16+(i*2),75+(i*5),(i%6)+2,8+(i*2),22+(i*2),(i%4)+2);}
ps("\n");ali_report();ali_state();ps("\n=== Demo Complete ===\n");return 0;}
