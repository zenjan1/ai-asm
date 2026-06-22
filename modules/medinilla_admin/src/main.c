/* medinilla_admin: Medinilla management technology administration (v1.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} medi_t;
typedef struct{int n_mediop,n_medi,n_medi2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} medi_state_t;
static medi_t medios[N],medie[N-2],medi2[N-4],mediac[N-6],mediam[N-6]; static medi_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(medi_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;medi_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MEDI] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int medi_init(void){if(init)return -1;st.n_mediop=0;st.n_medi=0;st.n_medi2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)medios[i].active=0;for(int i=0;i<N-2;i++)medie[i].active=0;for(int i=0;i<N-4;i++)medi2[i].active=0;for(int i=0;i<N-6;i++)mediac[i].active=0;for(int i=0;i<N-6;i++)mediam[i].active=0;init=1;ps("[MEDI] Medinilla initialized\n");return 0;}
int medi_planning(int t,int c,int a,int b,int d,int e,int y){return add(medios,&st.n_mediop,&st.t_f1,N,t,c,a,b,d,e,y);}
int medi_execution(int t,int c,int a,int b,int d,int e,int y){return add(medie,&st.n_medi,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int medi_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(medi2,&st.n_medi2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int medi_accessory(int t,int c,int a,int b,int d,int e,int y){return add(mediac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int medi_market(int t,int c,int a,int b,int d,int e,int y){return add(mediam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void medi_report(void){ps("[MEDI] Medip: ");pi(st.n_mediop);ps(" PCS=");pi(st.t_f1);ps("\nMedie: ");pi(st.n_medi);ps(" PCS=");pi(st.t_f2);ps("\nMedi2: ");pi(st.n_medi2);ps(" PCS=");pi(st.t_f3);ps("\nMdi: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void medi_state(void){ps("[MEDI] Medip=");pi(st.n_mediop);ps(" Medi=");pi(st.n_medi);ps(" Di2=");pi(st.n_medi2);ps(" Lac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Medinilla Admin Demo ===\n\n");medi_init();
ps("Medinilla planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;medi_planning(t,c,1567-(i*17),1556-(i*14),1536-(i*10),1518-(i*6),2020+(i%5));}
ps("\nMedinilla execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;medi_execution(t,c,1556-(i*15),1545-(i*12),1527-(i*8),1514-(i*5),2021+(i%4));}
ps("\nMedinilla evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;medi_evaluation(t,c,1548-(i*13),1537-(i*10),1521-(i*7),1510-(i*4),2022+(i%3));}
ps("\nMedinilla accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;medi_accessory(t,c,1540-(i*11),1531-(i*9),1517-(i*6),1507-(i*3),2023+(i%2));}
ps("\nMedinilla marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;medi_market(t,c,1534-(i*9),1525-(i*7),1512-(i*5),1504-(i*3),2024);}
ps("\n");medi_report();medi_state();ps("\n=== Demo Complete ===\n");return 0;}
