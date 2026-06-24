/* asarina_admin: Asarina (Creeping Snapdragon) climbing vine management (v1.0)
 * Asarina planning, planting, evaluation, vine training, market
 * Features: vine length, flower count, petal color, leaf shape, climbing height, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,vine_len,flower_ct,petal_color,leaf_shp,climb_ht,bloom_wk,active;} asn_t;
typedef struct{int n_plan,n_exec,n_eval,n_train,n_mkt,t_vine,t_flower,t_petal,t_leaf,t_climb;} asn_state_t;
static asn_t asnps[N],asnes[N-2],asnvs[N-4],asntr[N-6],asnms[N-6]; static asn_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(asn_t*a,int*cnt,int*sum,int mx,int lc,int vl,int fc,int pc,int ls,int ch,int bw){if(*cnt>=mx)return -1;asn_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->vine_len=vl;x->flower_ct=fc;x->petal_color=pc;x->leaf_shp=ls;x->climb_ht=ch;x->bloom_wk=bw;x->active=1;*sum+=vl;(*cnt)++;ps("[ASN] Asarina ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" vl=");pi(vl);ps(" fc=");pi(fc);ps(" pc=");pi(pc);ps(" ls=");pi(ls);ps(" ch=");pi(ch);ps("\n");return *cnt-1;}
int asn_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_train=0;st.n_mkt=0;st.t_vine=0;st.t_flower=0;st.t_petal=0;st.t_leaf=0;st.t_climb=0;for(int i=0;i<N;i++)asnps[i].active=0;for(int i=0;i<N-2;i++)asnes[i].active=0;for(int i=0;i<N-4;i++)asnvs[i].active=0;for(int i=0;i<N-6;i++)asntr[i].active=0;for(int i=0;i<N-6;i++)asnms[i].active=0;init=1;ps("[ASN] Asarina initialized\n");return 0;}
/* 1=trellis 2=hanging 3=container 4=fence 5=arbor */
int asn_planning(int lc,int vl,int fc,int pc,int ls,int ch,int bw){return add(asnps,&st.n_plan,&st.t_vine,N,lc,vl,fc,pc,ls,ch,bw);}
int asn_execution(int lc,int vl,int fc,int pc,int ls,int ch,int bw){return add(asnes,&st.n_exec,&st.t_flower,N-2,lc,vl,fc,pc,ls,ch,bw);}
int asn_evaluation(int lc,int vl,int fc,int pc,int ls,int ch,int bw){return add(asnvs,&st.n_eval,&st.t_petal,N-4,lc,vl,fc,pc,ls,ch,bw);}
int asn_vine_training(int lc,int vl,int fc,int pc,int ls,int ch,int bw){return add(asntr,&st.n_train,&st.t_leaf,N-6,lc,vl,fc,pc,ls,ch,bw);}
int asn_market(int lc,int vl,int fc,int pc,int ls,int ch,int bw){return add(asnms,&st.n_mkt,&st.t_climb,N-6,lc,vl,fc,pc,ls,ch,bw);}
void asn_report(void){ps("[ASN] Plan: ");pi(st.n_plan);ps(" vine=");pi(st.t_vine);ps("\nExec: ");pi(st.n_exec);ps(" flower=");pi(st.t_flower);ps("\nEval: ");pi(st.n_eval);ps(" petal=");pi(st.t_petal);ps("\nTrain: ");pi(st.n_train);ps(" leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" climb=");pi(st.t_climb);ps("\n");}
void asn_state(void){ps("[ASN] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Train=");pi(st.n_train);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Asarina (Creeping Snapdragon) Admin Demo ===\n\n");asn_init();
ps("Asarina planning (vine layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;asn_planning(lc,30+(i*8),4+(i%5),(i%6)+1,3+(i%3),40+(i*10),20+(i%6));}
ps("\nAsarina execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;asn_execution(lc,35+(i*6),5+(i%4),(i%6)+1,4+(i%3),45+(i*8),22+(i%5));}
ps("\nAsarina evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;asn_evaluation(lc,40+(i*5),6+(i%3),(i%5)+2,5+(i%2),50+(i*6),24+(i%4));}
ps("\nAsarina vine training...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;asn_vine_training(lc,25+(i*6),3+(i%4),(i%4)+1,3+(i%3),35+(i*8),18+(i%5));}
ps("\nAsarina climbing vine market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;asn_market(lc,45+(i*4),7+(i%3),(i%5)+2,6+(i%2),55+(i*5),26+(i%3));}
ps("\n");asn_report();asn_state();ps("\n=== Demo Complete ===\n");return 0;}
