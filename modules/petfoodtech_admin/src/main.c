/* petfoodtech_admin: Pet food technology administration (v1.0)
 * Dog food, cat food, pet snacks, pet supplements, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ptf_t;
typedef struct{int n_dg,n_ct,n_sn,n_pb,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ptf_state_t;
static ptf_t dgs[N],cts[N-2],sns[N-4],pbs[N-6],mks[N-6]; static ptf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ptf_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ptf_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[PTF] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ptf_init(void){if(init)return -1;st.n_dg=0;st.n_ct=0;st.n_sn=0;st.n_pb=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)dgs[i].active=0;for(int i=0;i<N-2;i++)cts[i].active=0;for(int i=0;i<N-4;i++)sns[i].active=0;for(int i=0;i<N-6;i++)pbs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[PTF] Petfoodtech initialized\n");return 0;}
int ptf_dog(int t,int c,int a,int b,int d,int e,int y){return add(dgs,&st.n_dg,&st.t_f1,N,t,c,a,b,d,e,y);}
int ptf_cat(int t,int c,int a,int b,int d,int e,int y){return add(cts,&st.n_ct,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ptf_snack(int t,int c,int a,int b,int d,int e,int y){return add(sns,&st.n_sn,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ptf_supplement(int t,int c,int a,int b,int d,int e,int y){return add(pbs,&st.n_pb,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ptf_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ptf_report(void){ps("[PTF] Dog: ");pi(st.n_dg);ps(" kg=");pi(st.t_f1);ps("\nCat: ");pi(st.n_ct);ps(" kg=");pi(st.t_f2);ps("\nSnack: ");pi(st.n_sn);ps(" kg=");pi(st.t_f3);ps("\nSupp: ");pi(st.n_pb);ps(" SKU=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ptf_state(void){ps("[PTF] Dg=");pi(st.n_dg);ps(" Ct=");pi(st.n_ct);ps(" Sn=");pi(st.n_sn);ps(" Pb=");pi(st.n_pb);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Pet Food Tech Admin Demo ===\n\n");ptf_init();
ps("Dog food...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ptf_dog(t,c,193+(i*17),178+(i*14),158+(i*10),140+(i*6),2020+(i%5));}
ps("\nCat food...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ptf_cat(t,c,182+(i*15),168+(i*12),150+(i*8),137+(i*5),2021+(i%4));}
ps("\nPet snacks...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ptf_snack(t,c,174+(i*13),160+(i*10),144+(i*7),133+(i*4),2022+(i%3));}
ps("\nPet supplements...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ptf_supplement(t,c,166+(i*11),154+(i*9),140+(i*6),130+(i*3),2023+(i%2));}
ps("\nPet food marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ptf_market(t,c,160+(i*9),149+(i*7),136+(i*5),128+(i*3),2024);}
ps("\n");ptf_report();ptf_state();ps("\n=== Demo Complete ===\n");return 0;}
