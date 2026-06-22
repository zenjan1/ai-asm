/* hutchinsonia_admin: Hutchinsonia management technology administration (v1.0)
 * Hutchinsonia planning, hutchinsonia execution, hutchinsonia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hut_t;
typedef struct{int n_hutp,n_hute,n_hut2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} hut_state_t;
static hut_t hutps[N],hutss[N-2],hutvss[N-4],hutas[N-6],hutmks[N-6]; static hut_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(hut_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hut_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HUT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hut_init(void){if(init)return -1;st.n_hutp=0;st.n_hute=0;st.n_hut2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)hutps[i].active=0;for(int i=0;i<N-2;i++)hutss[i].active=0;for(int i=0;i<N-4;i++)hutvss[i].active=0;for(int i=0;i<N-6;i++)hutas[i].active=0;for(int i=0;i<N-6;i++)hutmks[i].active=0;init=1;ps("[HUT] Hutchinsonia initialized\n");return 0;}
int hut_planning(int t,int c,int a,int b,int d,int e,int y){return add(hutps,&st.n_hutp,&st.t_f1,N,t,c,a,b,d,e,y);}
int hut_execution(int t,int c,int a,int b,int d,int e,int y){return add(hutss,&st.n_hute,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hut_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(hutvss,&st.n_hut2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hut_accessory(int t,int c,int a,int b,int d,int e,int y){return add(hutas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hut_market(int t,int c,int a,int b,int d,int e,int y){return add(hutmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hut_report(void){ps("[HUT] Hutp: ");pi(st.n_hutp);ps(" PCS=");pi(st.t_f1);ps("\nHute: ");pi(st.n_hute);ps(" PCS=");pi(st.t_f2);ps("\nHutv: ");pi(st.n_hut2);ps(" PCS=");pi(st.t_f3);ps("\nHutc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void hut_state(void){ps("[HUT] Hutp=");pi(st.n_hutp);ps(" Hute=");pi(st.n_hute);ps(" Hutv=");pi(st.n_hut2);ps(" Hutc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Hutchinsonia Admin Demo ===\n\n");hut_init();
ps("Hutchinsonia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hut_planning(t,c,1298+(i*17),1287+(i*14),1267+(i*10),1249+(i*6),2020+(i%5));}
ps("\nHutchinsonia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hut_execution(t,c,1287+(i*15),1276+(i*12),1258+(i*8),1245+(i*5),2021+(i%4));}
ps("\nHutchinsonia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hut_evaluation(t,c,1279+(i*13),1268+(i*10),1252+(i*7),1241+(i*4),2022+(i%3));}
ps("\nHutchinsonia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hut_accessory(t,c,1271+(i*11),1262+(i*9),1248+(i*6),1238+(i*3),2023+(i%2));}
ps("\nHutchinsonia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hut_market(t,c,1265+(i*9),1256+(i*7),1243+(i*5),1235+(i*3),2024);}
ps("\n");hut_report();hut_state();ps("\n=== Demo Complete ===\n");return 0;}
