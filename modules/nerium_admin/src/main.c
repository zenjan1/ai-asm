/* nerium_admin: Nerium oleander management (v1.0)
 * Nerium oleander cultivation, ornamental flowering, medicinal use
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ner_t;
typedef struct{int n_nerc,n_nerf,n_nerd,n_nerm,n_nerh,t_f1,t_f2,t_f3,t_f4,t_f5;} ner_state_t;
static ner_t nercs[N],nerfs[N-2],nerds[N-4],nerms[N-6],nerhs[N-6]; static ner_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ner_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ner_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[NER] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ner_init(void){if(init)return -1;st.n_nerc=0;st.n_nerf=0;st.n_nerd=0;st.n_nerm=0;st.n_nerh=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)nercs[i].active=0;for(int i=0;i<N-2;i++)nerfs[i].active=0;for(int i=0;i<N-4;i++)nerds[i].active=0;for(int i=0;i<N-6;i++)nerms[i].active=0;for(int i=0;i<N-6;i++)nerhs[i].active=0;init=1;ps("[NER] Nerium initialized\n");return 0;}
int ner_cultivation(int t,int c,int a,int b,int d,int e,int y){return add(nercs,&st.n_nerc,&st.t_f1,N,t,c,a,b,d,e,y);}
int ner_flowering(int t,int c,int a,int b,int d,int e,int y){return add(nerfs,&st.n_nerf,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ner_drought(int t,int c,int a,int b,int d,int e,int y){return add(nerds,&st.n_nerd,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ner_medicinal(int t,int c,int a,int b,int d,int e,int y){return add(nerms,&st.n_nerm,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ner_hedge(int t,int c,int a,int b,int d,int e,int y){return add(nerhs,&st.n_nerh,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ner_report(void){ps("[NER] Cultivation: ");pi(st.n_nerc);ps(" PCS=");pi(st.t_f1);ps("\nFlowering: ");pi(st.n_nerf);ps(" PCS=");pi(st.t_f2);ps("\nDrought: ");pi(st.n_nerd);ps(" PCS=");pi(st.t_f3);ps("\nMedicinal: ");pi(st.n_nerm);ps(" PCS=");pi(st.t_f4);ps("\nHedge: ");pi(st.n_nerh);ps(" USD=");pi(st.t_f5);ps("\n");}
void ner_state(void){ps("[NER] Cul=");pi(st.n_nerc);ps(" Flo=");pi(st.n_nerf);ps(" Dro=");pi(st.n_nerd);ps(" Med=");pi(st.n_nerm);ps(" Hed=");pi(st.n_nerh);ps("\n");}
int main(void){
ps("=== Nerium Admin Demo ===\n\n");ner_init();
ps("Nerium cultivation...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ner_cultivation(t,c,973+(i*17),962+(i*14),942+(i*10),924+(i*6),2020+(i%5));}
ps("\nFlowering...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ner_flowering(t,c,962+(i*15),951+(i*12),933+(i*8),920+(i*5),2021+(i%4));}
ps("\nDrought resistance...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ner_drought(t,c,954+(i*13),943+(i*10),927+(i*7),916+(i*4),2022+(i%3));}
ps("\nMedicinal compounds...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ner_medicinal(t,c,946+(i*11),937+(i*9),923+(i*6),913+(i*3),2023+(i%2));}
ps("\nHedge planting...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ner_hedge(t,c,940+(i*9),931+(i*7),918+(i*5),910+(i*3),2024);}
ps("\n");ner_report();ner_state();ps("\n=== Demo Complete ===\n");return 0;}
