/* uvularia_admin: Uvularia management technology administration (v1.0)
 * Uvularia planning, uvularia execution, uvularia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} uva_t;
typedef struct{int n_uvap,n_uvae,n_uva2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} uva_state_t;
static uva_t uvaps[N],uvass[N-2],uvavss[N-4],uvaas[N-6],uvamks[N-6]; static uva_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(uva_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;uva_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[UVA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int uva_init(void){if(init)return -1;st.n_uvap=0;st.n_uvae=0;st.n_uva2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)uvaps[i].active=0;for(int i=0;i<N-2;i++)uvass[i].active=0;for(int i=0;i<N-4;i++)uvavss[i].active=0;for(int i=0;i<N-6;i++)uvaas[i].active=0;for(int i=0;i<N-6;i++)uvamks[i].active=0;init=1;ps("[UVA] Uvularia initialized\n");return 0;}
int uva_planning(int t,int c,int a,int b,int d,int e,int y){return add(uvaps,&st.n_uvap,&st.t_f1,N,t,c,a,b,d,e,y);}
int uva_execution(int t,int c,int a,int b,int d,int e,int y){return add(uvass,&st.n_uvae,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int uva_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(uvavss,&st.n_uva2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int uva_accessory(int t,int c,int a,int b,int d,int e,int y){return add(uvaas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int uva_market(int t,int c,int a,int b,int d,int e,int y){return add(uvamks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void uva_report(void){ps("[UVA] Uvupp: ");pi(st.n_uvap);ps(" PCS=");pi(st.t_f1);ps("\nUvue: ");pi(st.n_uvae);ps(" PCS=");pi(st.t_f2);ps("\nUvuv: ");pi(st.n_uva2);ps(" PCS=");pi(st.t_f3);ps("\nUvuc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void uva_state(void){ps("[UVA] Uvupp=");pi(st.n_uvap);ps(" Uvue=");pi(st.n_uvae);ps(" Uvuv=");pi(st.n_uva2);ps(" Uvuc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Uvularia Admin Demo ===\n\n");uva_init();
ps("Uvularia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;uva_planning(t,c,1200+(i*17),1189+(i*14),1169+(i*10),1151+(i*6),2020+(i%5));}
ps("\nUvularia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;uva_execution(t,c,1189+(i*15),1178+(i*12),1160+(i*8),1147+(i*5),2021+(i%4));}
ps("\nUvularia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;uva_evaluation(t,c,1181+(i*13),1170+(i*10),1154+(i*7),1143+(i*4),2022+(i%3));}
ps("\nUvularia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;uva_accessory(t,c,1173+(i*11),1164+(i*9),1150+(i*6),1140+(i*3),2023+(i%2));}
ps("\nUvularia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;uva_market(t,c,1167+(i*9),1158+(i*7),1145+(i*5),1137+(i*3),2024);}
ps("\n");uva_report();uva_state();ps("\n=== Demo Complete ===\n");return 0;}
