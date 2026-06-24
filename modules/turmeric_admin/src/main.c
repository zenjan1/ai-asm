/* turmeric_admin: Turmeric (Curcuma longa) spice and medicinal rhizome management (v1.0)
 * Turmeric planning, planting, evaluation, processing, market
 * Features: curcumin content tracking, rhizome yield, harvest timing, processing method, grade
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,variety,curc_pct,yield_kg,harv_mo,proc_type,grade,store_mo,active;} tur_t;
typedef struct{int n_plan,n_exec,n_eval,n_proc,n_mkt,t_curc,t_yield,t_harv,t_proc,t_grade;} tur_state_t;
static tur_t turps[N],tures[N-2],turvs[N-4],turprs[N-6],turms[N-6]; static tur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tur_t*a,int*cnt,int*sum,int mx,int vr,int cp,int yk,int hm,int pt,int gr,int sm){if(*cnt>=mx)return -1;tur_t*x=&a[*cnt];x->id=*cnt;x->variety=vr;x->curc_pct=cp;x->yield_kg=yk;x->harv_mo=hm;x->proc_type=pt;x->grade=gr;x->store_mo=sm;x->active=1;*sum+=cp;(*cnt)++;ps("[TUR] Turmeric ");pi(*cnt-1);ps(" vr=");pi(vr);ps(" cp=");pi(cp);ps(" yk=");pi(yk);ps(" hm=");pi(hm);ps(" pt=");pi(pt);ps(" gr=");pi(gr);ps("\n");return *cnt-1;}
int tur_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_proc=0;st.n_mkt=0;st.t_curc=0;st.t_yield=0;st.t_harv=0;st.t_proc=0;st.t_grade=0;for(int i=0;i<N;i++)turps[i].active=0;for(int i=0;i<N-2;i++)tures[i].active=0;for(int i=0;i<N-4;i++)turvs[i].active=0;for(int i=0;i<N-6;i++)turprs[i].active=0;for(int i=0;i<N-6;i++)turms[i].active=0;init=1;ps("[TUR] Turmeric (Curcuma longa) initialized\n");return 0;}
/* 1=Alleppey 2=Madras 3=Kasthuri 4=Rajendra 5=Pragati */
int tur_planning(int vr,int cp,int yk,int hm,int pt,int gr,int sm){return add(turps,&st.n_plan,&st.t_curc,N,vr,cp,yk,hm,pt,gr,sm);}
int tur_execution(int vr,int cp,int yk,int hm,int pt,int gr,int sm){return add(tures,&st.n_exec,&st.t_yield,N-2,vr,cp,yk,hm,pt,gr,sm);}
int tur_evaluation(int vr,int cp,int yk,int hm,int pt,int gr,int sm){return add(turvs,&st.n_eval,&st.t_harv,N-4,vr,cp,yk,hm,pt,gr,sm);}
int tur_processing(int vr,int cp,int yk,int hm,int pt,int gr,int sm){return add(turprs,&st.n_proc,&st.t_proc,N-6,vr,cp,yk,hm,pt,gr,sm);}
int tur_market(int vr,int cp,int yk,int hm,int pt,int gr,int sm){return add(turms,&st.n_mkt,&st.t_grade,N-6,vr,cp,yk,hm,pt,gr,sm);}
void tur_report(void){ps("[TUR] Plan: ");pi(st.n_plan);ps(" curc=");pi(st.t_curc);ps("\nExec: ");pi(st.n_exec);ps(" yield=");pi(st.t_yield);ps("\nEval: ");pi(st.n_eval);ps(" harv=");pi(st.t_harv);ps("\nProc: ");pi(st.n_proc);ps(" proc=");pi(st.t_proc);ps("\nMkt: ");pi(st.n_mkt);ps(" grade=");pi(st.t_grade);ps("\n");}
void tur_state(void){ps("[TUR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Proc=");pi(st.n_proc);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Turmeric (Curcuma longa) Admin Demo ===\n\n");tur_init();
ps("Turmeric planning (rhizome selection)...\n");
for(int i=0;i<N;i++){int vr=(i%5)+1;tur_planning(vr,30+(i%8),12+(i%6),8+(i%4),(i%3)+1,(i%4)+1,6+(i%3));}
ps("\nTurmeric execution (planting)...\n");
for(int i=0;i<N-2;i++){int vr=(i%4)+2;tur_execution(vr,35+(i%7),15+(i%5),9+(i%3),(i%3)+1,(i%4)+1,7+(i%2));}
ps("\nTurmeric evaluation (harvest assessment)...\n");
for(int i=0;i<N-4;i++){int vr=(i%3)+1;tur_evaluation(vr,40+(i%6),18+(i%4),10+(i%2),(i%3)+1,(i%4)+2,8+(i%2));}
ps("\nTurmeric processing (boiling/drying)...\n");
for(int i=0;i<N-6;i++){int vr=(i%5)+1;tur_processing(vr,45+(i%5),20+(i%3),10+(i%2),(i%3)+1,1,9+(i%2));}
ps("\nTurmeric spice market...\n");
for(int i=0;i<N-6;i++){int vr=(i%4)+1;tur_market(vr,50+(i%4),22+(i%3),11+(i%2),(i%3)+2,(i%3)+1,10+(i%2));}
ps("\n");tur_report();tur_state();ps("\n=== Demo Complete ===\n");return 0;}
