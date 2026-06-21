/* choysia_admin: Choysia management technology administration (v1.0)
 * Choysia planning, choysia execution, choysia evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cho_t;
typedef struct{int n_chop,n_chse,n_cho2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cho_state_t;
static cho_t chops[N],choss[N-2],chovss[N-4],choas[N-6],chomks[N-6]; static cho_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cho_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cho_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cho_init(void){if(init)return -1;st.n_chop=0;st.n_chse=0;st.n_cho2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)chops[i].active=0;for(int i=0;i<N-2;i++)choss[i].active=0;for(int i=0;i<N-4;i++)chovss[i].active=0;for(int i=0;i<N-6;i++)choas[i].active=0;for(int i=0;i<N-6;i++)chomks[i].active=0;init=1;ps("[CHO] Choysia initialized\n");return 0;}
int cho_planning(int t,int c,int a,int b,int d,int e,int y){return add(chops,&st.n_chop,&st.t_f1,N,t,c,a,b,d,e,y);}
int cho_execution(int t,int c,int a,int b,int d,int e,int y){return add(choss,&st.n_chse,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cho_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(chovss,&st.n_cho2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cho_accessory(int t,int c,int a,int b,int d,int e,int y){return add(choas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cho_market(int t,int c,int a,int b,int d,int e,int y){return add(chomks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cho_report(void){ps("[CHO] Chopp: ");pi(st.n_chop);ps(" PCS=");pi(st.t_f1);ps("\nChse: ");pi(st.n_chse);ps(" PCS=");pi(st.t_f2);ps("\nChov: ");pi(st.n_cho2);ps(" PCS=");pi(st.t_f3);ps("\nChoc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cho_state(void){ps("[CHO] Chopp=");pi(st.n_chop);ps(" Chse=");pi(st.n_chse);ps(" Chov=");pi(st.n_cho2);ps(" Choc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Choysia Admin Demo ===\n\n");cho_init();
ps("Choysia planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cho_planning(t,c,984+(i*17),973+(i*14),953+(i*10),935+(i*6),2020+(i%5));}
ps("\nChoysia execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cho_execution(t,c,973+(i*15),962+(i*12),944+(i*8),931+(i*5),2021+(i%4));}
ps("\nChoysia evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cho_evaluation(t,c,965+(i*13),954+(i*10),938+(i*7),927+(i*4),2022+(i%3));}
ps("\nChoysia accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cho_accessory(t,c,957+(i*11),948+(i*9),934+(i*6),924+(i*3),2023+(i%2));}
ps("\nChoysia marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cho_market(t,c,951+(i*9),942+(i*7),929+(i*5),921+(i*3),2024);}
ps("\n");cho_report();cho_state();ps("\n=== Demo Complete ===\n");return 0;}
