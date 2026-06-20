/* toytech_admin: Toy technology administration (v1.0)
 * Plastic toys, plush toys, educational toys, electronic toys, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} toy_t;
typedef struct{int n_pl,n_plu,n_edu,n_ele,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} toy_state_t;
static toy_t pls[N],plus[N-2],edus[N-4],eles[N-6],mks[N-6]; static toy_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(toy_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;toy_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TOY] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int toy_init(void){if(init)return -1;st.n_pl=0;st.n_plu=0;st.n_edu=0;st.n_ele=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)plus[i].active=0;for(int i=0;i<N-4;i++)edus[i].active=0;for(int i=0;i<N-6;i++)eles[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TOY] Toytech initialized\n");return 0;}
int toy_plastic(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int toy_plush(int t,int c,int a,int b,int d,int e,int y){return add(plus,&st.n_plu,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int toy_educational(int t,int c,int a,int b,int d,int e,int y){return add(edus,&st.n_edu,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int toy_electronic(int t,int c,int a,int b,int d,int e,int y){return add(eles,&st.n_ele,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int toy_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void toy_report(void){ps("[TOY] Plastic: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nPlush: ");pi(st.n_plu);ps(" PCS=");pi(st.t_f2);ps("\nEdu: ");pi(st.n_edu);ps(" PCS=");pi(st.t_f3);ps("\nElec: ");pi(st.n_ele);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void toy_state(void){ps("[TOY] Pl=");pi(st.n_pl);ps(" Plu=");pi(st.n_plu);ps(" Edu=");pi(st.n_edu);ps(" Ele=");pi(st.n_ele);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Toy Tech Admin Demo ===\n\n");toy_init();
ps("Plastic toys...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;toy_plastic(t,c,196+(i*17),181+(i*14),161+(i*10),143+(i*6),2020+(i%5));}
ps("\nPlush toys...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;toy_plush(t,c,185+(i*15),171+(i*12),153+(i*8),140+(i*5),2021+(i%4));}
ps("\nEducational toys...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;toy_educational(t,c,177+(i*13),163+(i*10),147+(i*7),136+(i*4),2022+(i%3));}
ps("\nElectronic toys...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;toy_electronic(t,c,169+(i*11),157+(i*9),143+(i*6),133+(i*3),2023+(i%2));}
ps("\nToy marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;toy_market(t,c,163+(i*9),152+(i*7),139+(i*5),131+(i*3),2024);}
ps("\n");toy_report();toy_state();ps("\n=== Demo Complete ===\n");return 0;}
