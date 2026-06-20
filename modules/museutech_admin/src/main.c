/* museutech_admin: Museum technology administration (v1.0)
 * Collection management, exhibition planning, digital display, education, conservation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mus_t;
typedef struct{int n_co,n_ex,n_di,n_ed,n_cv,t_f1,t_f2,t_f3,t_f4,t_f5;} mus_state_t;
static mus_t cos[N],exs[N-2],dis[N-4],eds[N-6],cvs[N-6]; static mus_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mus_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mus_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MUS] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mus_init(void){if(init)return -1;st.n_co=0;st.n_ex=0;st.n_di=0;st.n_ed=0;st.n_cv=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cos[i].active=0;for(int i=0;i<N-2;i++)exs[i].active=0;for(int i=0;i<N-4;i++)dis[i].active=0;for(int i=0;i<N-6;i++)eds[i].active=0;for(int i=0;i<N-6;i++)cvs[i].active=0;init=1;ps("[MUS] Museutech initialized\n");return 0;}
int mus_collection(int t,int c,int a,int b,int d,int e,int y){return add(cos,&st.n_co,&st.t_f1,N,t,c,a,b,d,e,y);}
int mus_exhibit(int t,int c,int a,int b,int d,int e,int y){return add(exs,&st.n_ex,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mus_digital(int t,int c,int a,int b,int d,int e,int y){return add(dis,&st.n_di,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mus_education(int t,int c,int a,int b,int d,int e,int y){return add(eds,&st.n_ed,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mus_conserve(int t,int c,int a,int b,int d,int e,int y){return add(cvs,&st.n_cv,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mus_report(void){ps("[MUS] Coll: ");pi(st.n_co);ps(" PC=");pi(st.t_f1);ps("\nExhib: ");pi(st.n_ex);ps(" Hall=");pi(st.t_f2);ps("\nDigi: ");pi(st.n_di);ps(" VR=");pi(st.t_f3);ps("\nEdu: ");pi(st.n_ed);ps(" Tour=");pi(st.t_f4);ps("\nCons: ");pi(st.n_cv);ps(" pH=");pi(st.t_f5);ps("\n");}
void mus_state(void){ps("[MUS] Co=");pi(st.n_co);ps(" Ex=");pi(st.n_ex);ps(" Di=");pi(st.n_di);ps(" Ed=");pi(st.n_ed);ps(" Cv=");pi(st.n_cv);ps("\n");}
int main(void){
ps("=== Museum Tech Admin Demo ===\n\n");mus_init();
ps("Collection management...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mus_collection(t,c,171+(i*17),156+(i*14),136+(i*10),119+(i*6),2020+(i%5));}
ps("\nExhibition planning...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mus_exhibit(t,c,160+(i*15),146+(i*12),128+(i*8),115+(i*5),2021+(i%4));}
ps("\nDigital display...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mus_digital(t,c,152+(i*13),138+(i*10),122+(i*7),111+(i*4),2022+(i%3));}
ps("\nEducation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mus_education(t,c,144+(i*11),132+(i*9),118+(i*6),108+(i*3),2023+(i%2));}
ps("\nConservation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mus_conserve(t,c,138+(i*9),127+(i*7),114+(i*5),106+(i*3),2024);}
ps("\n");mus_report();mus_state();ps("\n=== Demo Complete ===\n");return 0;}
