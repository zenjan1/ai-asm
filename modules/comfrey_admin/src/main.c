/* comfrey_admin: Comfrey (Symphytum officinale) medicinal herb cultivation (v1.0)
 * Comfrey planting, feeding, cutting, harvest, market
 * Features: plant_ht_cm, leaf_span_cm, root_depth_cm, flower_count, allantoin_pct, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,lf_sp,root_dp,fl_ct,alnt_pct,harv_wk,active;} cmf_t;
typedef struct{int n_plant,n_feed,n_cut,n_harv,n_mkt,t_ht,t_sp,t_root,t_fl,t_alnt;} cmf_state_t;
static cmf_t cmfps[N],cmffd[N-2],cmfct[N-4],cmfhv[N-6],cmfmk[N-6]; static cmf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cmf_t*a,int*cnt,int*sum,int mx,int lc,int ph,int ls,int rd,int fc,int ap,int hw){if(*cnt>=mx)return -1;cmf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->lf_sp=ls;x->root_dp=rd;x->fl_ct=fc;x->alnt_pct=ap;x->harv_wk=hw;x->active=1;*sum+=ph;(*cnt)++;ps("[CMF] Comfrey ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" ls=");pi(ls);ps(" rd=");pi(rd);ps(" fc=");pi(fc);ps(" ap=");pi(ap);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cmf_init(void){if(init)return -1;st.n_plant=0;st.n_feed=0;st.n_cut=0;st.n_harv=0;st.n_mkt=0;st.t_ht=0;st.t_sp=0;st.t_root=0;st.t_fl=0;st.t_alnt=0;for(int i=0;i<N;i++)cmfps[i].active=0;for(int i=0;i<N-2;i++)cmffd[i].active=0;for(int i=0;i<N-4;i++)cmfct[i].active=0;for(int i=0;i<N-6;i++)cmfhv[i].active=0;for(int i=0;i<N-6;i++)cmfmk[i].active=0;init=1;ps("[CMF] Comfrey initialized\n");return 0;}
int cmf_planting(int lc,int ph,int ls,int rd,int fc,int ap,int hw){return add(cmfps,&st.n_plant,&st.t_ht,N,lc,ph,ls,rd,fc,ap,hw);}
int cmf_feeding(int lc,int ph,int ls,int rd,int fc,int ap,int hw){return add(cmffd,&st.n_feed,&st.t_sp,N-2,lc,ph,ls,rd,fc,ap,hw);}
int cmf_cutting(int lc,int ph,int ls,int rd,int fc,int ap,int hw){return add(cmfct,&st.n_cut,&st.t_root,N-4,lc,ph,ls,rd,fc,ap,hw);}
int cmf_harvest(int lc,int ph,int ls,int rd,int fc,int ap,int hw){return add(cmfhv,&st.n_harv,&st.t_fl,N-6,lc,ph,ls,rd,fc,ap,hw);}
int cmf_market(int lc,int ph,int ls,int rd,int fc,int ap,int hw){return add(cmfmk,&st.n_mkt,&st.t_alnt,N-6,lc,ph,ls,rd,fc,ap,hw);}
void cmf_report(void){ps("[CMF] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nFeed: ");pi(st.n_feed);ps(" Sp=");pi(st.t_sp);ps("\nCut: ");pi(st.n_cut);ps(" Root=");pi(st.t_root);ps("\nHarv: ");pi(st.n_harv);ps(" Fl=");pi(st.t_fl);ps("\nMkt: ");pi(st.n_mkt);ps(" Alnt=");pi(st.t_alnt);ps("\n");}
void cmf_state(void){ps("[CMF] Plant=");pi(st.n_plant);ps(" Feed=");pi(st.n_feed);ps(" Cut=");pi(st.n_cut);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Comfrey Admin Demo ===\n\n");cmf_init();
/* 1=garden 2=herb_bed 3=medicinal 4=greenhouse 5=market */
ps("Comfrey planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=40+(i*5),ls=20+(i*4),rd=30+(i*5),fc=5+(i*3),ap=2+(i%4),hw=12+(i%5);cmf_planting(lc,ph,ls,rd,fc,ap,hw);}
ps("\nComfrey feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=45+(i*4),ls=22+(i*3),rd=35+(i*4),fc=6+(i*2),ap=3+(i%3),hw=14+(i%4);cmf_feeding(lc,ph,ls,rd,fc,ap,hw);}
ps("\nComfrey cutting...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=50+(i*4),ls=25+(i*3),rd=40+(i*3),fc=7+(i*2),ap=4+(i%3),hw=16+(i%3);cmf_cutting(lc,ph,ls,rd,fc,ap,hw);}
ps("\nComfrey harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=35+(i*6),ls=18+(i*5),rd=25+(i*6),fc=4+(i*4),ap=1+(i%5),hw=10+(i%6);cmf_harvest(lc,ph,ls,rd,fc,ap,hw);}
ps("\nComfrey market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=55+(i*3),ls=28+(i*3),rd=45+(i*3),fc=8+(i*2),ap=5+(i%2),hw=18+(i%2);cmf_market(lc,ph,ls,rd,fc,ap,hw);}
ps("\n");cmf_report();cmf_state();ps("\n=== Demo Complete ===\n");return 0;}
