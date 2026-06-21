/* echeveria_admin: Echeveria management technology administration (v1.0)
 * Echeveria planning, echeveria execution, echeveria evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ech_t;
typedef struct{int n_echp,n_eche,n_ech2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ech_state_t;
static ech_t echps[N],echss[N-2],echvss[N-4],echas[N-6],echmks[N-6]; static ech_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ech_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ech_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ECH] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ech_init(void){if(init)return -1;st.n_echp=0;st.n_eche=0;st.n_ech2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)echps[i].active=0;for(int i=0;i<N-2;i++)echss[i].active=0;for(int i=0;i<N-4;i++)echvss[i].active=0;for(int i=0;i<N-6;i++)echas[i].active=0;for(int i=0;i<N-6;i++)echmks[i].active=0;init=1;ps("[ECH] Echeveria initialized\n");return 0;}
int ech_planning(int t,int c,int a,int b,int d,int e,int y){return add(echps,&st.n_echp,&st.t_f1,N,t,c,a,b,d,e,y);}
int ech_execution(int t,int c,int a,int b,int d,int e,int y){return add(echss,&st.n_eche,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ech_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(echvss,&st.n_ech2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ech_accessory(int t,int c,int a,int b,int d,int e,int y){return add(echas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ech_market(int t,int c,int a,int b,int d,int e,int y){return add(echmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ech_report(void){ps("[ECH] Echip: ");pi(st.n_echp);ps(" PCS=");pi(st.t_f1);ps("\nEche: ");pi(st.n_eche);ps(" PCS=");pi(st.t_f2);ps("\nEchv: ");pi(st.n_ech2);ps(" PCS=");pi(st.t_f3);ps("\nEchc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ech_state(void){ps("[ECH] Echip=");pi(st.n_echp);ps(" Eche=");pi(st.n_eche);ps(" Echv=");pi(st.n_ech2);ps(" Echc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Echeveria Admin Demo ===\n\n");ech_init();
ps("Echeveria planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ech_planning(t,c,996+(i*17),985+(i*14),965+(i*10),947+(i*6),2020+(i%5));}
ps("\nEcheveria execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ech_execution(t,c,985+(i*15),974+(i*12),956+(i*8),943+(i*5),2021+(i%4));}
ps("\nEcheveria evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ech_evaluation(t,c,977+(i*13),966+(i*10),950+(i*7),939+(i*4),2022+(i%3));}
ps("\nEcheveria accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ech_accessory(t,c,969+(i*11),960+(i*9),946+(i*6),936+(i*3),2023+(i%2));}
ps("\nEcheveria marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ech_market(t,c,963+(i*9),954+(i*7),941+(i*5),933+(i*3),2024);}
ps("\n");ech_report();ech_state();ps("\n=== Demo Complete ===\n");return 0;}
