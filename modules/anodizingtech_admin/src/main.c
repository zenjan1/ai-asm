/* anodizingtech_admin: Anodizing technology administration (v1.0)
 * Sulfuric acid anodizing, oxalic acid anodizing, chromic acid anodizing, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ano_t;
typedef struct{int n_su,n_ox,n_ch,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ano_state_t;
static ano_t sus[N],oxs[N-2],chs[N-4],acs[N-6],mks[N-6]; static ano_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ano_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ano_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANO] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ano_init(void){if(init)return -1;st.n_su=0;st.n_ox=0;st.n_ch=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)sus[i].active=0;for(int i=0;i<N-2;i++)oxs[i].active=0;for(int i=0;i<N-4;i++)chs[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ANO] Anodizingtech initialized\n");return 0;}
int ano_sulfuric(int t,int c,int a,int b,int d,int e,int y){return add(sus,&st.n_su,&st.t_f1,N,t,c,a,b,d,e,y);}
int ano_oxalic(int t,int c,int a,int b,int d,int e,int y){return add(oxs,&st.n_ox,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ano_chromic(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ano_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ano_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ano_report(void){ps("[ANO] Su: ");pi(st.n_su);ps(" PCS=");pi(st.t_f1);ps("\nOx: ");pi(st.n_ox);ps(" PCS=");pi(st.t_f2);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ano_state(void){ps("[ANO] Su=");pi(st.n_su);ps(" Ox=");pi(st.n_ox);ps(" Ch=");pi(st.n_ch);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anodizing Tech Admin Demo ===\n\n");ano_init();
ps("Sulfuric acid anodizing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ano_sulfuric(t,c,340+(i*17),325+(i*14),305+(i*10),287+(i*6),2020+(i%5));}
ps("\nOxalic acid anodizing...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ano_oxalic(t,c,329+(i*15),315+(i*12),297+(i*8),284+(i*5),2021+(i%4));}
ps("\nChromic acid anodizing...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ano_chromic(t,c,321+(i*13),307+(i*10),291+(i*7),280+(i*4),2022+(i%3));}
ps("\nAnodizing accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ano_accessory(t,c,313+(i*11),301+(i*9),287+(i*6),277+(i*3),2023+(i%2));}
ps("\nAnodizing marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ano_market(t,c,307+(i*9),296+(i*7),283+(i*5),275+(i*3),2024);}
ps("\n");ano_report();ano_state();ps("\n=== Demo Complete ===\n");return 0;}
