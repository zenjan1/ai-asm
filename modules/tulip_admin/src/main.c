/* tulip_admin: Tulip (Tulipa) bulb flower cultivation and hybrid development (v1.0)
 * Tulip planning, execution, evaluation, bulb propagation, flower market
 * Features: cultivar tracking, bloom timing, bulb multiplication, forcing, color classification
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,bloom_wk,bulb_ct,ht_cm,color_cl,force_d,grade,active;} tul_t;
typedef struct{int n_plan,n_exec,n_eval,n_bulb,n_mkt,t_bloom,t_bulb_s,t_ht,t_color,t_force;} tul_state_t;
static tul_t tulps[N],tules[N-2],tulvs[N-4],tulbs[N-6],tulms[N-6]; static tul_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tul_t*a,int*cnt,int*sum,int mx,int cv,int bw,int bc,int ht,int cl,int fd,int gr){if(*cnt>=mx)return -1;tul_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->bloom_wk=bw;x->bulb_ct=bc;x->ht_cm=ht;x->color_cl=cl;x->force_d=fd;x->grade=gr;x->active=1;*sum+=bw;(*cnt)++;ps("[TUL] Tulip ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" bw=");pi(bw);ps(" bc=");pi(bc);ps(" ht=");pi(ht);ps(" cl=");pi(cl);ps("\n");return *cnt-1;}
int tul_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_bulb=0;st.n_mkt=0;st.t_bloom=0;st.t_bulb_s=0;st.t_ht=0;st.t_color=0;st.t_force=0;for(int i=0;i<N;i++)tulps[i].active=0;for(int i=0;i<N-2;i++)tules[i].active=0;for(int i=0;i<N-4;i++)tulvs[i].active=0;for(int i=0;i<N-6;i++)tulbs[i].active=0;for(int i=0;i<N-6;i++)tulms[i].active=0;init=1;ps("[TUL] Tulip (bulb flower) initialized\n");return 0;}
/* 1=Single Early 2=Double Early 3=Triumph 4=Darwin 5=Parrot */
int tul_planning(int cv,int bw,int bc,int ht,int cl,int fd,int gr){return add(tulps,&st.n_plan,&st.t_bloom,N,cv,bw,bc,ht,cl,fd,gr);}
int tul_execution(int cv,int bw,int bc,int ht,int cl,int fd,int gr){return add(tules,&st.n_exec,&st.t_bulb_s,N-2,cv,bw,bc,ht,cl,fd,gr);}
int tul_evaluation(int cv,int bw,int bc,int ht,int cl,int fd,int gr){return add(tulvs,&st.n_eval,&st.t_ht,N-4,cv,bw,bc,ht,cl,fd,gr);}
int tul_bulb(int cv,int bw,int bc,int ht,int cl,int fd,int gr){return add(tulbs,&st.n_bulb,&st.t_color,N-6,cv,bw,bc,ht,cl,fd,gr);}
int tul_market(int cv,int bw,int bc,int ht,int cl,int fd,int gr){return add(tulms,&st.n_mkt,&st.t_force,N-6,cv,bw,bc,ht,cl,fd,gr);}
void tul_report(void){ps("[TUL] Plan: ");pi(st.n_plan);ps(" bloom=");pi(st.t_bloom);ps("\nExec: ");pi(st.n_exec);ps(" bulb=");pi(st.t_bulb_s);ps("\nEval: ");pi(st.n_eval);ps(" ht=");pi(st.t_ht);ps("\nBulb: ");pi(st.n_bulb);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" force=");pi(st.t_force);ps("\n");}
void tul_state(void){ps("[TUL] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Bulb=");pi(st.n_bulb);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tulip (Bulb Flower) Admin Demo ===\n\n");tul_init();
ps("Tulip planning (bed layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;tul_planning(cv,10+(i%8),3+(i%4),30+(i*5),(i%7)+1,60+(i*5),(i%3)+1);}
ps("\nTulip execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;tul_execution(cv,12+(i%7),4+(i%3),35+(i*4),(i%6)+1,65+(i*4),(i%3)+1);}
ps("\nTulip evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;tul_evaluation(cv,14+(i%6),5+(i%3),40+(i*3),(i%5)+2,70+(i*3),(i%2)+2);}
ps("\nTulip bulb propagation...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;tul_bulb(cv,8+(i%5),2+(i%3),25+(i*4),(i%4)+1,50+(i*5),1);}
ps("\nTulip flower market...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;tul_market(cv,16+(i%5),6+(i%2),45+(i*3),(i%5)+2,75+(i*3),(i%3)+1);}
ps("\n");tul_report();tul_state();ps("\n=== Demo Complete ===\n");return 0;}
