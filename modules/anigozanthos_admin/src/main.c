/* anigozanthos_admin: Anigozanthos (Kangaroo Paw) Australian native flower management (v1.0)
 * Anigozanthos planning, planting, evaluation, nectar collection, market
 * Features: flower length, fur coverage, stem height, bloom color, leaf fan size, nectar volume
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_len,fur_cov,stem_ht,bloom_color,leaf_fan,nectar_vl,active;} angz_t;
typedef struct{int n_plan,n_exec,n_eval,n_nectar,n_mkt,t_flower,t_fur,t_stem,t_bloom,t_nectar;} angz_state_t;
static angz_t angzps[N],angzes[N-2],angzvs[N-4],angznc[N-6],angzms[N-6]; static angz_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(angz_t*a,int*cnt,int*sum,int mx,int lc,int fl,int fc,int sh,int bc,int lf,int nv){if(*cnt>=mx)return -1;angz_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_len=fl;x->fur_cov=fc;x->stem_ht=sh;x->bloom_color=bc;x->leaf_fan=lf;x->nectar_vl=nv;x->active=1;*sum+=fl;(*cnt)++;ps("[ANGZ] Anigozanthos ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fl=");pi(fl);ps(" fc=");pi(fc);ps(" sh=");pi(sh);ps(" bc=");pi(bc);ps(" lf=");pi(lf);ps("\n");return *cnt-1;}
int angz_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_nectar=0;st.n_mkt=0;st.t_flower=0;st.t_fur=0;st.t_stem=0;st.t_bloom=0;st.t_nectar=0;for(int i=0;i<N;i++)angzps[i].active=0;for(int i=0;i<N-2;i++)angzes[i].active=0;for(int i=0;i<N-4;i++)angzvs[i].active=0;for(int i=0;i<N-6;i++)angznc[i].active=0;for(int i=0;i<N-6;i++)angzms[i].active=0;init=1;ps("[ANGZ] Anigozanthos initialized\n");return 0;}
/* 1=native_garden 2=cutting 3=container 4=xeriscape 5=wildlife */
int angz_planning(int lc,int fl,int fc,int sh,int bc,int lf,int nv){return add(angzps,&st.n_plan,&st.t_flower,N,lc,fl,fc,sh,bc,lf,nv);}
int angz_execution(int lc,int fl,int fc,int sh,int bc,int lf,int nv){return add(angzes,&st.n_exec,&st.t_fur,N-2,lc,fl,fc,sh,bc,lf,nv);}
int angz_evaluation(int lc,int fl,int fc,int sh,int bc,int lf,int nv){return add(angzvs,&st.n_eval,&st.t_stem,N-4,lc,fl,fc,sh,bc,lf,nv);}
int angz_nectar_collection(int lc,int fl,int fc,int sh,int bc,int lf,int nv){return add(angznc,&st.n_nectar,&st.t_bloom,N-6,lc,fl,fc,sh,bc,lf,nv);}
int angz_market(int lc,int fl,int fc,int sh,int bc,int lf,int nv){return add(angzms,&st.n_mkt,&st.t_nectar,N-6,lc,fl,fc,sh,bc,lf,nv);}
void angz_report(void){ps("[ANGZ] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" fur=");pi(st.t_fur);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nNectar: ");pi(st.n_nectar);ps(" bloom=");pi(st.t_bloom);ps("\nMkt: ");pi(st.n_mkt);ps(" nectar=");pi(st.t_nectar);ps("\n");}
void angz_state(void){ps("[ANGZ] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Nectar=");pi(st.n_nectar);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Anigozanthos (Kangaroo Paw) Admin Demo ===\n\n");angz_init();
ps("Anigozanthos planning (native layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;angz_planning(lc,30+(i*8),5+(i%6),60+(i*12),(i%7)+1,15+(i*4),10+(i*5));}
ps("\nAnigozanthos execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;angz_execution(lc,35+(i*6),6+(i%5),70+(i*10),(i%7)+1,18+(i*3),12+(i*4));}
ps("\nAnigozanthos evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;angz_evaluation(lc,40+(i*5),7+(i%4),80+(i*8),(i%6)+2,20+(i*3),15+(i*3));}
ps("\nAnigozanthos nectar collection...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;angz_nectar_collection(lc,25+(i*6),4+(i%5),50+(i*10),(i%5)+1,12+(i*4),8+(i*4));}
ps("\nAnigozanthos native flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;angz_market(lc,45+(i*4),8+(i%3),90+(i*6),(i%6)+2,22+(i*3),18+(i*3));}
ps("\n");angz_report();angz_state();ps("\n=== Demo Complete ===\n");return 0;}
