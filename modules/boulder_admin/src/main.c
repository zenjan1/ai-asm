/* boulder_admin: Boulder and landscape rock management for geology and landscaping (v1.0)
 * Boulder quarry, transport, placement, inspection, market
 * Features: weight_kg, diameter_cm, rock_type, surface_tex, origin_quarry, stability_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,wght_kg,dia_cm,rock_tp,surf_tx,origin_q,stab_idx,active;} bldr_t;
typedef struct{int n_quarry,n_trans,n_place,n_insp,n_mkt,t_wght,t_dia,t_rock,t_surf,t_orig;} bldr_state_t;
static bldr_t bldrps[N],bldrts[N-2],bldrlps[N-4],bldris[N-6],bldrms[N-6]; static bldr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bldr_t*a,int*cnt,int*sum,int mx,int lc,int wk,int dc,int rt,int sx,int oq,int si){if(*cnt>=mx)return -1;bldr_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->wght_kg=wk;x->dia_cm=dc;x->rock_tp=rt;x->surf_tx=sx;x->origin_q=oq;x->stab_idx=si;x->active=1;*sum+=wk;(*cnt)++;ps("[BLDR] Boulder ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" wk=");pi(wk);ps(" dc=");pi(dc);ps(" rt=");pi(rt);ps(" sx=");pi(sx);ps(" oq=");pi(oq);ps(" si=");pi(si);ps("\n");return *cnt-1;}
int bldr_init(void){if(init)return -1;st.n_quarry=0;st.n_trans=0;st.n_place=0;st.n_insp=0;st.n_mkt=0;st.t_wght=0;st.t_dia=0;st.t_rock=0;st.t_surf=0;st.t_orig=0;for(int i=0;i<N;i++)bldrps[i].active=0;for(int i=0;i<N-2;i++)bldrts[i].active=0;for(int i=0;i<N-4;i++)bldrlps[i].active=0;for(int i=0;i<N-6;i++)bldris[i].active=0;for(int i=0;i<N-6;i++)bldrms[i].active=0;init=1;ps("[BLDR] Boulder initialized\n");return 0;}
int bldr_quarry(int lc,int wk,int dc,int rt,int sx,int oq,int si){return add(bldrps,&st.n_quarry,&st.t_wght,N,lc,wk,dc,rt,sx,oq,si);}
int bldr_transport(int lc,int wk,int dc,int rt,int sx,int oq,int si){return add(bldrts,&st.n_trans,&st.t_dia,N-2,lc,wk,dc,rt,sx,oq,si);}
int bldr_placement(int lc,int wk,int dc,int rt,int sx,int oq,int si){return add(bldrlps,&st.n_place,&st.t_rock,N-4,lc,wk,dc,rt,sx,oq,si);}
int bldr_inspection(int lc,int wk,int dc,int rt,int sx,int oq,int si){return add(bldris,&st.n_insp,&st.t_surf,N-6,lc,wk,dc,rt,sx,oq,si);}
int bldr_market(int lc,int wk,int dc,int rt,int sx,int oq,int si){return add(bldrms,&st.n_mkt,&st.t_orig,N-6,lc,wk,dc,rt,sx,oq,si);}
void bldr_report(void){ps("[BLDR] Quarry: ");pi(st.n_quarry);ps(" Wght=");pi(st.t_wght);ps("\nTrans: ");pi(st.n_trans);ps(" Dia=");pi(st.t_dia);ps("\nPlace: ");pi(st.n_place);ps(" Rock=");pi(st.t_rock);ps("\nInsp: ");pi(st.n_insp);ps(" Surf=");pi(st.t_surf);ps("\nMkt: ");pi(st.n_mkt);ps(" Origin=");pi(st.t_orig);ps("\n");}
void bldr_state(void){ps("[BLDR] Quarry=");pi(st.n_quarry);ps(" Trans=");pi(st.n_trans);ps(" Place=");pi(st.n_place);ps(" Insp=");pi(st.n_insp);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Boulder Admin Demo ===\n\n");bldr_init();
/* 1=garden 2=retaining_wall 3=water_feature 4=pathway 5=monument */
ps("Boulder quarry...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,wght_kg=100+(i*50),dia_cm=30+(i*15),rock_tp=(i%5)+1,surf_tx=(i%4)+1,origin_q=(i%3)+1,stab_idx=7+(i%3);bldr_quarry(lc,wght_kg,dia_cm,rock_tp,surf_tx,origin_q,stab_idx);}
ps("\nBoulder transport...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,wght_kg=150+(i*40),dia_cm=40+(i*10),rock_tp=(i%4)+2,surf_tx=(i%4)+1,origin_q=(i%3)+1,stab_idx=8+(i%2);bldr_transport(lc,wght_kg,dia_cm,rock_tp,surf_tx,origin_q,stab_idx);}
ps("\nBoulder placement...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,wght_kg=200+(i*30),dia_cm=50+(i*8),rock_tp=(i%3)+1,surf_tx=(i%3)+2,origin_q=(i%3)+2,stab_idx=9+(i%1);bldr_placement(lc,wght_kg,dia_cm,rock_tp,surf_tx,origin_q,stab_idx);}
ps("\nBoulder inspection...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,wght_kg=120+(i*45),dia_cm=35+(i*12),rock_tp=(i%5)+1,surf_tx=(i%4)+1,origin_q=(i%3)+1,stab_idx=6+(i%3);bldr_inspection(lc,wght_kg,dia_cm,rock_tp,surf_tx,origin_q,stab_idx);}
ps("\nBoulder market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,wght_kg=250+(i*25),dia_cm=60+(i*5),rock_tp=(i%4)+2,surf_tx=(i%3)+1,origin_q=(i%3)+3,stab_idx=10;bldr_market(lc,wght_kg,dia_cm,rock_tp,surf_tx,origin_q,stab_idx);}
ps("\n");bldr_report();bldr_state();ps("\n=== Demo Complete ===\n");return 0;}
