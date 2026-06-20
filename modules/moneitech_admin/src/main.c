/* moneitech_admin: Money and finance technology administration (v1.0)
 * Payment and lending, investment and wealth, blockchain, digital banking, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mnt_t;
typedef struct{int n_pl,n_iw,n_bc,n_db,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mnt_state_t;
static mnt_t pls[N],iws[N-2],bcs[N-4],dbs[N-6],mks[N-6]; static mnt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mnt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mnt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MNT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mnt_init(void){if(init)return -1;st.n_pl=0;st.n_iw=0;st.n_bc=0;st.n_db=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pls[i].active=0;for(int i=0;i<N-2;i++)iws[i].active=0;for(int i=0;i<N-4;i++)bcs[i].active=0;for(int i=0;i<N-6;i++)dbs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MNT] Moneitech initialized\n");return 0;}
int mnt_payment(int t,int c,int a,int b,int d,int e,int y){return add(pls,&st.n_pl,&st.t_f1,N,t,c,a,b,d,e,y);}
int mnt_invest(int t,int c,int a,int b,int d,int e,int y){return add(iws,&st.n_iw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mnt_blockchain(int t,int c,int a,int b,int d,int e,int y){return add(bcs,&st.n_bc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mnt_banking(int t,int c,int a,int b,int d,int e,int y){return add(dbs,&st.n_db,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mnt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mnt_report(void){ps("[MNT] Pl: ");pi(st.n_pl);ps(" PCS=");pi(st.t_f1);ps("\nIw: ");pi(st.n_iw);ps(" USD=");pi(st.t_f2);ps("\nBc: ");pi(st.n_bc);ps(" PCS=");pi(st.t_f3);ps("\nDb: ");pi(st.n_db);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mnt_state(void){ps("[MNT] Pl=");pi(st.n_pl);ps(" Iw=");pi(st.n_iw);ps(" Bc=");pi(st.n_bc);ps(" Db=");pi(st.n_db);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Money Finance Tech Admin Demo ===\n\n");mnt_init();
ps("Payment and lending...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mnt_payment(t,c,244+(i*17),229+(i*14),209+(i*10),191+(i*6),2020+(i%5));}
ps("\nInvestment and wealth...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mnt_invest(t,c,233+(i*15),219+(i*12),201+(i*8),188+(i*5),2021+(i%4));}
ps("\nBlockchain...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mnt_blockchain(t,c,225+(i*13),211+(i*10),195+(i*7),184+(i*4),2022+(i%3));}
ps("\nDigital banking...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mnt_banking(t,c,217+(i*11),205+(i*9),191+(i*6),181+(i*3),2023+(i%2));}
ps("\nFinance marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mnt_market(t,c,211+(i*9),200+(i*7),187+(i*5),179+(i*3),2024);}
ps("\n");mnt_report();mnt_state();ps("\n=== Demo Complete ===\n");return 0;}
