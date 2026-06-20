/* shoetech_admin: Shoe technology administration (v1.0)
 * Sports shoes, leather shoes, casual shoes, boots, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} shoe_t;
typedef struct{int n_sn,n_lt,n_cl,n_bt,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} shoe_state_t;
static shoe_t sns[N],lts[N-2],cls[N-4],bts[N-6],mks[N-6]; static shoe_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(shoe_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;shoe_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[SHOE] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int shoe_init(void){if(init)return -1;st.n_sn=0;st.n_lt=0;st.n_cl=0;st.n_bt=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sns[i].active=0;for(int i=0;i<N-2;i++)lts[i].active=0;for(int i=0;i<N-4;i++)cls[i].active=0;for(int i=0;i<N-6;i++)bts[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[SHOE] Shoetech initialized\n");return 0;}
int shoe_sports(int t,int c,int a,int b,int d,int e,int y){return add(sns,&st.n_sn,&st.t_f1,N,t,c,a,b,d,e,y);}
int shoe_leather(int t,int c,int a,int b,int d,int e,int y){return add(lts,&st.n_lt,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int shoe_casual(int t,int c,int a,int b,int d,int e,int y){return add(cls,&st.n_cl,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int shoe_boot(int t,int c,int a,int b,int d,int e,int y){return add(bts,&st.n_bt,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int shoe_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void shoe_report(void){ps("[SHOE] Sport: ");pi(st.n_sn);ps(" PCS=");pi(st.t_f1);ps("\nLeather: ");pi(st.n_lt);ps(" PCS=");pi(st.t_f2);ps("\nCasual: ");pi(st.n_cl);ps(" PCS=");pi(st.t_f3);ps("\nBoot: ");pi(st.n_bt);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void shoe_state(void){ps("[SHOE] Sn=");pi(st.n_sn);ps(" Lt=");pi(st.n_lt);ps(" Cl=");pi(st.n_cl);ps(" Bt=");pi(st.n_bt);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Shoe Tech Admin Demo ===\n\n");shoe_init();
ps("Sports shoes...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;shoe_sports(t,c,199+(i*17),184+(i*14),164+(i*10),146+(i*6),2020+(i%5));}
ps("\nLeather shoes...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;shoe_leather(t,c,188+(i*15),174+(i*12),156+(i*8),143+(i*5),2021+(i%4));}
ps("\nCasual shoes...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;shoe_casual(t,c,180+(i*13),166+(i*10),150+(i*7),139+(i*4),2022+(i%3));}
ps("\nBoots...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;shoe_boot(t,c,172+(i*11),160+(i*9),146+(i*6),136+(i*3),2023+(i%2));}
ps("\nShoe marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;shoe_market(t,c,166+(i*9),155+(i*7),142+(i*5),134+(i*3),2024);}
ps("\n");shoe_report();shoe_state();ps("\n=== Demo Complete ===\n");return 0;}
