/* tagetes_admin: Tagetes (Marigold) ornamental and companion planting (v1.0)
 * Tagetes planning, execution, evaluation, companion planting, pest management market
 * Features: variety tracking, bloom density, pest repellent, deadheading, companion index
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,bloom_ct,ht_cm,pest_sc,comp_idx,days,color,active;} tag_t;
typedef struct{int n_plan,n_exec,n_eval,n_comp,n_mkt,t_bloom,t_ht,t_pest,t_comp_i,t_days;} tag_state_t;
static tag_t tagps[N],tages[N-2],tagvs[N-4],tagcs[N-6],tagms[N-6]; static tag_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tag_t*a,int*cnt,int*sum,int mx,int cv,int bl,int ht,int pc,int ci,int dy,int cl){if(*cnt>=mx)return -1;tag_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->bloom_ct=bl;x->ht_cm=ht;x->pest_sc=pc;x->comp_idx=ci;x->days=dy;x->color=cl;x->active=1;*sum+=bl;(*cnt)++;ps("[TAG] Tagetes ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" bl=");pi(bl);ps(" ht=");pi(ht);ps(" pc=");pi(pc);ps(" ci=");pi(ci);ps("\n");return *cnt-1;}
int tag_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_comp=0;st.n_mkt=0;st.t_bloom=0;st.t_ht=0;st.t_pest=0;st.t_comp_i=0;st.t_days=0;for(int i=0;i<N;i++)tagps[i].active=0;for(int i=0;i<N-2;i++)tages[i].active=0;for(int i=0;i<N-4;i++)tagvs[i].active=0;for(int i=0;i<N-6;i++)tagcs[i].active=0;for(int i=0;i<N-6;i++)tagms[i].active=0;init=1;ps("[TAG] Tagetes (marigold) initialized\n");return 0;}
/* 1=African 2=French 3=Signet 4=Triploid 5=Wild */
int tag_planning(int cv,int bl,int ht,int pc,int ci,int dy,int cl){return add(tagps,&st.n_plan,&st.t_bloom,N,cv,bl,ht,pc,ci,dy,cl);}
int tag_execution(int cv,int bl,int ht,int pc,int ci,int dy,int cl){return add(tages,&st.n_exec,&st.t_ht,N-2,cv,bl,ht,pc,ci,dy,cl);}
int tag_evaluation(int cv,int bl,int ht,int pc,int ci,int dy,int cl){return add(tagvs,&st.n_eval,&st.t_pest,N-4,cv,bl,ht,pc,ci,dy,cl);}
int tag_companion(int cv,int bl,int ht,int pc,int ci,int dy,int cl){return add(tagcs,&st.n_comp,&st.t_comp_i,N-6,cv,bl,ht,pc,ci,dy,cl);}
int tag_market(int cv,int bl,int ht,int pc,int ci,int dy,int cl){return add(tagms,&st.n_mkt,&st.t_days,N-6,cv,bl,ht,pc,ci,dy,cl);}
void tag_report(void){ps("[TAG] Plan: ");pi(st.n_plan);ps(" bloom=");pi(st.t_bloom);ps("\nExec: ");pi(st.n_exec);ps(" ht=");pi(st.t_ht);ps("\nEval: ");pi(st.n_eval);ps(" pest=");pi(st.t_pest);ps("\nComp: ");pi(st.n_comp);ps(" idx=");pi(st.t_comp_i);ps("\nMkt: ");pi(st.n_mkt);ps(" days=");pi(st.t_days);ps("\n");}
void tag_state(void){ps("[TAG] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Comp=");pi(st.n_comp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tagetes (Marigold) Admin Demo ===\n\n");tag_init();
ps("Tagetes planning (variety selection)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;tag_planning(cv,12+(i*4),25+(i*8),7+(i%4),8+(i%3),45+(i*5),(i%5)+1);}
ps("\nTagetes execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;tag_execution(cv,15+(i*3),30+(i*7),8+(i%3),9+(i%2),50+(i*4),(i%4)+1);}
ps("\nTagetes evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;tag_evaluation(cv,18+(i*3),35+(i*6),9+(i%3),10+(i%2),55+(i*3),(i%3)+2);}
ps("\nTagetes companion planting...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;tag_companion(cv,10+(i*3),20+(i*7),6+(i%4),7+(i%3),40+(i*4),1);}
ps("\nTagetes ornamental market...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;tag_market(cv,20+(i*3),40+(i*5),10+(i%2),11+(i%2),60+(i*3),(i%3)+1);}
ps("\n");tag_report();tag_state();ps("\n=== Demo Complete ===\n");return 0;}
