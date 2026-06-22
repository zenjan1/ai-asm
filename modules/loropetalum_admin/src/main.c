/* loropetalum_admin: Loropetalum management technology administration (v1.0)
 * Loropetalum planning, loropetalum execution, loropetalum evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} lor_t;
typedef struct{int n_lorp,n_lore,n_lor2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} lor_state_t;
static lor_t lorps[N],lorss[N-2],lorvss[N-4],loras[N-6],lormks[N-6]; static lor_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lor_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;lor_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[LOR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int lor_init(void){if(init)return -1;st.n_lorp=0;st.n_lore=0;st.n_lor2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lorps[i].active=0;for(int i=0;i<N-2;i++)lorss[i].active=0;for(int i=0;i<N-4;i++)lorvss[i].active=0;for(int i=0;i<N-6;i++)loras[i].active=0;for(int i=0;i<N-6;i++)lormks[i].active=0;init=1;ps("[LOR] Loropetalum initialized\n");return 0;}
int lor_planning(int t,int c,int a,int b,int d,int e,int y){return add(lorps,&st.n_lorp,&st.t_f1,N,t,c,a,b,d,e,y);}
int lor_execution(int t,int c,int a,int b,int d,int e,int y){return add(lorss,&st.n_lore,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int lor_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(lorvss,&st.n_lor2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int lor_accessory(int t,int c,int a,int b,int d,int e,int y){return add(loras,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int lor_market(int t,int c,int a,int b,int d,int e,int y){return add(lormks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void lor_report(void){ps("[LOR] Lorpp: ");pi(st.n_lorp);ps(" PCS=");pi(st.t_f1);ps("\nLore: ");pi(st.n_lore);ps(" PCS=");pi(st.t_f2);ps("\nLorv: ");pi(st.n_lor2);ps(" PCS=");pi(st.t_f3);ps("\nLorc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void lor_state(void){ps("[LOR] Lorpp=");pi(st.n_lorp);ps(" Lore=");pi(st.n_lore);ps(" Lorv=");pi(st.n_lor2);ps(" Lorc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Loropetalum Admin Demo ===\n\n");lor_init();
ps("Loropetalum planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;lor_planning(t,c,1156+(i*17),1145+(i*14),1125+(i*10),1107+(i*6),2020+(i%5));}
ps("\nLoropetalum execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;lor_execution(t,c,1145+(i*15),1134+(i*12),1116+(i*8),1103+(i*5),2021+(i%4));}
ps("\nLoropetalum evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;lor_evaluation(t,c,1137+(i*13),1126+(i*10),1110+(i*7),1099+(i*4),2022+(i%3));}
ps("\nLoropetalum accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lor_accessory(t,c,1129+(i*11),1120+(i*9),1106+(i*6),1096+(i*3),2023+(i%2));}
ps("\nLoropetalum marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;lor_market(t,c,1123+(i*9),1114+(i*7),1101+(i*5),1093+(i*3),2024);}
ps("\n");lor_report();lor_state();ps("\n=== Demo Complete ===\n");return 0;}
