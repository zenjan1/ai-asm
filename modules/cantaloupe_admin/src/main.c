/* cantaloupe_admin: Cantaloupe (Cucumis melo) melon cultivation management (v1.0)
 * Cantaloupe planting, irrigation, ripening, harvest, market
 * Features: vine_length_m, fruit_weight_kg, sugar_brix, rind_thick_mm, seed_count, harvest_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,vine_ln,frt_wt,sugar_bx,rind_tk,seed_ct,harv_wk,active;} cant_t;
typedef struct{int n_plant,n_irrig,n_ripe,n_harv,n_mkt,t_vine,t_wt,t_sugar,t_rind,t_seed;} cant_state_t;
static cant_t cantps[N],cantis[N-2],cantrs[N-4],canths[N-6],cantms[N-6]; static cant_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cant_t*a,int*cnt,int*sum,int mx,int lc,int vl,int fw,int sb,int rt,int sc,int hw){if(*cnt>=mx)return -1;cant_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->vine_ln=vl;x->frt_wt=fw;x->sugar_bx=sb;x->rind_tk=rt;x->seed_ct=sc;x->harv_wk=hw;x->active=1;*sum+=vl;(*cnt)++;ps("[CANT] Cantaloupe ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" vl=");pi(vl);ps(" fw=");pi(fw);ps(" sb=");pi(sb);ps(" rt=");pi(rt);ps(" sc=");pi(sc);ps(" hw=");pi(hw);ps("\n");return *cnt-1;}
int cant_init(void){if(init)return -1;st.n_plant=0;st.n_irrig=0;st.n_ripe=0;st.n_harv=0;st.n_mkt=0;st.t_vine=0;st.t_wt=0;st.t_sugar=0;st.t_rind=0;st.t_seed=0;for(int i=0;i<N;i++)cantps[i].active=0;for(int i=0;i<N-2;i++)cantis[i].active=0;for(int i=0;i<N-4;i++)cantrs[i].active=0;for(int i=0;i<N-6;i++)canths[i].active=0;for(int i=0;i<N-6;i++)cantms[i].active=0;init=1;ps("[CANT] Cantaloupe initialized\n");return 0;}
int cant_planting(int lc,int vl,int fw,int sb,int rt,int sc,int hw){return add(cantps,&st.n_plant,&st.t_vine,N,lc,vl,fw,sb,rt,sc,hw);}
int cant_irrigation(int lc,int vl,int fw,int sb,int rt,int sc,int hw){return add(cantis,&st.n_irrig,&st.t_wt,N-2,lc,vl,fw,sb,rt,sc,hw);}
int cant_ripening(int lc,int vl,int fw,int sb,int rt,int sc,int hw){return add(cantrs,&st.n_ripe,&st.t_sugar,N-4,lc,vl,fw,sb,rt,sc,hw);}
int cant_harvest(int lc,int vl,int fw,int sb,int rt,int sc,int hw){return add(canths,&st.n_harv,&st.t_rind,N-6,lc,vl,fw,sb,rt,sc,hw);}
int cant_market(int lc,int vl,int fw,int sb,int rt,int sc,int hw){return add(cantms,&st.n_mkt,&st.t_seed,N-6,lc,vl,fw,sb,rt,sc,hw);}
void cant_report(void){ps("[CANT] Plant: ");pi(st.n_plant);ps(" Vine=");pi(st.t_vine);ps("\nIrrig: ");pi(st.n_irrig);ps(" Wt=");pi(st.t_wt);ps("\nRipe: ");pi(st.n_ripe);ps(" Sugar=");pi(st.t_sugar);ps("\nHarv: ");pi(st.n_harv);ps(" Rind=");pi(st.t_rind);ps("\nMkt: ");pi(st.n_mkt);ps(" Seed=");pi(st.t_seed);ps("\n");}
void cant_state(void){ps("[CANT] Plant=");pi(st.n_plant);ps(" Irrig=");pi(st.n_irrig);ps(" Ripe=");pi(st.n_ripe);ps(" Harv=");pi(st.n_harv);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cantaloupe Admin Demo ===\n\n");cant_init();
/* 1=field 2=greenhouse 3=raised_bed 4=trellis 5=organic */
ps("Cantaloupe planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,vl=2+(i*1),fw=1+(i%4),sb=8+(i%3),rt=3+(i%5),sc=30+(i*10),hw=14+(i%4);cant_planting(lc,vl,fw,sb,rt,sc,hw);}
ps("\nCantaloupe irrigation...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,vl=3+(i*1),fw=2+(i%3),sb=9+(i%2),rt=4+(i%4),sc=35+(i*8),hw=16+(i%3);cant_irrigation(lc,vl,fw,sb,rt,sc,hw);}
ps("\nCantaloupe ripening...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,vl=4+(i*1),fw=3+(i%2),sb=10+(i%2),rt=5+(i%3),sc=40+(i*6),hw=18+(i%2);cant_ripening(lc,vl,fw,sb,rt,sc,hw);}
ps("\nCantaloupe harvest...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,vl=1+(i*2),fw=1+(i%4),sb=7+(i%3),rt=2+(i%5),sc=25+(i*12),hw=12+(i%5);cant_harvest(lc,vl,fw,sb,rt,sc,hw);}
ps("\nCantaloupe market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,vl=5+(i*1),fw=4+(i%2),sb=11+(i%2),rt=6+(i*2),sc=45+(i*5),hw=20+(i%2);cant_market(lc,vl,fw,sb,rt,sc,hw);}
ps("\n");cant_report();cant_state();ps("\n=== Demo Complete ===\n");return 0;}
