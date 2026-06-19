/* marinetech_admin: Marine technology administration (v1.0)
 * Marine monitoring, deep sea exploration, marine resources, marine ecology, marine engineering
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mnt_t;
typedef struct{int n_mo,n_ds,n_re,n_ec,n_en,t_f1,t_f2,t_f3,t_f4,t_f5;} mnt_state_t;
static mnt_t mos[N],dss[N-2],res[N-4],ecs[N-6],ens[N-6]; static mnt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mnt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mnt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MNT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mnt_init(void){if(init)return -1;st.n_mo=0;st.n_ds=0;st.n_re=0;st.n_ec=0;st.n_en=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mos[i].active=0;for(int i=0;i<N-2;i++)dss[i].active=0;for(int i=0;i<N-4;i++)res[i].active=0;for(int i=0;i<N-6;i++)ecs[i].active=0;for(int i=0;i<N-6;i++)ens[i].active=0;init=1;ps("[MNT] Marinetech initialized\n");return 0;}
int mnt_monitor(int t,int c,int a,int b,int d,int e,int y){return add(mos,&st.n_mo,&st.t_f1,N,t,c,a,b,d,e,y);}
int mnt_deepsea(int t,int c,int a,int b,int d,int e,int y){return add(dss,&st.n_ds,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mnt_resource(int t,int c,int a,int b,int d,int e,int y){return add(res,&st.n_re,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mnt_ecology(int t,int c,int a,int b,int d,int e,int y){return add(ecs,&st.n_ec,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mnt_engineer(int t,int c,int a,int b,int d,int e,int y){return add(ens,&st.n_en,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mnt_report(void){ps("[MNT] Mon: ");pi(st.n_mo);ps(" SST=");pi(st.t_f1);ps("\nDeep: ");pi(st.n_ds);ps(" AUV=");pi(st.t_f2);ps("\nRes: ");pi(st.n_re);ps(" Fish=");pi(st.t_f3);ps("\nEco: ");pi(st.n_ec);ps(" Reef=");pi(st.t_f4);ps("\nEng: ");pi(st.n_en);ps(" Port=");pi(st.t_f5);ps("\n");}
void mnt_state(void){ps("[MNT] Mo=");pi(st.n_mo);ps(" Ds=");pi(st.n_ds);ps(" Re=");pi(st.n_re);ps(" Ec=");pi(st.n_ec);ps(" En=");pi(st.n_en);ps("\n");}
int main(void){
ps("=== Marine Tech Admin Demo ===\n\n");mnt_init();
ps("Marine monitoring...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mnt_monitor(t,c,153+(i*17),138+(i*14),118+(i*10),101+(i*6),2020+(i%5));}
ps("\nDeep sea exploration...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mnt_deepsea(t,c,142+(i*15),128+(i*12),110+(i*8),97+(i*5),2021+(i%4));}
ps("\nMarine resources...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mnt_resource(t,c,134+(i*13),120+(i*10),104+(i*7),93+(i*4),2022+(i%3));}
ps("\nMarine ecology...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mnt_ecology(t,c,126+(i*11),114+(i*9),100+(i*6),90+(i*3),2023+(i%2));}
ps("\nMarine engineering...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mnt_engineer(t,c,120+(i*9),109+(i*7),96+(i*5),88+(i*3),2024);}
ps("\n");mnt_report();mnt_state();ps("\n=== Demo Complete ===\n");return 0;}
