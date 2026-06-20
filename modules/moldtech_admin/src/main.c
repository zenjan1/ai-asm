/* moldtech_admin: Mold technology administration (v1.0)
 * Injection mold, stamping mold, die-casting mold, forging mold, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} mld_t;
typedef struct{int n_ij,n_st,n_dc,n_fg,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} mld_state_t;
static mld_t ijs[N],sts[N-2],dcs[N-4],fgs[N-6],mks[N-6]; static mld_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mld_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;mld_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[MLD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int mld_init(void){if(init)return -1;st.n_ij=0;st.n_st=0;st.n_dc=0;st.n_fg=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)ijs[i].active=0;for(int i=0;i<N-2;i++)sts[i].active=0;for(int i=0;i<N-4;i++)dcs[i].active=0;for(int i=0;i<N-6;i++)fgs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[MLD] Moldtech initialized\n");return 0;}
int mld_injection(int t,int c,int a,int b,int d,int e,int y){return add(ijs,&st.n_ij,&st.t_f1,N,t,c,a,b,d,e,y);}
int mld_stamping(int t,int c,int a,int b,int d,int e,int y){return add(sts,&st.n_st,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int mld_diecast(int t,int c,int a,int b,int d,int e,int y){return add(dcs,&st.n_dc,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int mld_forging(int t,int c,int a,int b,int d,int e,int y){return add(fgs,&st.n_fg,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int mld_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void mld_report(void){ps("[MLD] Ij: ");pi(st.n_ij);ps(" PCS=");pi(st.t_f1);ps("\nSt: ");pi(st.n_st);ps(" PCS=");pi(st.t_f2);ps("\nDc: ");pi(st.n_dc);ps(" PCS=");pi(st.t_f3);ps("\nFg: ");pi(st.n_fg);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void mld_state(void){ps("[MLD] Ij=");pi(st.n_ij);ps(" St=");pi(st.n_st);ps(" Dc=");pi(st.n_dc);ps(" Fg=");pi(st.n_fg);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Mold Tech Admin Demo ===\n\n");mld_init();
ps("Injection mold...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;mld_injection(t,c,248+(i*17),233+(i*14),213+(i*10),195+(i*6),2020+(i%5));}
ps("\nStamping mold...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;mld_stamping(t,c,237+(i*15),223+(i*12),205+(i*8),192+(i*5),2021+(i%4));}
ps("\nDie-casting mold...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;mld_diecast(t,c,229+(i*13),215+(i*10),199+(i*7),188+(i*4),2022+(i%3));}
ps("\nForging mold...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mld_forging(t,c,221+(i*11),209+(i*9),195+(i*6),185+(i*3),2023+(i%2));}
ps("\nMold marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;mld_market(t,c,215+(i*9),204+(i*7),191+(i*5),183+(i*3),2024);}
ps("\n");mld_report();mld_state();ps("\n=== Demo Complete ===\n");return 0;}
