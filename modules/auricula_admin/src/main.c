/* auricula_admin: Auricula management technology administration (v1.0)
 * Auricula planning, auricula execution, auricula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aur_t;
typedef struct{int n_aurp,n_aure,n_aur2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aur_state_t;
static aur_t aurps[N],aurss[N-2],aurvss[N-4],auras[N-6],aurmks[N-6]; static aur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aur_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aur_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AUR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aur_init(void){if(init)return -1;st.n_aurp=0;st.n_aure=0;st.n_aur2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)aurps[i].active=0;for(int i=0;i<N-2;i++)aurss[i].active=0;for(int i=0;i<N-4;i++)aurvss[i].active=0;for(int i=0;i<N-6;i++)auras[i].active=0;for(int i=0;i<N-6;i++)aurmks[i].active=0;init=1;ps("[AUR] Auricula initialized\n");return 0;}
int aur_planning(int t,int c,int a,int b,int d,int e,int y){return add(aurps,&st.n_aurp,&st.t_f1,N,t,c,a,b,d,e,y);}
int aur_execution(int t,int c,int a,int b,int d,int e,int y){return add(aurss,&st.n_aure,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aur_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(aurvss,&st.n_aur2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aur_accessory(int t,int c,int a,int b,int d,int e,int y){return add(auras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aur_market(int t,int c,int a,int b,int d,int e,int y){return add(aurmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aur_report(void){ps("[AUR] Aurp: ");pi(st.n_aurp);ps(" PCS=");pi(st.t_f1);ps("\nAure: ");pi(st.n_aure);ps(" PCS=");pi(st.t_f2);ps("\nAurv: ");pi(st.n_aur2);ps(" PCS=");pi(st.t_f3);ps("\nAurc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aur_state(void){ps("[AUR] Aurp=");pi(st.n_aurp);ps(" Aure=");pi(st.n_aure);ps(" Aurv=");pi(st.n_aur2);ps(" Aurc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Auricula Admin Demo ===\n\n");aur_init();
ps("Auricula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aur_planning(t,c,977+(i*17),966+(i*14),946+(i*10),928+(i*6),2020+(i%5));}
ps("\nAuricula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aur_execution(t,c,966+(i*15),955+(i*12),937+(i*8),924+(i*5),2021+(i%4));}
ps("\nAuricula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aur_evaluation(t,c,958+(i*13),947+(i*10),931+(i*7),920+(i*4),2022+(i%3));}
ps("\nAuricula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aur_accessory(t,c,950+(i*11),941+(i*9),927+(i*6),917+(i*3),2023+(i%2));}
ps("\nAuricula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aur_market(t,c,944+(i*9),935+(i*7),922+(i*5),914+(i*3),2024);}
ps("\n");aur_report();aur_state();ps("\n=== Demo Complete ===\n");return 0;}
