/* celosia_admin: Celosia (Celosia argentea) ornamental cockscomb flower management (v1.0)
 * Celosia propagation, bloom, drying, exhibition, market
 * Features: spike height, spike width, petal count, color intensity, stem count, bloom week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,spk_ht,spk_wd,petal_ct,clr_int,stm_ct,bloom_wk,active;} cel_t;
typedef struct{int n_prop,n_bloom,n_dry,n_exhibit,n_mkt,t_spike,t_width,t_petal,t_color,t_stem;} cel_state_t;
static cel_t celpr[N],celbs[N-2],celdrs[N-4],celex[N-6],celms[N-6]; static cel_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cel_t*a,int*cnt,int*sum,int mx,int lc,int sh,int sw,int pc,int ci,int sc,int bw){if(*cnt>=mx)return -1;cel_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->spk_ht=sh;x->spk_wd=sw;x->petal_ct=pc;x->clr_int=ci;x->stm_ct=sc;x->bloom_wk=bw;x->active=1;*sum+=sh;(*cnt)++;ps("[CEL] Celosia ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" sh=");pi(sh);ps(" sw=");pi(sw);ps(" pc=");pi(pc);ps(" ci=");pi(ci);ps(" sc=");pi(sc);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int cel_init(void){if(init)return -1;st.n_prop=0;st.n_bloom=0;st.n_dry=0;st.n_exhibit=0;st.n_mkt=0;st.t_spike=0;st.t_width=0;st.t_petal=0;st.t_color=0;st.t_stem=0;for(int i=0;i<N;i++)celpr[i].active=0;for(int i=0;i<N-2;i++)celbs[i].active=0;for(int i=0;i<N-4;i++)celdrs[i].active=0;for(int i=0;i<N-6;i++)celex[i].active=0;for(int i=0;i<N-6;i++)celms[i].active=0;init=1;ps("[CEL] Celosia initialized\n");return 0;}
int cel_propagation(int lc,int sh,int sw,int pc,int ci,int sc,int bw){return add(celpr,&st.n_prop,&st.t_spike,N,lc,sh,sw,pc,ci,sc,bw);}
int cel_bloom(int lc,int sh,int sw,int pc,int ci,int sc,int bw){return add(celbs,&st.n_bloom,&st.t_width,N-2,lc,sh,sw,pc,ci,sc,bw);}
int cel_drying(int lc,int sh,int sw,int pc,int ci,int sc,int bw){return add(celdrs,&st.n_dry,&st.t_petal,N-4,lc,sh,sw,pc,ci,sc,bw);}
int cel_exhibition(int lc,int sh,int sw,int pc,int ci,int sc,int bw){return add(celex,&st.n_exhibit,&st.t_color,N-6,lc,sh,sw,pc,ci,sc,bw);}
int cel_market(int lc,int sh,int sw,int pc,int ci,int sc,int bw){return add(celms,&st.n_mkt,&st.t_stem,N-6,lc,sh,sw,pc,ci,sc,bw);}
void cel_report(void){ps("[CEL] Prop: ");pi(st.n_prop);ps(" Spike=");pi(st.t_spike);ps("\nBloom: ");pi(st.n_bloom);ps(" Width=");pi(st.t_width);ps("\nDry: ");pi(st.n_dry);ps(" Petal=");pi(st.t_petal);ps("\nExhibit: ");pi(st.n_exhibit);ps(" Color=");pi(st.t_color);ps("\nMkt: ");pi(st.n_mkt);ps(" Stem=");pi(st.t_stem);ps("\n");}
void cel_state(void){ps("[CEL] Prop=");pi(st.n_prop);ps(" Bloom=");pi(st.n_bloom);ps(" Dry=");pi(st.n_dry);ps(" Exhibit=");pi(st.n_exhibit);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Celosia Admin Demo ===\n\n");cel_init();
/* 1=garden 2=border 3=container 4=cut_flower 5=dried_arrangement */
ps("Celosia propagation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,sh=15+(i*5),sw=3+(i*2),pc=20+(i*10),ci=5+(i%6),sc=3+(i*2),bw=16+(i%4);cel_propagation(lc,sh,sw,pc,ci,sc,bw);}
ps("\nCelosia bloom...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,sh=18+(i*4),sw=4+(i*2),pc=25+(i*8),ci=6+(i%5),sc=4+(i*2),bw=18+(i%3);cel_bloom(lc,sh,sw,pc,ci,sc,bw);}
ps("\nCelosia drying...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,sh=20+(i*3),sw=5+(i*2),pc=30+(i*6),ci=7+(i%4),sc=5+(i*2),bw=20+(i%3);cel_drying(lc,sh,sw,pc,ci,sc,bw);}
ps("\nCelosia exhibition...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,sh=12+(i*6),sw=2+(i*3),pc=15+(i*12),ci=4+(i%7),sc=2+(i*3),bw=14+(i%5);cel_exhibition(lc,sh,sw,pc,ci,sc,bw);}
ps("\nCelosia market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,sh=22+(i*3),sw=6+(i*2),pc=35+(i*5),ci=8+(i%3),sc=6+(i*2),bw=22+(i%2);cel_market(lc,sh,sw,pc,ci,sc,bw);}
ps("\n");cel_report();cel_state();ps("\n=== Demo Complete ===\n");return 0;}
