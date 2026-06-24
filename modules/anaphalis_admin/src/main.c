/* anaphalis_admin: Anaphalis (Pearl Everlasting) dried flower plant management (v1.0)
 * Anaphalis planning, planting, evaluation, drying, market
 * Features: flower head diameter, petal layers, stem height, bloom duration, leaf texture, dry quality
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,head_dia,petal_layers,stem_ht,bloom_dk,leaf_tx,dry_qual,active;} anp_t;
typedef struct{int n_plan,n_exec,n_eval,n_dry,n_mkt,t_head,t_petal,t_stem,t_bloom,t_dry_q;} anp_state_t;
static anp_t anpps[N],anpes[N-2],anpvs[N-4],anpdv[N-6],anpms[N-6]; static anp_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(anp_t*a,int*cnt,int*sum,int mx,int lc,int hd,int pl,int sh,int bd,int lt,int dq){if(*cnt>=mx)return -1;anp_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->head_dia=hd;x->petal_layers=pl;x->stem_ht=sh;x->bloom_dk=bd;x->leaf_tx=lt;x->dry_qual=dq;x->active=1;*sum+=hd;(*cnt)++;ps("[ANP] Anaphalis ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" hd=");pi(hd);ps(" pl=");pi(pl);ps(" sh=");pi(sh);ps(" bd=");pi(bd);ps(" lt=");pi(lt);ps(" dq=");pi(dq);ps("\n");return *cnt-1;}
int anp_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_dry=0;st.n_mkt=0;st.t_head=0;st.t_petal=0;st.t_stem=0;st.t_bloom=0;st.t_dry_q=0;for(int i=0;i<N;i++)anpps[i].active=0;for(int i=0;i<N-2;i++)anpes[i].active=0;for(int i=0;i<N-4;i++)anpvs[i].active=0;for(int i=0;i<N-6;i++)anpdv[i].active=0;for(int i=0;i<N-6;i++)anpms[i].active=0;init=1;ps("[ANP] Anaphalis initialized\n");return 0;}
/* 1=dried_flower 2=garden 3=border 4=container 5=xeriscape */
int anp_planning(int lc,int hd,int pl,int sh,int bd,int lt,int dq){return add(anpps,&st.n_plan,&st.t_head,N,lc,hd,pl,sh,bd,lt,dq);}
int anp_execution(int lc,int hd,int pl,int sh,int bd,int lt,int dq){return add(anpes,&st.n_exec,&st.t_petal,N-2,lc,hd,pl,sh,bd,lt,dq);}
int anp_evaluation(int lc,int hd,int pl,int sh,int bd,int lt,int dq){return add(anpvs,&st.n_eval,&st.t_stem,N-4,lc,hd,pl,sh,bd,lt,dq);}
int anp_drying(int lc,int hd,int pl,int sh,int bd,int lt,int dq){return add(anpdv,&st.n_dry,&st.t_bloom,N-6,lc,hd,pl,sh,bd,lt,dq);}
int anp_market(int lc,int hd,int pl,int sh,int bd,int lt,int dq){return add(anpms,&st.n_mkt,&st.t_dry_q,N-6,lc,hd,pl,sh,bd,lt,dq);}
void anp_report(void){ps("[ANP] Plan: ");pi(st.n_plan);ps(" head=");pi(st.t_head);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDry: ");pi(st.n_dry);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" dry_q=");pi(st.t_dry_q);ps("\n");}
void anp_state(void){ps("[ANP] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Dry=");pi(st.n_dry);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anaphalis (Pearl Everlasting) Admin Demo ===\n\n");anp_init();
ps("Anaphalis planning (dried flower layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;anp_planning(lc,10+(i*3),3+(i%4),30+(i*6),20+(i*5),(i%5)+1,7+(i%3));}
ps("\nAnaphalis execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;anp_execution(lc,12+(i*2),4+(i%3),35+(i*5),22+(i*4),(i%5)+1,8+(i%2));}
ps("\nAnaphalis evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;anp_evaluation(lc,14+(i*2),5+(i%3),40+(i*4),25+(i*3),(i%4)+2,9+(i%2));}
ps("\nAnaphalis drying process...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;anp_drying(lc,8+(i*3),3+(i%3),25+(i*5),18+(i*4),(i%4)+1,6+(i%3));}
ps("\nAnaphalis dried flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;anp_market(lc,16+(i*2),6+(i%2),45+(i*3),28+(i*3),(i%5)+2,10+(i%2));}
ps("\n");anp_report();anp_state();ps("\n=== Demo Complete ===\n");return 0;}
