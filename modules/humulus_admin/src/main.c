/* humulus_admin: Humulus management technology administration (v1.0)
 * Humulus planning, humulus execution, humulus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hum_t;
typedef struct{int n_hump,n_hume,n_hum2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hum_state_t;
static hum_t humps[N],humss[N-2],humvss[N-4],humas[N-6],hummks[N-6]; static hum_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hum_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hum_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hum_init(void){if(init)return -1;st.n_hump=0;st.n_hume=0;st.n_hum2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)humps[i].active=0;for(int i=0;i<N-2;i++)humss[i].active=0;for(int i=0;i<N-4;i++)humvss[i].active=0;for(int i=0;i<N-6;i++)humas[i].active=0;for(int i=0;i<N-6;i++)hummks[i].active=0;init=1;ps("[HUM] Humulus initialized\n");return 0;}
int hum_planning(int t,int c,int a,int b,int d,int e,int y){return add(humps,&st.n_hump,&st.t_f1,N,t,c,a,b,d,e,y);}
int hum_execution(int t,int c,int a,int b,int d,int e,int y){return add(humss,&st.n_hume,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hum_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(humvss,&st.n_hum2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hum_accessory(int t,int c,int a,int b,int d,int e,int y){return add(humas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hum_market(int t,int c,int a,int b,int d,int e,int y){return add(hummks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hum_report(void){ps("[HUM] Hump: ");pi(st.n_hump);ps(" PCS=");pi(st.t_f1);ps("\nHume: ");pi(st.n_hume);ps(" PCS=");pi(st.t_f2);ps("\nHumv: ");pi(st.n_hum2);ps(" PCS=");pi(st.t_f3);ps("\nHumc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hum_state(void){ps("[HUM] Hump=");pi(st.n_hump);ps(" Hume=");pi(st.n_hume);ps(" Humv=");pi(st.n_hum2);ps(" Humc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Humulus Admin Demo ===\n\n");hum_init();
ps("Humulus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hum_planning(t,c,1020+(i*17),1009+(i*14),989+(i*10),971+(i*6),2020+(i%5));}
ps("\nHumulus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hum_execution(t,c,1009+(i*15),998+(i*12),980+(i*8),967+(i*5),2021+(i%4));}
ps("\nHumulus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hum_evaluation(t,c,1001+(i*13),990+(i*10),974+(i*7),963+(i*4),2022+(i%3));}
ps("\nHumulus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hum_accessory(t,c,993+(i*11),984+(i*9),970+(i*6),960+(i*3),2023+(i%2));}
ps("\nHumulus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hum_market(t,c,987+(i*9),978+(i*7),965+(i*5),957+(i*3),2024);}
ps("\n");hum_report();hum_state();ps("\n=== Demo Complete ===\n");return 0;}
