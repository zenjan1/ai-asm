/* euonymus_admin: Euonymus management technology administration (v1.0)
 * Euonymus planning, euonymus execution, euonymus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eun_t;
typedef struct{int n_eup,n_eue,n_euv,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eun_state_t;
static eun_t eupps[N],euss[N-2],euvss[N-4],euas[N-6],eumks[N-6]; static eun_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(eun_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eun_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eun_init(void){if(init)return -1;st.n_eup=0;st.n_eue=0;st.n_euv=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eupps[i].active=0;for(int i=0;i<N-2;i++)euss[i].active=0;for(int i=0;i<N-4;i++)euvss[i].active=0;for(int i=0;i<N-6;i++)euas[i].active=0;for(int i=0;i<N-6;i++)eumks[i].active=0;init=1;ps("[EUN] Euonymus initialized\n");return 0;}
int eun_planning(int t,int c,int a,int b,int d,int e,int y){return add(eupps,&st.n_eup,&st.t_f1,N,t,c,a,b,d,e,y);}
int eun_execution(int t,int c,int a,int b,int d,int e,int y){return add(euss,&st.n_eue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eun_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(euvss,&st.n_euv,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eun_accessory(int t,int c,int a,int b,int d,int e,int y){return add(euas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eun_market(int t,int c,int a,int b,int d,int e,int y){return add(eumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eun_report(void){ps("[EUN] Eup: ");pi(st.n_eup);ps(" PCS=");pi(st.t_f1);ps("\nEue: ");pi(st.n_eue);ps(" PCS=");pi(st.t_f2);ps("\nEuv: ");pi(st.n_euv);ps(" PCS=");pi(st.t_f3);ps("\nEuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eun_state(void){ps("[EUN] Eup=");pi(st.n_eup);ps(" Eue=");pi(st.n_eue);ps(" Euv=");pi(st.n_euv);ps(" Euc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Euonymus Admin Demo ===\n\n");eun_init();
ps("Euonymus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eun_planning(t,c,821+(i*17),810+(i*14),790+(i*10),772+(i*6),2020+(i%5));}
ps("\nEuonymus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eun_execution(t,c,810+(i*15),799+(i*12),781+(i*8),768+(i*5),2021+(i%4));}
ps("\nEuonymus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eun_evaluation(t,c,802+(i*13),791+(i*10),777+(i*7),766+(i*4),2022+(i%3));}
ps("\nEuonymus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eun_accessory(t,c,794+(i*11),785+(i*9),771+(i*6),761+(i*3),2023+(i%2));}
ps("\nEuonymus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eun_market(t,c,788+(i*9),779+(i*7),766+(i*5),758+(i*3),2024);}
ps("\n");eun_report();eun_state();ps("\n=== Demo Complete ===\n");return 0;}
