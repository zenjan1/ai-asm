/* abutilon_admin: Abutilon management technology administration (v1.0)
 * Abutilon planning, abutilon execution, abutilon evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} abt_t;
typedef struct{int n_abp,n_abe,n_abv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} abt_state_t;
static abt_t abtps[N],abtss[N-2],abtvss[N-4],abtas[N-6],abtmks[N-6]; static abt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(abt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;abt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ABT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int abt_init(void){if(init)return -1;st.n_abp=0;st.n_abe=0;st.n_abv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)abtps[i].active=0;for(int i=0;i<N-2;i++)abtss[i].active=0;for(int i=0;i<N-4;i++)abtvss[i].active=0;for(int i=0;i<N-6;i++)abtas[i].active=0;for(int i=0;i<N-6;i++)abtmks[i].active=0;init=1;ps("[ABT] Abutilon initialized\n");return 0;}
int abt_planning(int t,int c,int a,int b,int d,int e,int y){return add(abtps,&st.n_abp,&st.t_f1,N,t,c,a,b,d,e,y);}
int abt_execution(int t,int c,int a,int b,int d,int e,int y){return add(abtss,&st.n_abe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int abt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(abtvss,&st.n_abv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int abt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(abtas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int abt_market(int t,int c,int a,int b,int d,int e,int y){return add(abtmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void abt_report(void){ps("[ABT] Abp: ");pi(st.n_abp);ps(" PCS=");pi(st.t_f1);ps("\nAbe: ");pi(st.n_abe);ps(" PCS=");pi(st.t_f2);ps("\nAbv: ");pi(st.n_abv);ps(" PCS=");pi(st.t_f3);ps("\nAbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void abt_state(void){ps("[ABT] Abp=");pi(st.n_abp);ps(" Abe=");pi(st.n_abe);ps(" Abv=");pi(st.n_abv);ps(" Abc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Abutilon Admin Demo ===\n\n");abt_init();
ps("Abutilon planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;abt_planning(t,c,883+(i*17),872+(i*14),852+(i*10),834+(i*6),2020+(i%5));}
ps("\nAbutilon execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;abt_execution(t,c,872+(i*15),861+(i*12),843+(i*8),830+(i*5),2021+(i%4));}
ps("\nAbutilon evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;abt_evaluation(t,c,864+(i*13),853+(i*10),837+(i*7),826+(i*4),2022+(i%3));}
ps("\nAbutilon accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abt_accessory(t,c,856+(i*11),847+(i*9),833+(i*6),823+(i*3),2023+(i%2));}
ps("\nAbutilon marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;abt_market(t,c,850+(i*9),841+(i*7),828+(i*5),820+(i*3),2024);}
ps("\n");abt_report();abt_state();ps("\n=== Demo Complete ===\n");return 0;}
