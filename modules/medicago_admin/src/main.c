/* medicago_admin: Medicago alfalfa & medic forage management (v1.0)
 * Alfalfa nitrogen fixation, hay quality grading, nodule bacteria, dormancy, livestock feed
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} med_t;
typedef struct{int n_medc,n_mede,n_me2,n_mn,n_lf,t_f1,t_f2,t_f3,t_f4,t_f5;} med_state_t;
static med_t medcs[N],medes[N-2],me2s[N-4],mns[N-6],lfs[N-6]; static med_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(med_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;med_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MED] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int med_init(void){if(init)return -1;st.n_medc=0;st.n_mede=0;st.n_me2=0;st.n_mn=0;st.n_lf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)medcs[i].active=0;for(int i=0;i<N-2;i++)medes[i].active=0;for(int i=0;i<N-4;i++)me2s[i].active=0;for(int i=0;i<N-6;i++)mns[i].active=0;for(int i=0;i<N-6;i++)lfs[i].active=0;init=1;ps("[MED] Medicago initialized\n");return 0;}
int med_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(medcs,&st.n_medc,&st.t_f1,N,t,c,a,b,d,e,y);}
int med_hay(int t,int c,int a,int b,int d,int e,int y){return add(medes,&st.n_mede,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int med_nodule(int t,int c,int a,int b,int d,int e,int y){return add(me2s,&st.n_me2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int med_dormancy(int t,int c,int a,int b,int d,int e,int y){return add(mns,&st.n_mn,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int med_feed(int t,int c,int a,int b,int d,int e,int y){return add(lfs,&st.n_lf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void med_report(void){ps("[MED] Cultivation: ");pi(st.n_medc);ps(" PCS=");pi(st.t_f1);ps("\nHay: ");pi(st.n_mede);ps(" PCS=");pi(st.t_f2);ps("\nNodule: ");pi(st.n_me2);ps(" PCS=");pi(st.t_f3);ps("\nDormancy: ");pi(st.n_mn);ps(" PCS=");pi(st.t_f4);ps("\nFeed: ");pi(st.n_lf);ps(" USD=");pi(st.t_f5);ps("\n");}
void med_state(void){ps("[MED] Cul=");pi(st.n_medc);ps(" Hay=");pi(st.n_mede);ps(" Nod=");pi(st.n_me2);ps(" Dor=");pi(st.n_mn);ps(" Fee=");pi(st.n_lf);ps("\n");}
int main(void){
ps("=== Medicago Admin Demo ===\n\n");med_init();
ps("Alfalfa cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;med_cultivation(t,c,925+(i*17),914+(i*14),894+(i*10),876+(i*6),2020+(i%5));}
ps("\nHay quality grading...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;med_hay(t,c,914+(i*15),903+(i*12),885+(i*8),872+(i*5),2021+(i%4));}
ps("\nRoot nodule bacteria...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;med_nodule(t,c,906+(i*13),895+(i*10),879+(i*7),868+(i*4),2022+(i%3));}
ps("\nDormancy cycle...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;med_dormancy(t,c,898+(i*11),889+(i*9),875+(i*6),865+(i*3),2023+(i%2));}
ps("\nLivestock feed...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;med_feed(t,c,892+(i*9),883+(i*7),870+(i*5),862+(i*3),2024);}
ps("\n");med_report();med_state();ps("\n=== Demo Complete ===\n");return 0;}
