/* viburnum_admin: Viburnum flowering shrub and ornamental berry plant management (v1.0)
 * Viburnum planning, planting, evaluation, pruning, market
 * Features: berry cluster count, bloom scent, autumn color, shrub height, wildlife attraction
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,berry_ct,bloom_scent,autumn_col,shrub_ht,wildlife_at,prune_mo,active;} vbr_t;
typedef struct{int n_plan,n_exec,n_eval,n_prune,n_mkt,t_berry,t_scent,t_autumn,t_shrub,t_wildlife;} vbr_state_t;
static vbr_t vbrps[N],vbres[N-2],vbrvs[N-4],vbrpm[N-6],vbrms[N-6]; static vbr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(vbr_t*a,int*cnt,int*sum,int mx,int lc,int bc,int bs,int ac,int sh,int wa,int pm){if(*cnt>=mx)return -1;vbr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->berry_ct=bc;x->bloom_scent=bs;x->autumn_col=ac;x->shrub_ht=sh;x->wildlife_at=wa;x->prune_mo=pm;x->active=1;*sum+=bc;(*cnt)++;ps("[VBR] Viburnum ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bc=");pi(bc);ps(" bs=");pi(bs);ps(" ac=");pi(ac);ps(" sh=");pi(sh);ps(" wa=");pi(wa);ps("\n");return *cnt-1;}
int vbr_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_prune=0;st.n_mkt=0;st.t_berry=0;st.t_scent=0;st.t_autumn=0;st.t_shrub=0;st.t_wildlife=0;for(int i=0;i<N;i++)vbrps[i].active=0;for(int i=0;i<N-2;i++)vbres[i].active=0;for(int i=0;i<N-4;i++)vbrvs[i].active=0;for(int i=0;i<N-6;i++)vbrpm[i].active=0;for(int i=0;i<N-6;i++)vbrms[i].active=0;init=1;ps("[VBR] Viburnum initialized\n");return 0;}
/* 1=hedge 2=specimen 3=woodland_edge 4=wildlife_garden 5=foundation */
int vbr_planning(int lc,int bc,int bs,int ac,int sh,int wa,int pm){return add(vbrps,&st.n_plan,&st.t_berry,N,lc,bc,bs,ac,sh,wa,pm);}
int vbr_execution(int lc,int bc,int bs,int ac,int sh,int wa,int pm){return add(vbres,&st.n_exec,&st.t_scent,N-2,lc,bc,bs,ac,sh,wa,pm);}
int vbr_evaluation(int lc,int bc,int bs,int ac,int sh,int wa,int pm){return add(vbrvs,&st.n_eval,&st.t_autumn,N-4,lc,bc,bs,ac,sh,wa,pm);}
int vbr_pruning(int lc,int bc,int bs,int ac,int sh,int wa,int pm){return add(vbrpm,&st.n_prune,&st.t_shrub,N-6,lc,bc,bs,ac,sh,wa,pm);}
int vbr_market(int lc,int bc,int bs,int ac,int sh,int wa,int pm){return add(vbrms,&st.n_mkt,&st.t_wildlife,N-6,lc,bc,bs,ac,sh,wa,pm);}
void vbr_report(void){ps("[VBR] Plan: ");pi(st.n_plan);ps(" berry=");pi(st.t_berry);ps("\nExec: ");pi(st.n_exec);ps(" scent=");pi(st.t_scent);ps("\nEval: ");pi(st.n_eval);ps(" autumn=");pi(st.t_autumn);ps("\nPrune: ");pi(st.n_prune);ps(" shrub=");pi(st.t_shrub);ps("\nMkt: ");pi(st.n_mkt);ps(" wildlife=");pi(st.t_wildlife);ps("\n");}
void vbr_state(void){ps("[VBR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Prune=");pi(st.n_prune);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Viburnum Admin Demo ===\n\n");vbr_init();
ps("Viburnum planning (shrub layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;vbr_planning(lc,15+(i*4),(i%7)+1,(i%6)+1,100+(i*15),(i%5)+1,3+(i%3));}
ps("\nViburnum execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;vbr_execution(lc,18+(i*3),(i%7)+1,(i%6)+1,110+(i*12),(i%5)+1,3+(i%3));}
ps("\nViburnum evaluation (autumn color)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;vbr_evaluation(lc,20+(i*3),(i%6)+2,(i%6)+2,120+(i*10),(i%5)+2,4+(i%2));}
ps("\nViburnum pruning management...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;vbr_pruning(lc,12+(i*3),(i%5)+1,(i%5)+1,90+(i*10),(i%4)+1,3+(i%3));}
ps("\nViburnum ornamental market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;vbr_market(lc,22+(i*3),(i%7)+2,(i%6)+2,130+(i*8),(i%5)+2,4+(i%2));}
ps("\n");vbr_report();vbr_state();ps("\n=== Demo Complete ===\n");return 0;}
