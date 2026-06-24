/* myrica_admin: Myrica bayberry management (v1.0)
 * Myrica cerifera (wax myrtle), M. gale (bog myrtle) cultivation
 * Wax extraction, nitrogen fixation, coastal landscaping
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} myr_t;
typedef struct{int n_myrw,n_myrc,n_myrf,n_myrn,n_myrm,t_f1,t_f2,t_f3,t_f4,t_f5;} myr_state_t;
static myr_t myrws[N],myrcs[N-2],myrfs[N-4],myrns[N-6],myrms[N-6]; static myr_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(myr_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;myr_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MYR] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int myr_init(void){if(init)return -1;st.n_myrw=0;st.n_myrc=0;st.n_myrf=0;st.n_myrn=0;st.n_myrm=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)myrws[i].active=0;for(int i=0;i<N-2;i++)myrcs[i].active=0;for(int i=0;i<N-4;i++)myrfs[i].active=0;for(int i=0;i<N-6;i++)myrns[i].active=0;for(int i=0;i<N-6;i++)myrms[i].active=0;init=1;ps("[MYR] Myrica initialized\n");return 0;}
int myr_wax(int t,int c,int a,int b,int d,int e,int y){return add(myrws,&st.n_myrw,&st.t_f1,N,t,c,a,b,d,e,y);}
int myr_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(myrcs,&st.n_myrc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int myr_fixation(int t,int c,int a,int b,int d,int e,int y){return add(myrfs,&st.n_myrf,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int myr_nitrogen(int t,int c,int a,int b,int d,int e,int y){return add(myrns,&st.n_myrn,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int myr_medicinal(int t,int c,int a,int b,int d,int e,int y){return add(myrms,&st.n_myrm,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void myr_report(void){ps("[MYR] Wax: ");pi(st.n_myrw);ps(" PCS=");pi(st.t_f1);ps("\nCultivation: ");pi(st.n_myrc);ps(" PCS=");pi(st.t_f2);ps("\nFixation: ");pi(st.n_myrf);ps(" PCS=");pi(st.t_f3);ps("\nNitrogen: ");pi(st.n_myrn);ps(" PCS=");pi(st.t_f4);ps("\nMedicinal: ");pi(st.n_myrm);ps(" USD=");pi(st.t_f5);ps("\n");}
void myr_state(void){ps("[MYR] Wax=");pi(st.n_myrw);ps(" Cul=");pi(st.n_myrc);ps(" Fix=");pi(st.n_myrf);ps(" Nit=");pi(st.n_myrn);ps(" Med=");pi(st.n_myrm);ps("\n");}
int main(void){
ps("=== Myrica Admin Demo ===\n\n");myr_init();
ps("Wax extraction...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;myr_wax(t,c,949+(i*17),938+(i*14),918+(i*10),900+(i*6),2020+(i%5));}
ps("\nCultivation...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;myr_cultivation(t,c,938+(i*15),927+(i*12),909+(i*8),896+(i*5),2021+(i%4));}
ps("\nNitrogen fixation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;myr_fixation(t,c,930+(i*13),919+(i*10),903+(i*7),892+(i*4),2022+(i%3));}
ps("\nSoil nitrogen...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;myr_nitrogen(t,c,922+(i*11),913+(i*9),899+(i*6),889+(i*3),2023+(i%2));}
ps("\nMedicinal uses...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;myr_medicinal(t,c,916+(i*9),907+(i*7),894+(i*5),886+(i*3),2024);}
ps("\n");myr_report();myr_state();ps("\n=== Demo Complete ===\n");return 0;}
