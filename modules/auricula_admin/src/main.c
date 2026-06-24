/* auricula_admin: Auricula (Alpine Primrose) rock garden management (v1.0)
 * Auricula planning, planting, evaluation, division, market
 * Features: flower diameter, petal layers, stem height, flower color, leaf shape, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,flower_dia,petal_lyr,stem_ht,flower_color,leaf_shp,bloom_wk,active;} aur_t;
typedef struct{int n_plan,n_exec,n_eval,n_div,n_mkt,t_flower,t_petal,t_stem,t_color,t_leaf;} aur_state_t;
static aur_t aurps[N],aures[N-2],aurvs[N-4],aurdv[N-6],aurms[N-6]; static aur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aur_t*a,int*cnt,int*sum,int mx,int lc,int fd,int pl,int sh,int fc,int ls,int bw){if(*cnt>=mx)return -1;aur_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->flower_dia=fd;x->petal_lyr=pl;x->stem_ht=sh;x->flower_color=fc;x->leaf_shp=ls;x->bloom_wk=bw;x->active=1;*sum+=fd;(*cnt)++;ps("[AUR] Auricula ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" fd=");pi(fd);ps(" pl=");pi(pl);ps(" sh=");pi(sh);ps(" fc=");pi(fc);ps(" ls=");pi(ls);ps("\n");return *cnt-1;}
int aur_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_div=0;st.n_mkt=0;st.t_flower=0;st.t_petal=0;st.t_stem=0;st.t_color=0;st.t_leaf=0;for(int i=0;i<N;i++)aurps[i].active=0;for(int i=0;i<N-2;i++)aures[i].active=0;for(int i=0;i<N-4;i++)aurvs[i].active=0;for(int i=0;i<N-6;i++)aurdv[i].active=0;for(int i=0;i<N-6;i++)aurms[i].active=0;init=1;ps("[AUR] Auricula initialized\n");return 0;}
/* 1=rock_garden 2=alpine_house 3=container 4=crevice 5=trough */
int aur_planning(int lc,int fd,int pl,int sh,int fc,int ls,int bw){return add(aurps,&st.n_plan,&st.t_flower,N,lc,fd,pl,sh,fc,ls,bw);}
int aur_execution(int lc,int fd,int pl,int sh,int fc,int ls,int bw){return add(aures,&st.n_exec,&st.t_petal,N-2,lc,fd,pl,sh,fc,ls,bw);}
int aur_evaluation(int lc,int fd,int pl,int sh,int fc,int ls,int bw){return add(aurvs,&st.n_eval,&st.t_stem,N-4,lc,fd,pl,sh,fc,ls,bw);}
int aur_division(int lc,int fd,int pl,int sh,int fc,int ls,int bw){return add(aurdv,&st.n_div,&st.t_color,N-6,lc,fd,pl,sh,fc,ls,bw);}
int aur_market(int lc,int fd,int pl,int sh,int fc,int ls,int bw){return add(aurms,&st.n_mkt,&st.t_leaf,N-6,lc,fd,pl,sh,fc,ls,bw);}
void aur_report(void){ps("[AUR] Plan: ");pi(st.n_plan);ps(" flower=");pi(st.t_flower);ps("\nExec: ");pi(st.n_exec);ps(" petal=");pi(st.t_petal);ps("\nEval: ");pi(st.n_eval);ps(" stem=");pi(st.t_stem);ps("\nDiv: ");pi(st.n_div);ps(" color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" leaf=");pi(st.t_leaf);ps("\n");}
void aur_state(void){ps("[AUR] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Div=");pi(st.n_div);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Auricula (Alpine Primrose) Admin Demo ===\n\n");aur_init();
ps("Auricula planning (rock garden layout)...\n");
for(int i=0;i<N;i++){int lc=(i%5)+1;aur_planning(lc,3+(i*2),2+(i%4),8+(i*3),(i%7)+1,3+(i%3),14+(i%6));}
ps("\nAuricula execution (planting)...\n");
for(int i=0;i<N-2;i++){int lc=(i%4)+2;aur_execution(lc,4+(i*2),3+(i%3),10+(i*3),(i%7)+1,4+(i%2),16+(i%5));}
ps("\nAuricula evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int lc=(i%3)+1;aur_evaluation(lc,5+(i*2),3+(i%3),12+(i*2),(i%6)+2,5+(i%2),18+(i%4));}
ps("\nAuricula division...\n");
for(int i=0;i<N-6;i++){int lc=(i%5)+1;aur_division(lc,2+(i*2),2+(i%4),7+(i*3),(i%5)+1,3+(i%3),12+(i%5));}
ps("\nAuricula alpine flower market...\n");
for(int i=0;i<N-6;i++){int lc=(i%4)+1;aur_market(lc,6+(i*2),4+(i%2),(i%6)+2,6+(i%2),55+(i*4),20+(i%3));}
ps("\n");aur_report();aur_state();ps("\n=== Demo Complete ===\n");return 0;}
