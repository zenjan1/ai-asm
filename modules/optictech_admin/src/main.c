/* optictech_admin: Optical technology administration (v1.0)
 * Optical lenses, eyeglasses, optical instruments, optoelectronic products, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} opt_t;
typedef struct{int n_ln,n_eg,n_in,n_op,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} opt_state_t;
static opt_t lns[N],egs[N-2],ins[N-4],ops[N-6],mks[N-6]; static opt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(opt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;opt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[OPT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int opt_init(void){if(init)return -1;st.n_ln=0;st.n_eg=0;st.n_in=0;st.n_op=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)lns[i].active=0;for(int i=0;i<N-2;i++)egs[i].active=0;for(int i=0;i<N-4;i++)ins[i].active=0;for(int i=0;i<N-6;i++)ops[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[OPT] Optictech initialized\n");return 0;}
int opt_lens(int t,int c,int a,int b,int d,int e,int y){return add(lns,&st.n_ln,&st.t_f1,N,t,c,a,b,d,e,y);}
int opt_eyeglass(int t,int c,int a,int b,int d,int e,int y){return add(egs,&st.n_eg,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int opt_instrument(int t,int c,int a,int b,int d,int e,int y){return add(ins,&st.n_in,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int opt_optoelec(int t,int c,int a,int b,int d,int e,int y){return add(ops,&st.n_op,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int opt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void opt_report(void){ps("[OPT] Ln: ");pi(st.n_ln);ps(" PCS=");pi(st.t_f1);ps("\nEg: ");pi(st.n_eg);ps(" PCS=");pi(st.t_f2);ps("\nIn: ");pi(st.n_in);ps(" PCS=");pi(st.t_f3);ps("\nOp: ");pi(st.n_op);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void opt_state(void){ps("[OPT] Ln=");pi(st.n_ln);ps(" Eg=");pi(st.n_eg);ps(" In=");pi(st.n_in);ps(" Op=");pi(st.n_op);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Optical Tech Admin Demo ===\n\n");opt_init();
ps("Optical lenses...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;opt_lens(t,c,248+(i*17),233+(i*14),213+(i*10),195+(i*6),2020+(i%5));}
ps("\nEyeglasses...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;opt_eyeglass(t,c,237+(i*15),223+(i*12),205+(i*8),192+(i*5),2021+(i%4));}
ps("\nOptical instruments...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;opt_instrument(t,c,229+(i*13),215+(i*10),199+(i*7),188+(i*4),2022+(i%3));}
ps("\nOptoelectronic products...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opt_optoelec(t,c,221+(i*11),209+(i*9),195+(i*6),185+(i*3),2023+(i%2));}
ps("\nOptical marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;opt_market(t,c,215+(i*9),204+(i*7),191+(i*5),183+(i*3),2024);}
ps("\n");opt_report();opt_state();ps("\n=== Demo Complete ===\n");return 0;}
