/* euterpe_admin: Euterpe management technology administration (v1.0)
 * Euterpe planning, euterpe execution, euterpe evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} eut_t;
typedef struct{int n_eutp,n_eute,n_eut2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} eut_state_t;
static eut_t eutps[N],eutss[N-2],eutvss[N-4],eutas[N-6],eutmks[N-6]; static eut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(eut_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;eut_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int eut_init(void){if(init)return -1;st.n_eutp=0;st.n_eute=0;st.n_eut2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)eutps[i].active=0;for(int i=0;i<N-2;i++)eutss[i].active=0;for(int i=0;i<N-4;i++)eutvss[i].active=0;for(int i=0;i<N-6;i++)eutas[i].active=0;for(int i=0;i<N-6;i++)eutmks[i].active=0;init=1;ps("[EUT] Euterpe initialized\n");return 0;}
int eut_planning(int t,int c,int a,int b,int d,int e,int y){return add(eutps,&st.n_eutp,&st.t_f1,N,t,c,a,b,d,e,y);}
int eut_execution(int t,int c,int a,int b,int d,int e,int y){return add(eutss,&st.n_eute,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int eut_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(eutvss,&st.n_eut2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int eut_accessory(int t,int c,int a,int b,int d,int e,int y){return add(eutas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int eut_market(int t,int c,int a,int b,int d,int e,int y){return add(eutmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void eut_report(void){ps("[EUT] Eutp: ");pi(st.n_eutp);ps(" PCS=");pi(st.t_f1);ps("\nEute: ");pi(st.n_eute);ps(" PCS=");pi(st.t_f2);ps("\nEutv: ");pi(st.n_eut2);ps(" PCS=");pi(st.t_f3);ps("\nEutc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void eut_state(void){ps("[EUT] Eutp=");pi(st.n_eutp);ps(" Eute=");pi(st.n_eute);ps(" Eutv=");pi(st.n_eut2);ps(" Eutc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Euterpe Admin Demo ===\n\n");eut_init();
ps("Euterpe planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;eut_planning(t,c,1282+(i*17),1271+(i*14),1251+(i*10),1233+(i*6),2020+(i%5));}
ps("\nEuterpe execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;eut_execution(t,c,1271+(i*15),1260+(i*12),1242+(i*8),1229+(i*5),2021+(i%4));}
ps("\nEuterpe evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;eut_evaluation(t,c,1263+(i*13),1252+(i*10),1236+(i*7),1225+(i*4),2022+(i%3));}
ps("\nEuterpe accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eut_accessory(t,c,1255+(i*11),1246+(i*9),1232+(i*6),1222+(i*3),2023+(i%2));}
ps("\nEuterpe marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;eut_market(t,c,1249+(i*9),1240+(i*7),1227+(i*5),1219+(i*3),2024);}
ps("\n");eut_report();eut_state();ps("\n=== Demo Complete ===\n");return 0;}
