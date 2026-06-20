/* mold_admin: Mold technology administration (v1.0)
 * Stamping mold, plastic mold, casting mold, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mld_t;
typedef struct{int n_st,n_pl,n_cs,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mld_state_t;
static mld_t sts[N],pls[N-2],css[N-4],acs[N-6],mks[N-6]; static mld_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mld_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mld_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mld_init(void){if(init)return -1;st.n_st=0;st.n_pl=0;st.n_cs=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sts[i].active=0;for(int i=0;i<N-2;i++)pls[i].active=0;for(int i=0;i<N-4;i++)css[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MLD] Mold initialized\n");return 0;}
int mld_stamping(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f1,N,t,c,a,b,d,e,y);}
int mld_plastic(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mld_casting(int t,int c,int a,int b,int d,int e,int y){return add(css,&st.n_cs,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mld_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mld_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mld_report(void){ps("[MLD] St: ");pi(st.n_st);ps(" PCS=");pi(st.t_f1);ps("\nPl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f2);ps("\nCs: ");pi(st.n_cs);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mld_state(void){ps("[MLD] St=");pi(st.n_st);ps(" Pl=");pi(st.n_pl);ps(" Cs=");pi(st.n_cs);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mold Admin Demo ===\n\n");mld_init();
ps("Stamping molds...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mld_stamping(t,c,387+(i*17),372+(i*14),352+(i*10),334+(i*6),2020+(i%5));}
ps("\nPlastic molds...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mld_plastic(t,c,376+(i*15),362+(i*12),344+(i*8),331+(i*5),2021+(i%4));}
ps("\nCasting molds...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mld_casting(t,c,368+(i*13),354+(i*10),338+(i*7),327+(i*4),2022+(i%3));}
ps("\nMold accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mld_accessory(t,c,360+(i*11),348+(i*9),334+(i*6),324+(i*3),2023+(i%2));}
ps("\nMold marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mld_market(t,c,354+(i*9),343+(i*7),330+(i*5),322+(i*3),2024);}
ps("\n");mld_report();mld_state();ps("\n=== Demo Complete ===\n");return 0;}
