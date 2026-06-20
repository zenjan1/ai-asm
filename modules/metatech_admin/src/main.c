/* metatech_admin: Metallurgy technology administration (v1.0)
 * Ferrous metallurgy, nonferrous metallurgy, powder metallurgy, equipment, environmental
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mtl_t;
typedef struct{int n_fe,n_nf,n_pw,n_eq,n_en,t_f1,t_f2,t_f3,t_f4,t_f5;} mtl_state_t;
static mtl_t fes[N],nfs[N-2],pws[N-4],eqs[N-6],ens[N-6]; static mtl_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mtl_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mtl_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MTL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mtl_init(void){if(init)return -1;st.n_fe=0;st.n_nf=0;st.n_pw=0;st.n_eq=0;st.n_en=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fes[i].active=0;for(int i=0;i<N-2;i++)nfs[i].active=0;for(int i=0;i<N-4;i++)pws[i].active=0;for(int i=0;i<N-6;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)ens[i].active=0;init=1;ps("[MTL] Metatech initialized\n");return 0;}
int mtl_ferrous(int t,int c,int a,int b,int d,int e,int y){return add(fes,&st.n_fe,&st.t_f1,N,t,c,a,b,d,e,y);}
int mtl_nonferrous(int t,int c,int a,int b,int d,int e,int y){return add(nfs,&st.n_nf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mtl_powder(int t,int c,int a,int b,int d,int e,int y){return add(pws,&st.n_pw,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mtl_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mtl_env(int t,int c,int a,int b,int d,int e,int y){return add(ens,&st.n_en,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mtl_report(void){ps("[MTL] Fe: ");pi(st.n_fe);ps(" BF=");pi(st.t_f1);ps("\nNF: ");pi(st.n_nf);ps(" Al=");pi(st.t_f2);ps("\nPW: ");pi(st.n_pw);ps(" Sint=");pi(st.t_f3);ps("\nEquip: ");pi(st.n_eq);ps(" Roll=");pi(st.t_f4);ps("\nEnv: ");pi(st.n_en);ps(" DeSOx=");pi(st.t_f5);ps("\n");}
void mtl_state(void){ps("[MTL] Fe=");pi(st.n_fe);ps(" Nf=");pi(st.n_nf);ps(" Pw=");pi(st.n_pw);ps(" Eq=");pi(st.n_eq);ps(" En=");pi(st.n_en);ps("\n");}
int main(void){
ps("=== Metallurgy Tech Admin Demo ===\n\n");mtl_init();
ps("Ferrous metallurgy...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mtl_ferrous(t,c,163+(i*17),148+(i*14),128+(i*10),111+(i*6),2020+(i%5));}
ps("\nNonferrous metallurgy...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mtl_nonferrous(t,c,152+(i*15),138+(i*12),120+(i*8),107+(i*5),2021+(i%4));}
ps("\nPowder metallurgy...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mtl_powder(t,c,144+(i*13),130+(i*10),114+(i*7),103+(i*4),2022+(i%3));}
ps("\nMetallurgy equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtl_equipment(t,c,136+(i*11),124+(i*9),110+(i*6),100+(i*3),2023+(i%2));}
ps("\nMetallurgy environmental...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mtl_env(t,c,130+(i*9),119+(i*7),106+(i*5),98+(i*3),2024);}
ps("\n");mtl_report();mtl_state();ps("\n=== Demo Complete ===\n");return 0;}
