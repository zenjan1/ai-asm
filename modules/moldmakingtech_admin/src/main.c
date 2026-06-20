/* moldmakingtech_admin: Mold making technology administration (v1.0)
 * Injection dies, die casting dies, stamping dies, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mld_t;
typedef struct{int n_in,n_dc,n_st,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mld_state_t;
static mld_t ins[N],dcs[N-2],sts[N-4],acs[N-6],mks[N-6]; static mld_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mld_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mld_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mld_init(void){if(init)return -1;st.n_in=0;st.n_dc=0;st.n_st=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ins[i].active=0;for(int i=0;i<N-2;i++)dcs[i].active=0;for(int i=0;i<N-4;i++)sts[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MLD] Moldmakingtech initialized\n");return 0;}
int mld_injection(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f1,N,t,c,a,b,d,e,y);}
int mld_diecasting(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mld_stamping(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mld_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mld_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mld_report(void){ps("[MLD] In: ");pi(st.n_in);ps(" PCS=");pi(st.t_f1);ps("\nDc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f2);ps("\nSt: ");pi(st.n_st);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mld_state(void){ps("[MLD] In=");pi(st.n_in);ps(" Dc=");pi(st.n_dc);ps(" St=");pi(st.n_st);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mold Making Tech Admin Demo ===\n\n");mld_init();
ps("Injection dies...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mld_injection(t,c,295+(i*17),280+(i*14),260+(i*10),242+(i*6),2020+(i%5));}
ps("\nDie casting dies...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mld_diecasting(t,c,284+(i*15),270+(i*12),252+(i*8),239+(i*5),2021+(i%4));}
ps("\nStamping dies...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mld_stamping(t,c,276+(i*13),262+(i*10),246+(i*7),235+(i*4),2022+(i%3));}
ps("\nMold accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mld_accessory(t,c,268+(i*11),256+(i*9),242+(i*6),232+(i*3),2023+(i%2));}
ps("\nMold marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mld_market(t,c,262+(i*9),251+(i*7),238+(i*5),230+(i*3),2024);}
ps("\n");mld_report();mld_state();ps("\n=== Demo Complete ===\n");return 0;}
