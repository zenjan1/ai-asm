/* tourtech_admin: Tourism technology administration (v1.0)
 * Travel agencies, hotels, attractions, travel services, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} trt_t;
typedef struct{int n_ta,n_ht,n_at,n_ts,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} trt_state_t;
static trt_t tas[N],hts[N-2],ats[N-4],tss[N-6],mks[N-6]; static trt_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(trt_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;trt_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[TRT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int trt_init(void){if(init)return -1;st.n_ta=0;st.n_ht=0;st.n_at=0;st.n_ts=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)tas[i].active=0;for(int i=0;i<N-2;i++)hts[i].active=0;for(int i=0;i<N-4;i++)ats[i].active=0;for(int i=0;i<N-6;i++)tss[i].active=0;for(int i=0;i<N-6;i++)mks[i].active=0;init=1;ps("[TRT] Tourtech initialized\n");return 0;}
int trt_agency(int t,int c,int a,int b,int d,int e,int y){return add(tas,&st.n_ta,&st.t_f1,N,t,c,a,b,d,e,y);}
int trt_hotel(int t,int c,int a,int b,int d,int e,int y){return add(hts,&st.n_ht,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int trt_attraction(int t,int c,int a,int b,int d,int e,int y){return add(ats,&st.n_at,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int trt_service(int t,int c,int a,int b,int d,int e,int y){return add(tss,&st.n_ts,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int trt_market(int t,int c,int a,int b,int d,int e,int y){return add(mks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void trt_report(void){ps("[TRT] Ta: ");pi(st.n_ta);ps(" PCS=");pi(st.t_f1);ps("\nHt: ");pi(st.n_ht);ps(" Rooms=");pi(st.t_f2);ps("\nAt: ");pi(st.n_at);ps(" PCS=");pi(st.t_f3);ps("\nTs: ");pi(st.n_ts);ps(" Jobs=");pi(st.t_f4);ps("\nMkt: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void trt_state(void){ps("[TRT] Ta=");pi(st.n_ta);ps(" Ht=");pi(st.n_ht);ps(" At=");pi(st.n_at);ps(" Ts=");pi(st.n_ts);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Tourism Tech Admin Demo ===\n\n");trt_init();
ps("Travel agencies...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;trt_agency(t,c,239+(i*17),224+(i*14),204+(i*10),186+(i*6),2020+(i%5));}
ps("\nHotels...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;trt_hotel(t,c,228+(i*15),214+(i*12),196+(i*8),183+(i*5),2021+(i%4));}
ps("\nAttractions...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;trt_attraction(t,c,220+(i*13),206+(i*10),190+(i*7),179+(i*4),2022+(i%3));}
ps("\nTravel services...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trt_service(t,c,212+(i*11),200+(i*9),186+(i*6),176+(i*3),2023+(i%2));}
ps("\nTourism marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;trt_market(t,c,206+(i*9),195+(i*7),182+(i*5),174+(i*3),2024);}
ps("\n");trt_report();trt_state();ps("\n=== Demo Complete ===\n");return 0;}
