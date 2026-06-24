/* artemisia_admin: Artemisia (Wormwood) aromatic herb management (v1.0)
 * Artemisia planning, planting, evaluation, leaf harvest, market
 * Features: stem height, leaf silver intensity, compound content, bloom cluster, root depth, drought tolerance
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,stem_ht,leaf_sv,compound_ct,bloom_cl,root_dep,drought_tl,active;} art_t;
typedef struct{int n_plan,n_exec,n_eval,n_leaf,n_mkt,t_stem,t_leaf_v,t_compound,t_bloom,t_root;} art_state_t;
static art_t artps[N],artes[N-2],artvs[N-4],artlv[N-6],artms[N-6]; static art_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(art_t*a,int*cnt,int*sum,int mx,int lc,int sh,int ls,int cc,int bc,int rd,int dt){if(*cnt>=mx)return -1;art_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->stem_ht=sh;x->leaf_sv=ls;x->compound_ct=cc;x->bloom_cl=bc;x->root_dep=rd;x->drought_tl=dt;x->active=1;*sum+=sh;(*cnt)++;ps("[ART] Artemisia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" ls=");pi(ls);ps(" cc=");pi(cc);ps(" bc=");pi(bc);ps(" rd=");pi(rd);ps("\n");return *cnt-1;}
int art_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_leaf=0;st.n_mkt=0;st.t_stem=0;st.t_leaf_v=0;st.t_compound=0;st.t_bloom=0;st.t_root=0;for(int i=0;i<N;i++)artps[i].active=0;for(int i=0;i<N-2;i++)artes[i].active=0;for(int i=0;i<N-4;i++)artvs[i].active=0;for(int i=0;i<N-6;i++)artlv[i].active=0;for(int i=0;i<N-6;i++)artms[i].active=0;init=1;ps("[ART] Artemisia initialized\n");return 0;}
/* 1=herb_garden 2=medicinal 3=xeriscape 4=border 5=container */
int art_planning(int lc,int sh,int ls,int cc,int bc,int rd,int dt){return add(artps,&st.n_plan,&st.t_stem,N,lc,sh,ls,cc,bc,rd,dt);}
int art_execution(int lc,int sh,int ls,int cc,int bc,int rd,int dt){return add(artes,&st.n_exec,&st.t_leaf_v,N-2,lc,sh,ls,cc,bc,rd,dt);}
int art_evaluation(int lc,int sh,int ls,int cc,int bc,int rd,int dt){return add(artvs,&st.n_eval,&st.t_compound,N-4,lc,sh,ls,cc,bc,rd,dt);}
int art_leaf_harvest(int lc,int sh,int ls,int cc,int bc,int rd,int dt){return add(artlv,&st.n_leaf,&st.t_bloom,N-6,lc,sh,ls,cc,bc,rd,dt);}
int art_market(int lc,int sh,int ls,int cc,int bc,int rd,int dt){return add(artms,&st.n_mkt,&st.t_root,N-6,lc,sh,ls,cc,bc,rd,dt);}
void art_report(void){ps("[ART] Plan: ");pi(st.n_plan);ps(" stem=");pi(st.t_stem);ps("\nExec: ");pi(st.n_exec);ps(" leaf=");pi(st.t_leaf_v);ps("\nEval: ");pi(st.n_eval);ps(" compound=");pi(st.t_compound);ps("\nLeaf: ");pi(st.n_leaf);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" root=");pi(st.t_root);ps("\n");}
void art_state(void){ps("[ART] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Leaf=");pi(st.n_leaf);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Artemisia (Wormwood) Admin Demo ===\n\n");art_init();
ps("Artemisia planning (herb layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;art_planning(lc,40+(i*8),7+(i%4),15+(i*4),5+(i%3),20+(i*5),(i%5)+1);}
ps("\nArtemisia execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;art_execution(lc,45+(i*6),8+(i%3),18+(i*3),6+(i%3),25+(i*4),(i%5)+1);}
ps("\nArtemisia evaluation (compound check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;art_evaluation(lc,50+(i*5),9+(i%3),20+(i*3),7+(i%2),30+(i*3),(i%4)+2);}
ps("\nArtemisia leaf harvest...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;art_leaf_harvest(lc,35+(i*6),6+(i%4),12+(i*4),4+(i%3),18+(i*4),(i%5)+1);}
ps("\nArtemisia medicinal market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;art_market(lc,55+(i*4),10+(i%2),22+(i*3),8+(i%2),35+(i*3),(i%4)+2);}
ps("\n");art_report();art_state();ps("\n=== Demo Complete ===\n");return 0;}
