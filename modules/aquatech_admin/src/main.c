/* aquatech_admin: Aquatic technology administration (v1.0)
 * Marine farming, freshwater farming, ocean fishing, seafood processing, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} aqa_t;
typedef struct{int n_mf,n_ff,n_of,n_sp,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} aqa_state_t;
static aqa_t mfs[N],ffs[N-2],ofs[N-4],sps[N-6],mks[N-6]; static aqa_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(aqa_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;aqa_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[AQA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int aqa_init(void){if(init)return -1;st.n_mf=0;st.n_ff=0;st.n_of=0;st.n_sp=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mfs[i].active=0;for(int i=0;i<N-2;i++)ffs[i].active=0;for(int i=0;i<N-4;i++)ofs[i].active=0;for(int i=0;i<N-6;i++)sps[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[AQA] Aquatech initialized\n");return 0;}
int aqa_marine(int t,int c,int a,int b,int d,int e,int y){return add(mfs,&st.n_mf,&st.t_f1,N,t,c,a,b,d,e,y);}
int aqa_freshwater(int t,int c,int a,int b,int d,int e,int y){return add(ffs,&st.n_ff,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int aqa_ocean(int t,int c,int a,int b,int d,int e,int y){return add(ofs,&st.n_of,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int aqa_process(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int aqa_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void aqa_report(void){ps("[AQA] Mf: ");pi(st.n_mf);ps(" PCS=");pi(st.t_f1);ps("\nFf: ");pi(st.n_ff);ps(" PCS=");pi(st.t_f2);ps("\nOf: ");pi(st.n_of);ps(" PCS=");pi(st.t_f3);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void aqa_state(void){ps("[AQA] Mf=");pi(st.n_mf);ps(" Ff=");pi(st.n_ff);ps(" Of=");pi(st.n_of);ps(" Sp=");pi(st.n_sp);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Aqua Tech Admin Demo ===\n\n");aqa_init();
ps("Marine farming...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;aqa_marine(t,c,248+(i*17),233+(i*14),213+(i*10),195+(i*6),2020+(i%5));}
ps("\nFreshwater farming...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;aqa_freshwater(t,c,237+(i*15),223+(i*12),205+(i*8),192+(i*5),2021+(i%4));}
ps("\nOcean fishing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;aqa_ocean(t,c,229+(i*13),215+(i*10),199+(i*7),188+(i*4),2022+(i%3));}
ps("\nSeafood processing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aqa_process(t,c,221+(i*11),209+(i*9),195+(i*6),185+(i*3),2023+(i%2));}
ps("\nAquatic marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;aqa_market(t,c,215+(i*9),204+(i*7),191+(i*5),183+(i*3),2024);}
ps("\n");aqa_report();aqa_state();ps("\n=== Demo Complete ===\n");return 0;}
