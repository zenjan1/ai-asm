/* filipendula_admin: Filipendula management technology administration (v1.0)
 * Filipendula planning, filipendula execution, filipendula evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} fil_t;
typedef struct{int n_filp,n_file,n_fil2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} fil_state_t;
static fil_t filps[N],filss[N-2],filvss[N-4],filas[N-6],filmks[N-6]; static fil_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(fil_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;fil_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[FIL] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int fil_init(void){if(init)return -1;st.n_filp=0;st.n_file=0;st.n_fil2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)filps[i].active=0;for(int i=0;i<N-2;i++)filss[i].active=0;for(int i=0;i<N-4;i++)filvss[i].active=0;for(int i=0;i<N-6;i++)filas[i].active=0;for(int i=0;i<N-6;i++)filmks[i].active=0;init=1;ps("[FIL] Filipendula initialized\n");return 0;}
int fil_planning(int t,int c,int a,int b,int d,int e,int y){return add(filps,&st.n_filp,&st.t_f1,N,t,c,a,b,d,e,y);}
int fil_execution(int t,int c,int a,int b,int d,int e,int y){return add(filss,&st.n_file,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int fil_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(filvss,&st.n_fil2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int fil_accessory(int t,int c,int a,int b,int d,int e,int y){return add(filas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int fil_market(int t,int c,int a,int b,int d,int e,int y){return add(filmks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void fil_report(void){ps("[FIL] Filp: ");pi(st.n_filp);ps(" PCS=");pi(st.t_f1);ps("\nFile: ");pi(st.n_file);ps(" PCS=");pi(st.t_f2);ps("\nFilv: ");pi(st.n_fil2);ps(" PCS=");pi(st.t_f3);ps("\nFilc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void fil_state(void){ps("[FIL] Filp=");pi(st.n_filp);ps(" File=");pi(st.n_file);ps(" Filv=");pi(st.n_fil2);ps(" Filc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Filipendula Admin Demo ===\n\n");fil_init();
ps("Filipendula planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;fil_planning(t,c,923+(i*17),912+(i*14),892+(i*10),874+(i*6),2020+(i%5));}
ps("\nFilipendula execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;fil_execution(t,c,912+(i*15),901+(i*12),883+(i*8),870+(i*5),2021+(i%4));}
ps("\nFilipendula evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;fil_evaluation(t,c,904+(i*13),893+(i*10),877+(i*7),866+(i*4),2022+(i%3));}
ps("\nFilipendula accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fil_accessory(t,c,896+(i*11),887+(i*9),873+(i*6),863+(i*3),2023+(i%2));}
ps("\nFilipendula marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;fil_market(t,c,890+(i*9),881+(i*7),868+(i*5),860+(i*3),2024);}
ps("\n");fil_report();fil_state();ps("\n=== Demo Complete ===\n");return 0;}
