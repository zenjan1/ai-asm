/* nandina_admin: Nandina heavenly bamboo management (v1.0)
 * Nandina domestica cultivation, ornamental berries, evergreen foliage
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} nan_t;
typedef struct{int n_nanc,n_nanb,n_nanf,n_nanw,n_nand,t_f1,t_f2,t_f3,t_f4,t_f5;} nan_state_t;
static nan_t nancs[N],nanbs[N-2],nanfs[N-4],nanws[N-6],nands[N-6]; static nan_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(nan_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;nan_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NAN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int nan_init(void){if(init)return -1;st.n_nanc=0;st.n_nanb=0;st.n_nanf=0;st.n_nanw=0;st.n_nand=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nancs[i].active=0;for(int i=0;i<N-2;i++)nanbs[i].active=0;for(int i=0;i<N-4;i++)nanfs[i].active=0;for(int i=0;i<N-6;i++)nanws[i].active=0;for(int i=0;i<N-6;i++)nands[i].active=0;init=1;ps("[NAN] Nandina initialized\n");return 0;}
int nan_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(nancs,&st.n_nanc,&st.t_f1,N,t,c,a,b,d,e,y);}
int nan_berry(int t,int c,int a,int b,int d,int e,int y){return add(nanbs,&st.n_nanb,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int nan_foliage(int t,int c,int a,int b,int d,int e,int y){return add(nanfs,&st.n_nanf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int nan_wildlife(int t,int c,int a,int b,int d,int e,int y){return add(nanws,&st.n_nanw,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int nan_drought(int t,int c,int a,int b,int d,int e,int y){return add(nands,&st.n_nand,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void nan_report(void){ps("[NAN] Cultivation: ");pi(st.n_nanc);ps(" PCS=");pi(st.t_f1);ps("\nBerry: ");pi(st.n_nanb);ps(" PCS=");pi(st.t_f2);ps("\nFoliage: ");pi(st.n_nanf);ps(" PCS=");pi(st.t_f3);ps("\nWildlife: ");pi(st.n_nanw);ps(" PCS=");pi(st.t_f4);ps("\nDrought: ");pi(st.n_nand);ps(" USD=");pi(st.t_f5);ps("\n");}
void nan_state(void){ps("[NAN] Cul=");pi(st.n_nanc);ps(" Ber=");pi(st.n_nanb);ps(" Fol=");pi(st.n_nanf);ps(" Wil=");pi(st.n_nanw);ps(" Drg=");pi(st.n_nand);ps("\n");}
int main(void){
ps("=== Nandina Admin Demo ===\n\n");nan_init();
ps("Nandina cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;nan_cultivation(t,c,961+(i*17),950+(i*14),930+(i*10),912+(i*6),2020+(i%5));}
ps("\nBerry production...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;nan_berry(t,c,950+(i*15),939+(i*12),921+(i*8),908+(i*5),2021+(i%4));}
ps("\nFoliage management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;nan_foliage(t,c,942+(i*13),931+(i*10),915+(i*7),904+(i*4),2022+(i%3));}
ps("\nWildlife habitat...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nan_wildlife(t,c,934+(i*11),925+(i*9),911+(i*6),901+(i*3),2023+(i%2));}
ps("\nDrought tolerance...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;nan_drought(t,c,928+(i*9),919+(i*7),906+(i*5),898+(i*3),2024);}
ps("\n");nan_report();nan_state();ps("\n=== Demo Complete ===\n");return 0;}
