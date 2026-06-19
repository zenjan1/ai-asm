/* monetary_economics_admin: Monetary economics administration system (v1.0)
 * Monetary theory, banking system, monetary transmission, financial intermediation, monetary innovation
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} m_sub_t;
typedef struct{int n_mon,n_bnk,n_tra,n_fin,n_inn,t_f1,t_f2,t_f3,t_f4,t_f5;} mon_state_t;
static m_sub_t mons[N],bnks[N-2],tras[N-4],fins[N-6],inns[N-6]; static mon_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(m_sub_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;m_sub_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MON] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mon_init(void){if(init)return -1;st.n_mon=0;st.n_bnk=0;st.n_tra=0;st.n_fin=0;st.n_inn=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mons[i].active=0;for(int i=0;i<N-2;i++)bnks[i].active=0;for(int i=0;i<N-4;i++)tras[i].active=0;for(int i=0;i<N-6;i++)fins[i].active=0;for(int i=0;i<N-6;i++)inns[i].active=0;init=1;ps("[MON] Monetary economics initialized\n");return 0;}
int mon_theory(int t,int c,int a,int b,int d,int e,int y){return add(mons,&st.n_mon,&st.t_f1,N,t,c,a,b,d,e,y);}
int mon_banking(int t,int c,int a,int b,int d,int e,int y){return add(bnks,&st.n_bnk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mon_trans(int t,int c,int a,int b,int d,int e,int y){return add(tras,&st.n_tra,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mon_inter(int t,int c,int a,int b,int d,int e,int y){return add(fins,&st.n_fin,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mon_innov(int t,int c,int a,int b,int d,int e,int y){return add(inns,&st.n_inn,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mon_report(void){ps("[MON] Theory: ");pi(st.n_mon);ps(" Theory=");pi(st.t_f1);ps("\nBanking: ");pi(st.n_bnk);ps(" Bank=");pi(st.t_f2);ps("\nTrans: ");pi(st.n_tra);ps(" Trans=");pi(st.t_f3);ps("\nInter: ");pi(st.n_fin);ps(" Inter=");pi(st.t_f4);ps("\nInnov: ");pi(st.n_inn);ps(" Innov=");pi(st.t_f5);ps("\n");}
void mon_state(void){ps("[MON] Mo=");pi(st.n_mon);ps(" Bk=");pi(st.n_bnk);ps(" Tr=");pi(st.n_tra);ps(" Fi=");pi(st.n_fin);ps(" In=");pi(st.n_inn);ps("\n");}
int main(void){
ps("=== Monetary Economics Admin Demo ===\n\n");mon_init();
ps("Monetary theory...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mon_theory(t,c,76+(i*17),62+(i*14),42+(i*10),25+(i*6),2020+(i%5));}
ps("\nBanking system...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mon_banking(t,c,66+(i*15),52+(i*12),34+(i*8),21+(i*5),2021+(i%4));}
ps("\nMonetary transmission...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mon_trans(t,c,58+(i*13),44+(i*10),28+(i*6),17+(i*3),2022+(i%3));}
ps("\nFinancial intermediation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mon_inter(t,c,50+(i*11),38+(i*8),24+(i*5),15+(i*3),2023+(i%2));}
ps("\nMonetary innovation...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mon_innov(t,c,44+(i*9),33+(i*7),20+(i*4),13+(i*2),2024);}
ps("\n");mon_report();mon_state();ps("\n=== Demo Complete ===\n");return 0;}
