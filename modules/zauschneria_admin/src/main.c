/* zauschneria_admin: Zauschneria management technology administration (v1.0)
 * Zauschneria planning, zauschneria execution, zauschneria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} zau_t;
typedef struct{int n_zaup,n_zaue,n_zau2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} zau_state_t;
static zau_t zaups[N],zauss[N-2],zauvss[N-4],zauas[N-6],zaumks[N-6]; static zau_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(zau_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;zau_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ZAU] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int zau_init(void){if(init)return -1;st.n_zaup=0;st.n_zaue=0;st.n_zau2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)zaups[i].active=0;for(int i=0;i<N-2;i++)zauss[i].active=0;for(int i=0;i<N-4;i++)zauvss[i].active=0;for(int i=0;i<N-6;i++)zauas[i].active=0;for(int i=0;i<N-6;i++)zaumks[i].active=0;init=1;ps("[ZAU] Zauschneria initialized\n");return 0;}
int zau_planning(int t,int c,int a,int b,int d,int e,int y){return add(zaups,&st.n_zaup,&st.t_f1,N,t,c,a,b,d,e,y);}
int zau_execution(int t,int c,int a,int b,int d,int e,int y){return add(zauss,&st.n_zaue,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int zau_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(zauvss,&st.n_zau2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int zau_accessory(int t,int c,int a,int b,int d,int e,int y){return add(zauas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int zau_market(int t,int c,int a,int b,int d,int e,int y){return add(zaumks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void zau_report(void){ps("[ZAU] Zaupp: ");pi(st.n_zaup);ps(" PCS=");pi(st.t_f1);ps("\nZaue: ");pi(st.n_zaue);ps(" PCS=");pi(st.t_f2);ps("\nZauv: ");pi(st.n_zau2);ps(" PCS=");pi(st.t_f3);ps("\nZauc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void zau_state(void){ps("[ZAU] Zaupp=");pi(st.n_zaup);ps(" Zaue=");pi(st.n_zaue);ps(" Zauv=");pi(st.n_zau2);ps(" Zauc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Zauschneria Admin Demo ===\n\n");zau_init();
ps("Zauschneria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;zau_planning(t,c,1113+(i*17),1102+(i*14),1082+(i*10),1064+(i*6),2020+(i%5));}
ps("\nZauschneria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;zau_execution(t,c,1102+(i*15),1091+(i*12),1073+(i*8),1060+(i*5),2021+(i%4));}
ps("\nZauschneria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;zau_evaluation(t,c,1094+(i*13),1083+(i*10),1067+(i*7),1056+(i*4),2022+(i%3));}
ps("\nZauschneria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zau_accessory(t,c,1086+(i*11),1077+(i*9),1063+(i*6),1053+(i*3),2023+(i%2));}
ps("\nZauschneria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;zau_market(t,c,1080+(i*9),1071+(i*7),1058+(i*5),1050+(i*3),2024);}
ps("\n");zau_report();zau_state();ps("\n=== Demo Complete ===\n");return 0;}
