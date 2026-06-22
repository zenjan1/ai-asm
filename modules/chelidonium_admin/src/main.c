/* chelidonium_admin: Chelidonium management technology administration (v1.0)
 * Chelidonium planning, chelidonium execution, chelidonium evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lid_t;
typedef struct{int n_lidp,n_lide,n_lid2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lid_state_t;
static lid_t lidps[N],lidss[N-2],lidvss[N-4],lidas[N-6],lidmks[N-6]; static lid_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(lid_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lid_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LID] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lid_init(void){if(init)return -1;st.n_lidp=0;st.n_lide=0;st.n_lid2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lidps[i].active=0;for(int i=0;i<N-2;i++)lidss[i].active=0;for(int i=0;i<N-4;i++)lidvss[i].active=0;for(int i=0;i<N-6;i++)lidas[i].active=0;for(int i=0;i<N-6;i++)lidmks[i].active=0;init=1;ps("[LID] Chelidonium initialized\n");return 0;}
int lid_planning(int t,int c,int a,int b,int d,int e,int y){return add(lidps,&st.n_lidp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lid_execution(int t,int c,int a,int b,int d,int e,int y){return add(lidss,&st.n_lide,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lid_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lidvss,&st.n_lid2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lid_accessory(int t,int c,int a,int b,int d,int e,int y){return add(lidas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lid_market(int t,int c,int a,int b,int d,int e,int y){return add(lidmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lid_report(void){ps("[LID] Chepp: ");pi(st.n_lidp);ps(" PCS=");pi(st.t_f1);ps("\nChepe: ");pi(st.n_lide);ps(" PCS=");pi(st.t_f2);ps("\nChev: ");pi(st.n_lid2);ps(" PCS=");pi(st.t_f3);ps("\nChec: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lid_state(void){ps("[LID] Chepp=");pi(st.n_lidp);ps(" Chepe=");pi(st.n_lide);ps(" Chev=");pi(st.n_lid2);ps(" Chec=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Chelidonium Admin Demo ===\n\n");lid_init();
ps("Chelidonium planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lid_planning(t,c,1233+(i*17),1222+(i*14),1202+(i*10),1184+(i*6),2020+(i%5));}
ps("\nChelidonium execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lid_execution(t,c,1222+(i*15),1211+(i*12),1193+(i*8),1180+(i*5),2021+(i%4));}
ps("\nChelidonium evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lid_evaluation(t,c,1214+(i*13),1203+(i*10),1187+(i*7),1176+(i*4),2022+(i%3));}
ps("\nChelidonium accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lid_accessory(t,c,1206+(i*11),1197+(i*9),1183+(i*6),1173+(i*3),2023+(i%2));}
ps("\nChelidonium marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lid_market(t,c,1200+(i*9),1191+(i*7),1178+(i*5),1170+(i*3),2024);}
ps("\n");lid_report();lid_state();ps("\n=== Demo Complete ===\n");return 0;}
