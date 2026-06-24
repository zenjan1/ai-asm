/* avicennia_admin: Avicennia (Mangrove) coastal wetland management (v1.0)
 * Avicennia planning, planting, evaluation, propagation, market
 * Features: tree height, trunk diameter, leaf width, salt tolerance, root spread, tide level
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,tree_ht,trunk_dia,leaf_wd,salt_tl,root_sp,tide_lv,active;} avic_t;
typedef struct{int n_plan,n_exec,n_eval,n_prop,n_mkt,t_ht,t_trunk,t_leaf,t_salt,t_root;} avic_state_t;
static avic_t avicps[N],avices[N-2],avicvs[N-4],avicpr[N-6],avicms[N-6]; static avic_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(avic_t*a,int*cnt,int*sum,int mx,int lc,int th,int td,int lw,int slt,int rsp,int tlv){if(*cnt>=mx)return -1;avic_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->tree_ht=th;x->trunk_dia=td;x->leaf_wd=lw;x->salt_tl=slt;x->root_sp=rsp;x->tide_lv=tlv;x->active=1;*sum+=th;(*cnt)++;ps("[AVIC] Avicennia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" th=");pi(th);ps(" td=");pi(td);ps(" lw=");pi(lw);ps(" slt=");pi(slt);ps(" rsp=");pi(rsp);ps("\n");return *cnt-1;}
int avic_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prop=0;st.n_mkt=0;st.t_ht=0;st.t_trunk=0;st.t_leaf=0;st.t_salt=0;st.t_root=0;for(int i=0;i<N;i++)avicps[i].active=0;for(int i=0;i<N-2;i++)avices[i].active=0;for(int i=0;i<N-4;i++)avicvs[i].active=0;for(int i=0;i<N-6;i++)avicpr[i].active=0;for(int i=0;i<N-6;i++)avicms[i].active=0;init=1;ps("[AVIC] Avicennia initialized\n");return 0;}
/* 1=coastal_wetland 2=estuary 3=tidal_flat 4=restoration 5=buffer_zone */
int avic_planning(int lc,int th,int td,int lw,int slt,int rsp,int tlv){return add(avicps,&st.n_plan,&st.t_ht,N,lc,th,td,lw,slt,rsp,tlv);}
int avic_execution(int lc,int th,int td,int lw,int slt,int rsp,int tlv){return add(avices,&st.n_exec,&st.t_trunk,N-2,lc,th,td,lw,slt,rsp,tlv);}
int avic_evaluation(int lc,int th,int td,int lw,int slt,int rsp,int tlv){return add(avicvs,&st.n_eval,&st.t_leaf,N-4,lc,th,td,lw,slt,rsp,tlv);}
int avic_propagation(int lc,int th,int td,int lw,int slt,int rsp,int tlv){return add(avicpr,&st.n_prop,&st.t_salt,N-6,lc,th,td,lw,slt,rsp,tlv);}
int avic_market(int lc,int th,int td,int lw,int slt,int rsp,int tlv){return add(avicms,&st.n_mkt,&st.t_root,N-6,lc,th,td,lw,slt,rsp,tlv);}
void avic_report(void){ps("[AVIC] Plan: ");pi(st.n_plan);ps(" ht=");pi(st.t_ht);ps("\nExec: ");pi(st.n_exec);ps(" trunk=");pi(st.t_trunk);ps("\nEval: ");pi(st.n_eval);ps(" leaf=");pi(st.t_leaf);ps("\nProp: ");pi(st.n_prop);ps(" salt=");pi(st.t_salt);ps("\nMkt: ");pi(st.n_mkt);ps(" root=");pi(st.t_root);ps("\n");}
void avic_state(void){ps("[AVIC] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prop=");pi(st.n_prop);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Avicennia (Mangrove) Admin Demo ===\n\n");avic_init();
ps("Avicennia planning (coastal layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;avic_planning(lc,300+(i*50),15+(i*5),8+(i*3),60+(i*5),40+(i*8),(i%4)+1);}
ps("\nAvicennia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;avic_execution(lc,350+(i*40),18+(i*4),10+(i*2),65+(i*4),45+(i*7),(i%3)+2);}
ps("\nAvicennia evaluation (growth check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;avic_evaluation(lc,400+(i*30),20+(i*3),12+(i*2),70+(i*3),50+(i*6),(i%3)+2);}
ps("\nAvicennia propagation...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;avic_propagation(lc,250+(i*45),12+(i*5),6+(i*3),55+(i*5),35+(i*8),(i%4)+1);}
ps("\nAvicennia coastal wetland market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;avic_market(lc,450+(i*30),22+(i*3),14+(i*2),75+(i*3),55+(i*5),(i%3)+3);}
ps("\n");avic_report();avic_state();ps("\n=== Demo Complete ===\n");return 0;}
