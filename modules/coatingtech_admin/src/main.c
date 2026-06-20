/* coatingtech_admin: Coating technology administration (v1.0)
 * Paint coatings, powder coatings, thermal spray coatings, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} ctg_t;
typedef struct{int n_pt,n_pw,n_ts,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} ctg_state_t;
static ctg_t pts[N],pws[N-2],tss[N-4],acs[N-6],mks[N-6]; static ctg_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(ctg_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;ctg_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CTG] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int ctg_init(void){if(init)return -1;st.n_pt=0;st.n_pw=0;st.n_ts=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pts[i].active=0;for(int i=0;i<N-2;i++)pws[i].active=0;for(int i=0;i<N-4;i++)tss[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[CTG] Coatingtech initialized\n");return 0;}
int ctg_paint(int t,int c,int a,int b,int d,int e,int y){return add(pts,&st.n_pt,&st.t_f1,N,t,c,a,b,d,e,y);}
int ctg_powder(int t,int c,int a,int b,int d,int e,int y){return add(pws,&st.n_pw,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int ctg_spray(int t,int c,int a,int b,int d,int e,int y){return add(tss,&st.n_ts,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int ctg_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int ctg_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void ctg_report(void){ps("[CTG] Pt: ");pi(st.n_pt);ps(" PCS=");pi(st.t_f1);ps("\nPw: ");pi(st.n_pw);ps(" PCS=");pi(st.t_f2);ps("\nTs: ");pi(st.n_ts);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void ctg_state(void){ps("[CTG] Pt=");pi(st.n_pt);ps(" Pw=");pi(st.n_pw);ps(" Ts=");pi(st.n_ts);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Coating Tech Admin Demo ===\n\n");ctg_init();
ps("Paint coatings...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;ctg_paint(t,c,272+(i*17),257+(i*14),237+(i*10),219+(i*6),2020+(i%5));}
ps("\nPowder coatings...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;ctg_powder(t,c,261+(i*15),247+(i*12),229+(i*8),216+(i*5),2021+(i%4));}
ps("\nThermal spray...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;ctg_spray(t,c,253+(i*13),239+(i*10),223+(i*7),212+(i*4),2022+(i%3));}
ps("\nCoating accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctg_accessory(t,c,245+(i*11),233+(i*9),219+(i*6),209+(i*3),2023+(i%2));}
ps("\nCoating marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;ctg_market(t,c,239+(i*9),228+(i*7),215+(i*5),207+(i*3),2024);}
ps("\n");ctg_report();ctg_state();ps("\n=== Demo Complete ===\n");return 0;}
