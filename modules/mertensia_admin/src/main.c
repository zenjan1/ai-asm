/* mertensia_admin: Mertensia management technology administration (v1.0)
 * Mertensia planning, mertensia execution, mertensia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mer_t;
typedef struct{int n_merp,n_mere,n_mer2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mer_state_t;
static mer_t merps[N],merss[N-2],mervss[N-4],meras[N-6],mermks[N-6]; static mer_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mer_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mer_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mer_init(void){if(init)return -1;st.n_merp=0;st.n_mere=0;st.n_mer2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)merps[i].active=0;for(int i=0;i<N-2;i++)merss[i].active=0;for(int i=0;i<N-4;i++)mervss[i].active=0;for(int i=0;i<N-6;i++)meras[i].active=0;for(int i=0;i<N-6;i++)mermks[i].active=0;init=1;ps("[MER] Mertensia initialized\n");return 0;}
int mer_planning(int t,int c,int a,int b,int d,int e,int y){return add(merps,&st.n_merp,&st.t_f1,N,t,c,a,b,d,e,y);}
int mer_execution(int t,int c,int a,int b,int d,int e,int y){return add(merss,&st.n_mere,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mer_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(mervss,&st.n_mer2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mer_accessory(int t,int c,int a,int b,int d,int e,int y){return add(meras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mer_market(int t,int c,int a,int b,int d,int e,int y){return add(mermks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mer_report(void){ps("[MER] Merpp: ");pi(st.n_merp);ps(" PCS=");pi(st.t_f1);ps("\nMere: ");pi(st.n_mere);ps(" PCS=");pi(st.t_f2);ps("\nMerv: ");pi(st.n_mer2);ps(" PCS=");pi(st.t_f3);ps("\nMerc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mer_state(void){ps("[MER] Merpp=");pi(st.n_merp);ps(" Mere=");pi(st.n_mere);ps(" Merv=");pi(st.n_mer2);ps(" Merc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mertensia Admin Demo ===\n\n");mer_init();
ps("Mertensia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mer_planning(t,c,1159+(i*17),1148+(i*14),1128+(i*10),1110+(i*6),2020+(i%5));}
ps("\nMertensia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mer_execution(t,c,1148+(i*15),1137+(i*12),1119+(i*8),1106+(i*5),2021+(i%4));}
ps("\nMertensia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mer_evaluation(t,c,1140+(i*13),1129+(i*10),1113+(i*7),1102+(i*4),2022+(i%3));}
ps("\nMertensia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mer_accessory(t,c,1132+(i*11),1123+(i*9),1109+(i*6),1099+(i*3),2023+(i%2));}
ps("\nMertensia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mer_market(t,c,1126+(i*9),1117+(i*7),1104+(i*5),1096+(i*3),2024);}
ps("\n");mer_report();mer_state();ps("\n=== Demo Complete ===\n");return 0;}
