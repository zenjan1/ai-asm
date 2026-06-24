/* canna_admin: Canna (Canna indica) tropical ornamental plant management (v1.0)
 * Canna planting, division, bloom, overwintering, market
 * Features: plant_height_cm, leaf_span_cm, flower_diameter_cm, rhizome_count, color_variety, bloom_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,plnt_ht,leaf_sp,flr_dia,rhiz_ct,clr_var,bloom_wk,active;} cna_t;
typedef struct{int n_plant,n_div,n_bloom,n_over,n_mkt,t_ht,t_leaf,t_flr,t_rhiz,t_color;} cna_state_t;
static cna_t cnaps[N],cnads[N-2],cnabs[N-4],cnaos[N-6],cnams[N-6]; static cna_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cna_t*a,int*cnt,int*sum,int mx,int lc,int ph,int ls,int fd,int rc,int cv,int bw){if(*cnt>=mx)return -1;cna_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->plnt_ht=ph;x->leaf_sp=ls;x->flr_dia=fd;x->rhiz_ct=rc;x->clr_var=cv;x->bloom_wk=bw;x->active=1;*sum+=ph;(*cnt)++;ps("[CNA] Canna ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" ph=");pi(ph);ps(" ls=");pi(ls);ps(" fd=");pi(fd);ps(" rc=");pi(rc);ps(" cv=");pi(cv);ps(" bw=");pi(bw);ps("\n");return *cnt-1;}
int cna_init(void){if(init)return -1;st.n_plant=0;st.n_div=0;st.n_bloom=0;st.n_over=0;st.n_mkt=0;st.t_ht=0;st.t_leaf=0;st.t_flr=0;st.t_rhiz=0;st.t_color=0;for(int i=0;i<N;i++)cnaps[i].active=0;for(int i=0;i<N-2;i++)cnads[i].active=0;for(int i=0;i<N-4;i++)cnabs[i].active=0;for(int i=0;i<N-6;i++)cnaos[i].active=0;for(int i=0;i<N-6;i++)cnams[i].active=0;init=1;ps("[CNA] Canna initialized\n");return 0;}
int cna_planting(int lc,int ph,int ls,int fd,int rc,int cv,int bw){return add(cnaps,&st.n_plant,&st.t_ht,N,lc,ph,ls,fd,rc,cv,bw);}
int cna_division(int lc,int ph,int ls,int fd,int rc,int cv,int bw){return add(cnads,&st.n_div,&st.t_leaf,N-2,lc,ph,ls,fd,rc,cv,bw);}
int cna_bloom(int lc,int ph,int ls,int fd,int rc,int cv,int bw){return add(cnabs,&st.n_bloom,&st.t_flr,N-4,lc,ph,ls,fd,rc,cv,bw);}
int cna_overwintering(int lc,int ph,int ls,int fd,int rc,int cv,int bw){return add(cnaos,&st.n_over,&st.t_rhiz,N-6,lc,ph,ls,fd,rc,cv,bw);}
int cna_market(int lc,int ph,int ls,int fd,int rc,int cv,int bw){return add(cnams,&st.n_mkt,&st.t_color,N-6,lc,ph,ls,fd,rc,cv,bw);}
void cna_report(void){ps("[CNA] Plant: ");pi(st.n_plant);ps(" Ht=");pi(st.t_ht);ps("\nDiv: ");pi(st.n_div);ps(" Leaf=");pi(st.t_leaf);ps("\nBloom: ");pi(st.n_bloom);ps(" Flr=");pi(st.t_flr);ps("\nOver: ");pi(st.n_over);ps(" Rhiz=");pi(st.t_rhiz);ps("\nMkt: ");pi(st.n_mkt);ps(" Color=");pi(st.t_color);ps("\n");}
void cna_state(void){ps("[CNA] Plant=");pi(st.n_plant);ps(" Div=");pi(st.n_div);ps(" Bloom=");pi(st.n_bloom);ps(" Over=");pi(st.n_over);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Canna Admin Demo ===\n\n");cna_init();
/* 1=tropical_garden 2=container 3=border 4=pond_edge 5=greenhouse */
ps("Canna planting...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,ph=60+(i*10),ls=20+(i*5),fd=5+(i*2),rc=3+(i%5),cv=(i%6)+1,bw=20+(i%4);cna_planting(lc,ph,ls,fd,rc,cv,bw);}
ps("\nCanna division...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,ph=65+(i*8),ls=22+(i*4),fd=6+(i*2),rc=4+(i%4),cv=(i%5)+1,bw=22+(i%3);cna_division(lc,ph,ls,fd,rc,cv,bw);}
ps("\nCanna bloom...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,ph=70+(i*6),ls=25+(i*3),fd=7+(i*2),rc=5+(i%3),cv=(i%4)+1,bw=24+(i%3);cna_bloom(lc,ph,ls,fd,rc,cv,bw);}
ps("\nCanna overwintering...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,ph=50+(i*12),ls=18+(i*6),fd=4+(i%3),rc=2+(i%5),cv=(i%6)+1,bw=18+(i%5);cna_overwintering(lc,ph,ls,fd,rc,cv,bw);}
ps("\nCanna market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,ph=75+(i*5),ls=28+(i*3),fd=8+(i*2),rc=6+(i%2),cv=(i%3)+4,bw=26+(i%2);cna_market(lc,ph,ls,fd,rc,cv,bw);}
ps("\n");cna_report();cna_state();ps("\n=== Demo Complete ===\n");return 0;}
