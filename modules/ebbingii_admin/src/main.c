/* ebbingii_admin: Ebbingii management technology administration (v1.0)
 * Ebbingii planning, ebbingii execution, ebbingii evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ebi_t;
typedef struct{int n_ebip,n_ebie,n_ebi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ebi_state_t;
static ebi_t ebips[N],ebiss[N-2],ebivss[N-4],ebias[N-6],ebimks[N-6]; static ebi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(ebi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ebi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[EBI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ebi_init(void){if(init)return -1;st.n_ebip=0;st.n_ebie=0;st.n_ebi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ebips[i].active=0;for(int i=0;i<N-2;i++)ebiss[i].active=0;for(int i=0;i<N-4;i++)ebivss[i].active=0;for(int i=0;i<N-6;i++)ebias[i].active=0;for(int i=0;i<N-6;i++)ebimks[i].active=0;init=1;ps("[EBI] Ebbingii initialized\n");return 0;}
int ebi_planning(int t,int c,int a,int b,int d,int e,int y){return add(ebips,&st.n_ebip,&st.t_f1,N,t,c,a,b,d,e,y);}
int ebi_execution(int t,int c,int a,int b,int d,int e,int y){return add(ebiss,&st.n_ebie,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ebi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(ebivss,&st.n_ebi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ebi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(ebias,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ebi_market(int t,int c,int a,int b,int d,int e,int y){return add(ebimks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ebi_report(void){ps("[EBI] Ebbp: ");pi(st.n_ebip);ps(" PCS=");pi(st.t_f1);ps("\nEbbe: ");pi(st.n_ebie);ps(" PCS=");pi(st.t_f2);ps("\nEbbv: ");pi(st.n_ebi2);ps(" PCS=");pi(st.t_f3);ps("\nEbbc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ebi_state(void){ps("[EBI] Ebbp=");pi(st.n_ebip);ps(" Ebbe=");pi(st.n_ebie);ps(" Ebbv=");pi(st.n_ebi2);ps(" Ebbc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Ebbingii Admin Demo ===\n\n");ebi_init();
ps("Ebbingii planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ebi_planning(t,c,1271+(i*17),1260+(i*14),1240+(i*10),1222+(i*6),2020+(i%5));}
ps("\nEbbingii execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ebi_execution(t,c,1260+(i*15),1249+(i*12),1231+(i*8),1218+(i*5),2021+(i%4));}
ps("\nEbbingii evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ebi_evaluation(t,c,1252+(i*13),1241+(i*10),1225+(i*7),1214+(i*4),2022+(i%3));}
ps("\nEbbingii accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ebi_accessory(t,c,1244+(i*11),1235+(i*9),1221+(i*6),1211+(i*3),2023+(i%2));}
ps("\nEbbingii marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ebi_market(t,c,1238+(i*9),1229+(i*7),1216+(i*5),1208+(i*3),2024);}
ps("\n");ebi_report();ebi_state();ps("\n=== Demo Complete ===\n");return 0;}
