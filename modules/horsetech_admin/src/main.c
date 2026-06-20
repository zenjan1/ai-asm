/* horsetech_admin: Horse technology administration (v1.0)
 * Horse breeding, equestrian sports, horse stud, horse equipment, horse tourism
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} hrt_t;
typedef struct{int n_br,n_sp,n_st,n_eq,n_to,t_f1,t_f2,t_f3,t_f4,t_f5;} hrt_state_t;
static hrt_t brs[N],sps[N-2],sts[N-4],eqs[N-6],tos[N-6]; static hrt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(hrt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;hrt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[HRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int hrt_init(void){if(init)return -1;st.n_br=0;st.n_sp=0;st.n_st=0;st.n_eq=0;st.n_to=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)brs[i].active=0;for(int i=0;i<N-2;i++)sps[i].active=0;for(int i=0;i<N-4;i++)sts[i].active=0;for(int i=0;i<N-6;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)tos[i].active=0;init=1;ps("[HRT] Horsetech initialized\n");return 0;}
int hrt_breed(int t,int c,int a,int b,int d,int e,int y){return add(brs,&st.n_br,&st.t_f1,N,t,c,a,b,d,e,y);}
int hrt_sport(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int hrt_stud(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int hrt_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int hrt_tourism(int t,int c,int a,int b,int d,int e,int y){return add(tos,&st.n_to,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void hrt_report(void){ps("[HRT] Breed: ");pi(st.n_br);ps(" Head=");pi(st.t_f1);ps("\nSport: ");pi(st.n_sp);ps(" PTS=");pi(st.t_f2);ps("\nStud: ");pi(st.n_st);ps(" DNA=");pi(st.t_f3);ps("\nEquip: ");pi(st.n_eq);ps(" Tack=");pi(st.t_f4);ps("\nTour: ");pi(st.n_to);ps(" Guest=");pi(st.t_f5);ps("\n");}
void hrt_state(void){ps("[HRT] Br=");pi(st.n_br);ps(" Sp=");pi(st.n_sp);ps(" St=");pi(st.n_st);ps(" Eq=");pi(st.n_eq);ps(" To=");pi(st.n_to);ps("\n");}
int main(void){
ps("=== Horse Tech Admin Demo ===\n\n");hrt_init();
ps("Horse breeding...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;hrt_breed(t,c,169+(i*17),154+(i*14),134+(i*10),117+(i*6),2020+(i%5));}
ps("\nEquestrian sports...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;hrt_sport(t,c,158+(i*15),144+(i*12),126+(i*8),113+(i*5),2021+(i%4));}
ps("\nHorse stud...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;hrt_stud(t,c,150+(i*13),136+(i*10),120+(i*7),109+(i*4),2022+(i%3));}
ps("\nHorse equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrt_equipment(t,c,142+(i*11),130+(i*9),116+(i*6),106+(i*3),2023+(i%2));}
ps("\nHorse tourism...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;hrt_tourism(t,c,136+(i*9),125+(i*7),112+(i*5),104+(i*3),2024);}
ps("\n");hrt_report();hrt_state();ps("\n=== Demo Complete ===\n");return 0;}
