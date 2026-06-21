/* althaea_admin: Althaea management technology administration (v1.0)
 * Althaea planning, althaea execution, althaea evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} alt_t;
typedef struct{int n_altp,n_alte,n_alt2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} alt_state_t;
static alt_t altps[N],altss[N-2],altvss[N-4],altas[N-6],altmks[N-6]; static alt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(alt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;alt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ALT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int alt_init(void){if(init)return -1;st.n_altp=0;st.n_alte=0;st.n_alt2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)altps[i].active=0;for(int i=0;i<N-2;i++)altss[i].active=0;for(int i=0;i<N-4;i++)altvss[i].active=0;for(int i=0;i<N-6;i++)altas[i].active=0;for(int i=0;i<N-6;i++)altmks[i].active=0;init=1;ps("[ALT] Althaea initialized\n");return 0;}
int alt_planning(int t,int c,int a,int b,int d,int e,int y){return add(altps,&st.n_altp,&st.t_f1,N,t,c,a,b,d,e,y);}
int alt_execution(int t,int c,int a,int b,int d,int e,int y){return add(altss,&st.n_alte,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int alt_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(altvss,&st.n_alt2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int alt_accessory(int t,int c,int a,int b,int d,int e,int y){return add(altas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int alt_market(int t,int c,int a,int b,int d,int e,int y){return add(altmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void alt_report(void){ps("[ALT] Altp: ");pi(st.n_altp);ps(" PCS=");pi(st.t_f1);ps("\nAlte: ");pi(st.n_alte);ps(" PCS=");pi(st.t_f2);ps("\nAltv: ");pi(st.n_alt2);ps(" PCS=");pi(st.t_f3);ps("\nAlc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void alt_state(void){ps("[ALT] Altp=");pi(st.n_altp);ps(" Alte=");pi(st.n_alte);ps(" Altv=");pi(st.n_alt2);ps(" Alc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Althaea Admin Demo ===\n\n");alt_init();
ps("Althaea planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;alt_planning(t,c,893+(i*17),882+(i*14),862+(i*10),844+(i*6),2020+(i%5));}
ps("\nAlthaea execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;alt_execution(t,c,882+(i*15),871+(i*12),853+(i*8),840+(i*5),2021+(i%4));}
ps("\nAlthaea evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;alt_evaluation(t,c,874+(i*13),863+(i*10),847+(i*7),836+(i*4),2022+(i%3));}
ps("\nAlthaea accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alt_accessory(t,c,866+(i*11),857+(i*9),843+(i*6),833+(i*3),2023+(i%2));}
ps("\nAlthaea marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;alt_market(t,c,860+(i*9),851+(i*7),838+(i*5),830+(i*3),2024);}
ps("\n");alt_report();alt_state();ps("\n=== Demo Complete ===\n");return 0;}
