/* statitech_admin: Stationery technology administration (v1.0)
 * Pen manufacturing, paper products, office supplies, art supplies, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} sta_t;
typedef struct{int n_pn,n_pp,n_of,n_art,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} sta_state_t;
static sta_t pns[N],pps[N-2],ofs[N-4],arts[N-6],mks[N-6]; static sta_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(sta_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;sta_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[STA] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int sta_init(void){if(init)return -1;st.n_pn=0;st.n_pp=0;st.n_of=0;st.n_art=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)pns[i].active=0;for(int i=0;i<N-2;i++)pps[i].active=0;for(int i=0;i<N-4;i++)ofs[i].active=0;for(int i=0;i<N-6;i++)arts[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[STA] Statitech initialized\n");return 0;}
int sta_pen(int t,int c,int a,int b,int d,int e,int y){return add(pns,&st.n_pn,&st.t_f1,N,t,c,a,b,d,e,y);}
int sta_paper(int t,int c,int a,int b,int d,int e,int y){return add(pps,&st.n_pp,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int sta_office(int t,int c,int a,int b,int d,int e,int y){return add(ofs,&st.n_of,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int sta_art(int t,int c,int a,int b,int d,int e,int y){return add(arts,&st.n_art,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int sta_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void sta_report(void){ps("[STA] Pen: ");pi(st.n_pn);ps(" PCS=");pi(st.t_f1);ps("\nPaper: ");pi(st.n_pp);ps(" Ream=");pi(st.t_f2);ps("\nOffice: ");pi(st.n_of);ps(" PCS=");pi(st.t_f3);ps("\nArt: ");pi(st.n_art);ps(" PCS=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void sta_state(void){ps("[STA] Pn=");pi(st.n_pn);ps(" Pp=");pi(st.n_pp);ps(" Of=");pi(st.n_of);ps(" Art=");pi(st.n_art);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Stationery Tech Admin Demo ===\n\n");sta_init();
ps("Pen manufacturing...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;sta_pen(t,c,197+(i*17),182+(i*14),162+(i*10),144+(i*6),2020+(i%5));}
ps("\nPaper products...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;sta_paper(t,c,186+(i*15),172+(i*12),154+(i*8),141+(i*5),2021+(i%4));}
ps("\nOffice supplies...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;sta_office(t,c,178+(i*13),164+(i*10),148+(i*7),137+(i*4),2022+(i%3));}
ps("\nArt supplies...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sta_art(t,c,170+(i*11),158+(i*9),144+(i*6),134+(i*3),2023+(i%2));}
ps("\nStationery marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;sta_market(t,c,164+(i*9),153+(i*7),140+(i*5),132+(i*3),2024);}
ps("\n");sta_report();sta_state();ps("\n=== Demo Complete ===\n");return 0;}
