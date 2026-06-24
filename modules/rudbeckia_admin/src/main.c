/* rudbeckia_admin: Rudbeckia (Black-eyed Susan) wildflower administration (v1.0)
 * Rudbeckia planning, execution, evaluation, seed collection, meadow marketing
 * Features: seed collection, deadheading schedule, pollinator habitat, companion planting
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,flower_dia,bloom_wk,self_seed,butterfly,companion,height,color,active;} rud_t;
typedef struct{int n_plan,n_exec,n_eval,n_seed,n_mkt,t_dia,t_bloom,t_seed_r,t_bfly,t_ht;} rud_state_t;
static rud_t rudps[N],rudes[N-2],rudvs[N-4],rudss[N-6],rudms[N-6]; static rud_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rud_t*a,int*cnt,int*sum,int mx,int cv,int fd,int bw,int ss,int bf,int co,int ht){if(*cnt>=mx)return -1;rud_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->flower_dia=fd;x->bloom_wk=bw;x->self_seed=ss;x->butterfly=bf;x->companion=co;x->height=ht;x->active=1;*sum+=fd;(*cnt)++;ps("[RUD] Rudbeckia ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" fd=");pi(fd);ps(" bw=");pi(bw);ps(" ss=");pi(ss);ps(" bf=");pi(bf);ps("\n");return *cnt-1;}
int rud_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_seed=0;st.n_mkt=0;st.t_dia=0;st.t_bloom=0;st.t_seed_r=0;st.t_bfly=0;st.t_ht=0;for(int i=0;i<N;i++)rudps[i].active=0;for(int i=0;i<N-2;i++)rudes[i].active=0;for(int i=0;i<N-4;i++)rudvs[i].active=0;for(int i=0;i<N-6;i++)rudss[i].active=0;for(int i=0;i<N-6;i++)rudms[i].active=0;init=1;ps("[RUD] Rudbeckia (Black-eyed Susan) initialized\n");return 0;}
/* 1=Goldsturm 2=Cherry Brandy 3=Indian Summer 4=Prairie Sun 5=Little Goldy */
int rud_planning(int cv,int fd,int bw,int ss,int bf,int co,int ht){return add(rudps,&st.n_plan,&st.t_dia,N,cv,fd,bw,ss,bf,co,ht);}
int rud_execution(int cv,int fd,int bw,int ss,int bf,int co,int ht){return add(rudes,&st.n_exec,&st.t_bloom,N-2,cv,fd,bw,ss,bf,co,ht);}
int rud_evaluation(int cv,int fd,int bw,int ss,int bf,int co,int ht){return add(rudvs,&st.n_eval,&st.t_seed_r,N-4,cv,fd,bw,ss,bf,co,ht);}
int rud_seed(int cv,int fd,int bw,int ss,int bf,int co,int ht){return add(rudss,&st.n_seed,&st.t_bfly,N-6,cv,fd,bw,ss,bf,co,ht);}
int rud_market(int cv,int fd,int bw,int ss,int bf,int co,int ht){return add(rudms,&st.n_mkt,&st.t_ht,N-6,cv,fd,bw,ss,bf,co,ht);}
void rud_report(void){ps("[RUD] Plan: ");pi(st.n_plan);ps(" dia=");pi(st.t_dia);ps("\nExec: ");pi(st.n_exec);ps(" bloom=");pi(st.t_bloom);ps("\nEval: ");pi(st.n_eval);ps(" seed=");pi(st.t_seed_r);ps("\nSeed: ");pi(st.n_seed);ps(" bfly=");pi(st.t_bfly);ps("\nMkt: ");pi(st.n_mkt);ps(" ht=");pi(st.t_ht);ps("\n");}
void rud_state(void){ps("[RUD] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Seed=");pi(st.n_seed);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Rudbeckia (Black-eyed Susan) Admin Demo ===\n\n");rud_init();
ps("Rudbeckia planning (meadow layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;rud_planning(cv,95+(i*4),22+(i%8),70+(i*5),12+(i%6),(i%4)+1,65+(i*8));}
ps("\nRudbeckia execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;rud_execution(cv,100+(i*3),24+(i%7),75+(i*4),14+(i%5),(i%3)+2,70+(i*7));}
ps("\nRudbeckia evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;rud_evaluation(cv,105+(i*4),26+(i%6),80+(i*3),16+(i%4),(i%3)+2,75+(i*6));}
ps("\nRudbeckia seed collection...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;rud_seed(cv,85+(i*3),20+(i%5),60+(i*4),10+(i%5),(i%4)+1,55+(i*7));}
ps("\nRudbeckia meadow marketing...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;rud_market(cv,110+(i*3),28+(i%6),85+(i*3),18+(i%4),(i%3)+2,80+(i*5));}
ps("\n");rud_report();rud_state();ps("\n=== Demo Complete ===\n");return 0;}
