/* cineraria_admin: Cineraria management technology administration (v1.0)
 * Cineraria planning, cineraria execution, cineraria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cin_t;
typedef struct{int n_cinp,n_cine,n_cin2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cin_state_t;
static cin_t cinps[N],cinss[N-2],cinvss[N-4],cinas[N-6],cinmks[N-6]; static cin_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cin_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cin_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CIN] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cin_init(void){if(init)return -1;st.n_cinp=0;st.n_cine=0;st.n_cin2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cinps[i].active=0;for(int i=0;i<N-2;i++)cinss[i].active=0;for(int i=0;i<N-4;i++)cinvss[i].active=0;for(int i=0;i<N-6;i++)cinas[i].active=0;for(int i=0;i<N-6;i++)cinmks[i].active=0;init=1;ps("[CIN] Cineraria initialized\n");return 0;}
int cin_planning(int t,int c,int a,int b,int d,int e,int y){return add(cinps,&st.n_cinp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cin_execution(int t,int c,int a,int b,int d,int e,int y){return add(cinss,&st.n_cine,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cin_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cinvss,&st.n_cin2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cin_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cinas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cin_market(int t,int c,int a,int b,int d,int e,int y){return add(cinmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cin_report(void){ps("[CIN] Cinnp: ");pi(st.n_cinp);ps(" PCS=");pi(st.t_f1);ps("\nCinne: ");pi(st.n_cine);ps(" PCS=");pi(st.t_f2);ps("\nCinv: ");pi(st.n_cin2);ps(" PCS=");pi(st.t_f3);ps("\nCinc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cin_state(void){ps("[CIN] Cinnp=");pi(st.n_cinp);ps(" Cinne=");pi(st.n_cine);ps(" Cinv=");pi(st.n_cin2);ps(" Cinc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cineraria Admin Demo ===\n\n");cin_init();
ps("Cineraria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cin_planning(t,c,985+(i*17),974+(i*14),954+(i*10),936+(i*6),2020+(i%5));}
ps("\nCineraria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cin_execution(t,c,974+(i*15),963+(i*12),945+(i*8),932+(i*5),2021+(i%4));}
ps("\nCineraria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cin_evaluation(t,c,966+(i*13),955+(i*10),939+(i*7),928+(i*4),2022+(i%3));}
ps("\nCineraria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cin_accessory(t,c,958+(i*11),949+(i*9),935+(i*6),925+(i*3),2023+(i%2));}
ps("\nCineraria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cin_market(t,c,952+(i*9),943+(i*7),930+(i*5),922+(i*3),2024);}
ps("\n");cin_report();cin_state();ps("\n=== Demo Complete ===\n");return 0;}
