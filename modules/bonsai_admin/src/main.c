/* bonsai_admin: Bonsai miniature tree cultivation management (v1.0)
 * Bonsai cultivation, styling, watering, exhibition, market
 * Features: age_years, height_cm, trunk_mm, leaf_scale, style_type, pot_size
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,age_yr,ht_cm,trunk_mm,leaf_sc,style,pot_sz,active;} bons_t;
typedef struct{int n_cult,n_style,n_water,n_exhibit,n_mkt,t_age,t_ht,t_trunk,t_leaf,t_style;} bons_state_t;
static bons_t bonsps[N],bonsst[N-2],bonsws[N-4],bonsex[N-6],bonsms[N-6]; static bons_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(bons_t*a,int*cnt,int*sum,int mx,int lc,int ay,int hc,int tm,int ls,int sy,int pz){if(*cnt>=mx)return -1;bons_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->age_yr=ay;x->ht_cm=hc;x->trunk_mm=tm;x->leaf_sc=ls;x->style=sy;x->pot_sz=pz;x->active=1;*sum+=ay;(*cnt)++;ps("[BONS] Bonsai ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ay=");pi(ay);ps(" hc=");pi(hc);ps(" tm=");pi(tm);ps(" ls=");pi(ls);ps(" sy=");pi(sy);ps(" pz=");pi(pz);ps("\n");return *cnt-1;}
int bons_init(void){if(init)return -1;st.n_cult=0;st.n_style=0;st.n_water=0;st.n_exhibit=0;st.n_mkt=0;st.t_age=0;st.t_ht=0;st.t_trunk=0;st.t_leaf=0;st.t_style=0;for(int i=0;i<N;i++)bonsps[i].active=0;for(int i=0;i<N-2;i++)bonsst[i].active=0;for(int i=0;i<N-4;i++)bonsws[i].active=0;for(int i=0;i<N-6;i++)bonsex[i].active=0;for(int i=0;i<N-6;i++)bonsms[i].active=0;init=1;ps("[BONS] Bonsai initialized\n");return 0;}
int bons_cultivation(int lc,int ay,int hc,int tm,int ls,int sy,int pz){return add(bonsps,&st.n_cult,&st.t_age,N,lc,ay,hc,tm,ls,sy,pz);}
int bons_styling(int lc,int ay,int hc,int tm,int ls,int sy,int pz){return add(bonsst,&st.n_style,&st.t_ht,N-2,lc,ay,hc,tm,ls,sy,pz);}
int bons_watering(int lc,int ay,int hc,int tm,int ls,int sy,int pz){return add(bonsws,&st.n_water,&st.t_trunk,N-4,lc,ay,hc,tm,ls,sy,pz);}
int bons_exhibition(int lc,int ay,int hc,int tm,int ls,int sy,int pz){return add(bonsex,&st.n_exhibit,&st.t_leaf,N-6,lc,ay,hc,tm,ls,sy,pz);}
int bons_market(int lc,int ay,int hc,int tm,int ls,int sy,int pz){return add(bonsms,&st.n_mkt,&st.t_style,N-6,lc,ay,hc,tm,ls,sy,pz);}
void bons_report(void){ps("[BONS] Cult: ");pi(st.n_cult);ps(" Age=");pi(st.t_age);ps("\nStyle: ");pi(st.n_style);ps(" Ht=");pi(st.t_ht);ps("\nWater: ");pi(st.n_water);ps(" Trunk=");pi(st.t_trunk);ps("\nExhibit: ");pi(st.n_exhibit);ps(" Leaf=");pi(st.t_leaf);ps("\nMkt: ");pi(st.n_mkt);ps(" Style=");pi(st.t_style);ps("\n");}
void bons_state(void){ps("[BONS] Cult=");pi(st.n_cult);ps(" Style=");pi(st.n_style);ps(" Water=");pi(st.n_water);ps(" Exhibit=");pi(st.n_exhibit);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Bonsai Admin Demo ===\n\n");bons_init();
/* 1=indoor 2=greenhouse 3=garden 4=showroom 5=nursery */
ps("Bonsai cultivation...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ay=5+(i*3),hc=15+(i*5),tm=10+(i*3),ls=1+(i%4),sy=(i%5)+1,pz=10+(i*2);bons_cultivation(lc,ay,hc,tm,ls,sy,pz);}
ps("\nBonsai styling...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ay=8+(i*2),hc=20+(i*4),tm=15+(i*2),ls=2+(i%3),sy=(i%5)+1,pz=12+(i*2);bons_styling(lc,ay,hc,tm,ls,sy,pz);}
ps("\nBonsai watering...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ay=10+(i*2),hc=25+(i*3),tm=20+(i*2),ls=3+(i%3),sy=(i%4)+2,pz=14+(i*2);bons_watering(lc,ay,hc,tm,ls,sy,pz);}
ps("\nBonsai exhibition...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ay=15+(i*2),hc=30+(i*2),tm=25+(i*2),ls=4+(i%2),sy=(i%5)+1,pz=16+(i*2);bons_exhibition(lc,ay,hc,tm,ls,sy,pz);}
ps("\nBonsai market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ay=20+(i*3),hc=35+(i*3),tm=30+(i*2),ls=5+(i%2),sy=(i%3)+3,pz=18+(i*2);bons_market(lc,ay,hc,tm,ls,sy,pz);}
ps("\n");bons_report();bons_state();ps("\n=== Demo Complete ===\n");return 0;}
