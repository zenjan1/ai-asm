/* tropaeolum_admin: Tropaeolum (Nasturtium) edible flower and companion planting (v1.0)
 * Tropaeolum planning, execution, evaluation, pest trap, edible flower market
 * Features: variety tracking (trailing/climbing), flower color, edibility rating, pest trap
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,flower_ct,color,edible_gr,pest_tr,vine_len,habit,active;} tro_t;
typedef struct{int n_plan,n_exec,n_eval,n_trap,n_mkt,t_flower,t_color,t_edible,t_pest,t_vine;} tro_state_t;
static tro_t tropss[N],troes[N-2],trovs[N-4],trots[N-6],troms[N-6]; static tro_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tro_t*a,int*cnt,int*sum,int mx,int cv,int fc,int cl,int eg,int pt,int vl,int hb){if(*cnt>=mx)return -1;tro_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->flower_ct=fc;x->color=cl;x->edible_gr=eg;x->pest_tr=pt;x->vine_len=vl;x->habit=hb;x->active=1;*sum+=fc;(*cnt)++;ps("[TRO] Tropaeolum ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" fl=");pi(fc);ps(" cl=");pi(cl);ps(" eg=");pi(eg);ps(" pt=");pi(pt);ps("\n");return *cnt-1;}
int tro_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_trap=0;st.n_mkt=0;st.t_flower=0;st.t_color=0;st.t_edible=0;st.t_pest=0;st.t_vine=0;for(int i=0;i<N;i++)tropss[i].active=0;for(int i=0;i<N-2;i++)troes[i].active=0;for(int i=0;i<N-4;i++)trovs[i].active=0;for(int i=0;i<N-6;i++)trots[i].active=0;for(int i=0;i<N-6;i++)troms[i].active=0;init=1;ps("[TRO] Tropaeolum (nasturtium) initialized\n");return 0;}
/* 1=majus(trailing) 2=peregrinum(climbing) 3=tuberosum 4=minus(dwarf) 5=speciosum */
int tro_planning(int cv,int fc,int cl,int eg,int pt,int vl,int hb){return add(tropss,&st.n_plan,&st.t_flower,N,cv,fc,cl,eg,pt,vl,hb);}
int tro_execution(int cv,int fc,int cl,int eg,int pt,int vl,int hb){return add(troes,&st.n_exec,&st.t_color,N-2,cv,fc,cl,eg,pt,vl,hb);}
int tro_evaluation(int cv,int fc,int cl,int eg,int pt,int vl,int hb){return add(trovs,&st.n_eval,&st.t_edible,N-4,cv,fc,cl,eg,pt,vl,hb);}
int tro_pest_trap(int cv,int fc,int cl,int eg,int pt,int vl,int hb){return add(trots,&st.n_trap,&st.t_pest,N-6,cv,fc,cl,eg,pt,vl,hb);}
int tro_market(int cv,int fc,int cl,int eg,int pt,int vl,int hb){return add(troms,&st.n_mkt,&st.t_vine,N-6,cv,fc,cl,eg,pt,vl,hb);}
void tro_report(void){ps("[TRO] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" color=");pi(st.t_color);ps("\nEval: ");pi(st.n_eval);ps(" edible=");pi(st.t_edible);ps("\nTrap: ");pi(st.n_trap);ps(" pest=");pi(st.t_pest);ps("\nMkt: ");pi(st.n_mkt);ps(" vine=");pi(st.t_vine);ps("\n");}
void tro_state(void){ps("[TRO] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Trap=");pi(st.n_trap);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Tropaeolum (Nasturtium) Admin Demo ===\n\n");tro_init();
ps("Tropaeolum planning (garden layout)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;tro_planning(cv,8+(i*3),(i%6)+1,(i%4)+1,(i%5)+1,60+(i*15),(i%3)+1);}
ps("\nTropaeolum execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;tro_execution(cv,10+(i*2),(i%5)+1,(i%3)+2,(i%4)+2,70+(i*12),(i%3)+1);}
ps("\nTropaeolum evaluation (edibility check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;tro_evaluation(cv,12+(i*2),(i%4)+2,(i%3)+2,(i%3)+2,80+(i*10),(i%2)+2);}
ps("\nTropaeolum pest trap monitoring...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;tro_pest_trap(cv,6+(i*3),(i%6)+1,(i%4)+1,(i%5)+1,50+(i*14),1);}
ps("\nTropaeolum edible flower market...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;tro_market(cv,14+(i*2),(i%5)+2,(i%3)+2,(i%3)+2,90+(i*8),(i%3)+1);}
ps("\n");tro_report();tro_state();ps("\n=== Demo Complete ===\n");return 0;}
