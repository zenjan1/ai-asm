/* berry_admin: Berry (Mixed Berry) fruit production management (v1.0)
 * Berry planning, planting, evaluation, pruning, market
 * Features: fruit diameter, fruit count, bush height, berry color, sugar brix, harvest week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,fruit_dia,fruit_ct,bush_ht,berry_color,sugar_bx,harvest_wk,active;} berr_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_fruit,t_count,t_ht,t_color,t_sugar;} berr_state_t;
static berr_t berrps[N],berres[N-2],berrvs[N-4],berrpr[N-6],berrms[N-6]; static berr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(berr_t*a,int*cnt,int*sum,int mx,int lc,int fd,int fct,int bh,int bc,int sb,int hw){if(*cnt>=mx)return -1;berr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->fruit_dia=fd;x->fruit_ct=fct;x->bush_ht=bh;x->berry_color=bc;x->sugar_bx=sb;x->harvest_wk=hw;x->active=1;*sum+=fd;(*cnt)++;ps("[BERR] Berry ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" fct=");pi(fct);ps(" bh=");pi(bh);ps(" bc=");pi(bc);ps(" sb=");pi(sb);ps("\n");return *cnt-1;}
int berr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_fruit=0;st.t_count=0;st.t_ht=0;st.t_color=0;st.t_sugar=0;for(int i=0;i<N;i++)berrps[i].active=0;for(int i=0;i<N-2;i++)berres[i].active=0;for(int i=0;i<N-4;i++)berrvs[i].active=0;for(int i=0;i<N-6;i++)berrpr[i].active=0;for(int i=0;i<N-6;i++)berrms[i].active=0;init=1;ps("[BERR] Berry initialized\n");return 0;}
/* 1=orchard 2=hedgerow 3=container 4=polytunnel 5=field */
int berr_planning(int lc,int fd,int fct,int bh,int bc,int sb,int hw){return add(berrps,&st.n_plan,&st.t_fruit,N,lc,fd,fct,bh,bc,sb,hw);}
int berr_execution(int lc,int fd,int fct,int bh,int bc,int sb,int hw){return add(berres,&st.n_exec,&st.t_count,N-2,lc,fd,fct,bh,bc,sb,hw);}
int berr_evaluation(int lc,int fd,int fct,int bh,int bc,int sb,int hw){return add(berrvs,&st.n_eval,&st.t_ht,N-4,lc,fd,fct,bh,bc,sb,hw);}
int berr_pruning(int lc,int fd,int fct,int bh,int bc,int sb,int hw){return add(berrpr,&st.n_prune,&st.t_color,N-6,lc,fd,fct,bh,bc,sb,hw);}
int berr_market(int lc,int fd,int fct,int bh,int bc,int sb,int hw){return add(berrms,&st.n_mkt,&st.t_sugar,N-6,lc,fd,fct,bh,bc,sb,hw);}
void berr_report(void){ps("[BERR] Plan: ");pi(st.n_plan);ps(" fruit=");pi(st.t_fruit);ps("\nExec: ");pi(st.n_exec);ps(" count=");pi(st.t_count);ps("\nEval: ");pi(st.n_eval);ps(" ht=");pi(st.t_ht);ps("\nPrune: ");pi(st.n_prune);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" sugar=");pi(st.t_sugar);ps("\n");}
void berr_state(void){ps("[BERR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Berry (Mixed Berry) Admin Demo ===\n\n");berr_init();
ps("Berry planning (orchard layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;berr_planning(lc,10+(i*3),8+(i%6),40+(i*8),(i%5)+1,80+(i*5),28+(i%6));}
ps("\nBerry execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;berr_execution(lc,12+(i*2),10+(i%5),45+(i*7),(i%5)+1,85+(i*4),30+(i%5));}
ps("\nBerry evaluation (fruit check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;berr_evaluation(lc,14+(i*2),12+(i%4),50+(i*6),(i%4)+2,90+(i%3),32+(i%4));}
ps("\nBerry pruning...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;berr_pruning(lc,8+(i*3),6+(i%5),35+(i*8),(i%5)+1,75+(i%5),26+(i%5));}
ps("\nBerry fruit production market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;berr_market(lc,16+(i*2),14+(i%3),(i%6)+2,95+(i%3),34+(i%3),55+(i%4));}
ps("\n");berr_report();berr_state();ps("\n=== Demo Complete ===\n");return 0;}
