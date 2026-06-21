/* fragaria_admin: Fragaria management technology administration (v1.0)
 * Fragaria planning, fragaria execution, fragaria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fra_t;
typedef struct{int n_frap,n_frae,n_fra2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fra_state_t;
static fra_t fraps[N],frass[N-2],fravss[N-4],fraas[N-6],framks[N-6]; static fra_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fra_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fra_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FRA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fra_init(void){if(init)return -1;st.n_frap=0;st.n_frae=0;st.n_fra2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)fraps[i].active=0;for(int i=0;i<N-2;i++)frass[i].active=0;for(int i=0;i<N-4;i++)fravss[i].active=0;for(int i=0;i<N-6;i++)fraas[i].active=0;for(int i=0;i<N-6;i++)framks[i].active=0;init=1;ps("[FRA] Fragaria initialized\n");return 0;}
int fra_planning(int t,int c,int a,int b,int d,int e,int y){return add(fraps,&st.n_frap,&st.t_f1,N,t,c,a,b,d,e,y);}
int fra_execution(int t,int c,int a,int b,int d,int e,int y){return add(frass,&st.n_frae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fra_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(fravss,&st.n_fra2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fra_accessory(int t,int c,int a,int b,int d,int e,int y){return add(fraas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fra_market(int t,int c,int a,int b,int d,int e,int y){return add(framks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fra_report(void){ps("[FRA] Frarp: ");pi(st.n_frap);ps(" PCS=");pi(st.t_f1);ps("\nFrae: ");pi(st.n_frae);ps(" PCS=");pi(st.t_f2);ps("\nFrav: ");pi(st.n_fra2);ps(" PCS=");pi(st.t_f3);ps("\nFrac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fra_state(void){ps("[FRA] Frarp=");pi(st.n_frap);ps(" Frae=");pi(st.n_frae);ps(" Frav=");pi(st.n_fra2);ps(" Frac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Fragaria Admin Demo ===\n\n");fra_init();
ps("Fragaria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fra_planning(t,c,1000+(i*17),989+(i*14),969+(i*10),951+(i*6),2020+(i%5));}
ps("\nFragaria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fra_execution(t,c,989+(i*15),978+(i*12),960+(i*8),947+(i*5),2021+(i%4));}
ps("\nFragaria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fra_evaluation(t,c,981+(i*13),970+(i*10),954+(i*7),943+(i*4),2022+(i%3));}
ps("\nFragaria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fra_accessory(t,c,973+(i*11),964+(i*9),950+(i*6),940+(i*3),2023+(i%2));}
ps("\nFragaria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fra_market(t,c,967+(i*9),958+(i*7),945+(i*5),937+(i*3),2024);}
ps("\n");fra_report();fra_state();ps("\n=== Demo Complete ===\n");return 0;}
