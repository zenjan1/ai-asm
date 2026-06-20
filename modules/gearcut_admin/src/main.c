/* gearcut_admin: Gear machining technology administration (v1.0)
 * Hobbing, shaping, shaving and grinding, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} grc_t;
typedef struct{int n_hb,n_sh,n_sg,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} grc_state_t;
static grc_t hbs[N],shs[N-2],sgs[N-4],acs[N-6],mks[N-6]; static grc_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grc_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;grc_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[GRC] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int grc_init(void){if(init)return -1;st.n_hb=0;st.n_sh=0;st.n_sg=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hbs[i].active=0;for(int i=0;i<N-2;i++)shs[i].active=0;for(int i=0;i<N-4;i++)sgs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[GRC] Gearcut initialized\n");return 0;}
int grc_hobbing(int t,int c,int a,int b,int d,int e,int y){return add(hbs,&st.n_hb,&st.t_f1,N,t,c,a,b,d,e,y);}
int grc_shaping(int t,int c,int a,int b,int d,int e,int y){return add(shs,&st.n_sh,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int grc_finish(int t,int c,int a,int b,int d,int e,int y){return add(sgs,&st.n_sg,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int grc_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int grc_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void grc_report(void){ps("[GRC] Hb: ");pi(st.n_hb);ps(" PCS=");pi(st.t_f1);ps("\nSh: ");pi(st.n_sh);ps(" PCS=");pi(st.t_f2);ps("\nSg: ");pi(st.n_sg);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void grc_state(void){ps("[GRC] Hb=");pi(st.n_hb);ps(" Sh=");pi(st.n_sh);ps(" Sg=");pi(st.n_sg);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Gearcut Admin Demo ===\n\n");grc_init();
ps("Hobbing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;grc_hobbing(t,c,407+(i*17),392+(i*14),372+(i*10),354+(i*6),2020+(i%5));}
ps("\nShaping...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;grc_shaping(t,c,396+(i*15),382+(i*12),364+(i*8),351+(i*5),2021+(i%4));}
ps("\nShaving and grinding...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;grc_finish(t,c,388+(i*13),374+(i*10),358+(i*7),347+(i*4),2022+(i%3));}
ps("\nGearcut accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grc_accessory(t,c,380+(i*11),368+(i*9),354+(i*6),344+(i*3),2023+(i%2));}
ps("\nGearcut marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;grc_market(t,c,374+(i*9),363+(i*7),350+(i*5),342+(i*3),2024);}
ps("\n");grc_report();grc_state();ps("\n=== Demo Complete ===\n");return 0;}
