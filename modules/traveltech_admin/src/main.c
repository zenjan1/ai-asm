/* traveltech_admin: Travel technology administration (v1.0)
 * Smart itinerary, online booking, destination management, travel experience, travel data
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} tvt_t;
typedef struct{int n_ti,n_bk,n_dm,n_te,n_td,t_f1,t_f2,t_f3,t_f4,t_f5;} tvt_state_t;
static tvt_t tis[N],bks[N-2],dms[N-4],tes[N-6],tds[N-6]; static tvt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(tvt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;tvt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TVT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int tvt_init(void){if(init)return -1;st.n_ti=0;st.n_bk=0;st.n_dm=0;st.n_te=0;st.n_td=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tis[i].active=0;for(int i=0;i<N-2;i++)bks[i].active=0;for(int i=0;i<N-4;i++)dms[i].active=0;for(int i=0;i<N-6;i++)tes[i].active=0;for(int i=0;i<N-6;i++)tds[i].active=0;init=1;ps("[TVT] Traveltech initialized\n");return 0;}
int tvt_itinerary(int t,int c,int a,int b,int d,int e,int y){return add(tis,&st.n_ti,&st.t_f1,N,t,c,a,b,d,e,y);}
int tvt_booking(int t,int c,int a,int b,int d,int e,int y){return add(bks,&st.n_bk,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int tvt_destination(int t,int c,int a,int b,int d,int e,int y){return add(dms,&st.n_dm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int tvt_experience(int t,int c,int a,int b,int d,int e,int y){return add(tes,&st.n_te,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int tvt_data(int t,int c,int a,int b,int d,int e,int y){return add(tds,&st.n_td,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void tvt_report(void){ps("[TVT] Trip: ");pi(st.n_ti);ps(" AI=");pi(st.t_f1);ps("\nBook: ");pi(st.n_bk);ps(" OTA=");pi(st.t_f2);ps("\nDest: ");pi(st.n_dm);ps(" Ops=");pi(st.t_f3);ps("\nExpr: ");pi(st.n_te);ps(" VR=");pi(st.t_f4);ps("\nData: ");pi(st.n_td);ps(" BI=");pi(st.t_f5);ps("\n");}
void tvt_state(void){ps("[TVT] Ti=");pi(st.n_ti);ps(" Bk=");pi(st.n_bk);ps(" Dm=");pi(st.n_dm);ps(" Te=");pi(st.n_te);ps(" Td=");pi(st.n_td);ps("\n");}
int main(void){
ps("=== Travel Tech Admin Demo ===\n\n");tvt_init();
ps("Smart itinerary...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;tvt_itinerary(t,c,149+(i*17),134+(i*14),114+(i*10),97+(i*6),2020+(i%5));}
ps("\nOnline booking...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;tvt_booking(t,c,138+(i*15),124+(i*12),106+(i*8),93+(i*5),2021+(i%4));}
ps("\nDestination management...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;tvt_destination(t,c,130+(i*13),116+(i*10),100+(i*7),89+(i*4),2022+(i%3));}
ps("\nTravel experience...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tvt_experience(t,c,122+(i*11),110+(i*9),96+(i*6),86+(i*3),2023+(i%2));}
ps("\nTravel data...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;tvt_data(t,c,116+(i*9),105+(i*7),92+(i*5),84+(i*3),2024);}
ps("\n");tvt_report();tvt_state();ps("\n=== Demo Complete ===\n");return 0;}
