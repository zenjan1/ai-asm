/* anchor_admin: Anchoring technology administration (v1.0)
 * Mechanical anchors, chemical anchors, special anchoring, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} anx_t;
typedef struct{int n_mc,n_ch,n_sp,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} anx_state_t;
static anx_t mcs[N],chs[N-2],sps[N-4],acs[N-6],mks[N-6]; static anx_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(anx_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;anx_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ANX] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int anx_init(void){if(init)return -1;st.n_mc=0;st.n_ch=0;st.n_sp=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)mcs[i].active=0;for(int i=0;i<N-2;i++)chs[i].active=0;for(int i=0;i<N-4;i++)sps[i].active=0;for(int i=0;i<N-6;i++)acs[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[ANX] Anchor initialized\n");return 0;}
int anx_mechanical(int t,int c,int a,int b,int d,int e,int y){return add(mcs,&st.n_mc,&st.t_f1,N,t,c,a,b,d,e,y);}
int anx_chemical(int t,int c,int a,int b,int d,int e,int y){return add(chs,&st.n_ch,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int anx_special(int t,int c,int a,int b,int d,int e,int y){return add(sps,&st.n_sp,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int anx_accessory(int t,int c,int a,int b,int d,int e,int y){return add(acs,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int anx_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void anx_report(void){ps("[ANX] Mc: ");pi(st.n_mc);ps(" PCS=");pi(st.t_f1);ps("\nCh: ");pi(st.n_ch);ps(" PCS=");pi(st.t_f2);ps("\nSp: ");pi(st.n_sp);ps(" PCS=");pi(st.t_f3);ps("\nAc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void anx_state(void){ps("[ANX] Mc=");pi(st.n_mc);ps(" Ch=");pi(st.n_ch);ps(" Sp=");pi(st.n_sp);ps(" Ac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Anchor Admin Demo ===\n\n");anx_init();
ps("Mechanical anchors...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;anx_mechanical(t,c,420+(i*17),409+(i*14),389+(i*10),371+(i*6),2020+(i%5));}
ps("\nChemical anchors...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;anx_chemical(t,c,409+(i*15),398+(i*12),380+(i*8),367+(i*5),2021+(i%4));}
ps("\nSpecial anchoring...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;anx_special(t,c,401+(i*13),390+(i*10),374+(i*7),363+(i*4),2022+(i%3));}
ps("\nAnchoring accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anx_accessory(t,c,393+(i*11),384+(i*9),370+(i*6),360+(i*3),2023+(i%2));}
ps("\nAnchoring marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;anx_market(t,c,387+(i*9),378+(i*7),365+(i*5),357+(i*3),2024);}
ps("\n");anx_report();anx_state();ps("\n=== Demo Complete ===\n");return 0;}
