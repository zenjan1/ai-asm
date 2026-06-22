/* myosotis_admin: Myosotis management technology administration (v1.0)
 * Myosotis planning, myosotis execution, myosotis evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} myo_t;
typedef struct{int n_myop,n_myoe,n_myo2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} myo_state_t;
static myo_t myops[N],myoss[N-2],myovss[N-4],myoas[N-6],myomks[N-6]; static myo_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(myo_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;myo_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MYO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int myo_init(void){if(init)return -1;st.n_myop=0;st.n_myoe=0;st.n_myo2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)myops[i].active=0;for(int i=0;i<N-2;i++)myoss[i].active=0;for(int i=0;i<N-4;i++)myovss[i].active=0;for(int i=0;i<N-6;i++)myoas[i].active=0;for(int i=0;i<N-6;i++)myomks[i].active=0;init=1;ps("[MYO] Myosotis initialized\n");return 0;}
int myo_planning(int t,int c,int a,int b,int d,int e,int y){return add(myops,&st.n_myop,&st.t_f1,N,t,c,a,b,d,e,y);}
int myo_execution(int t,int c,int a,int b,int d,int e,int y){return add(myoss,&st.n_myoe,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int myo_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(myovss,&st.n_myo2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int myo_accessory(int t,int c,int a,int b,int d,int e,int y){return add(myoas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int myo_market(int t,int c,int a,int b,int d,int e,int y){return add(myomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void myo_report(void){ps("[MYO] Myopp: ");pi(st.n_myop);ps(" PCS=");pi(st.t_f1);ps("\nMyoe: ");pi(st.n_myoe);ps(" PCS=");pi(st.t_f2);ps("\nMyov: ");pi(st.n_myo2);ps(" PCS=");pi(st.t_f3);ps("\nMyoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void myo_state(void){ps("[MYO] Myopp=");pi(st.n_myop);ps(" Myoe=");pi(st.n_myoe);ps(" Myov=");pi(st.n_myo2);ps(" Myoc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Myosotis Admin Demo ===\n\n");myo_init();
ps("Myosotis planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;myo_planning(t,c,1161+(i*17),1150+(i*14),1130+(i*10),1112+(i*6),2020+(i%5));}
ps("\nMyosotis execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;myo_execution(t,c,1150+(i*15),1139+(i*12),1121+(i*8),1108+(i*5),2021+(i%4));}
ps("\nMyosotis evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;myo_evaluation(t,c,1142+(i*13),1131+(i*10),1115+(i*7),1104+(i*4),2022+(i%3));}
ps("\nMyosotis accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;myo_accessory(t,c,1134+(i*11),1125+(i*9),1111+(i*6),1101+(i*3),2023+(i%2));}
ps("\nMyosotis marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;myo_market(t,c,1128+(i*9),1119+(i*7),1106+(i*5),1098+(i*3),2024);}
ps("\n");myo_report();myo_state();ps("\n=== Demo Complete ===\n");return 0;}
