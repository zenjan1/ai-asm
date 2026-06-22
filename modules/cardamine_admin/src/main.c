/* cardamine_admin: Cardamine management technology administration (v1.0)
 * Cardamine planning, cardamine execution, cardamine evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} card_t;
typedef struct{int n_cardp,n_card,n_card2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} card_state_t;
static card_t cards[N],carde[N-2],card2[N-4],cardac[N-6],cardam[N-6]; static card_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(card_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;card_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CARD] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int card_init(void){if(init)return -1;st.n_cardp=0;st.n_card=0;st.n_card2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)cards[i].active=0;for(int i=0;i<N-2;i++)carde[i].active=0;for(int i=0;i<N-4;i++)card2[i].active=0;for(int i=0;i<N-6;i++)cardac[i].active=0;for(int i=0;i<N-6;i++)cardam[i].active=0;init=1;ps("[CARD] Cardamine initialized\n");return 0;}
int card_planning(int t,int c,int a,int b,int d,int e,int y){return add(cards,&st.n_cardp,&st.t_f1,N,t,c,a,b,d,e,y);}
int card_execution(int t,int c,int a,int b,int d,int e,int y){return add(carde,&st.n_card,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int card_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(card2,&st.n_card2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int card_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cardac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int card_market(int t,int c,int a,int b,int d,int e,int y){return add(cardam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void card_report(void){ps("[CARD] Arp: ");pi(st.n_cardp);ps(" PCS=");pi(st.t_f1);ps("\nAre: ");pi(st.n_card);ps(" PCS=");pi(st.t_f2);ps("\nAr2: ");pi(st.n_card2);ps(" PCS=");pi(st.t_f3);ps("\nAac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void card_state(void){ps("[CARD] Arp=");pi(st.n_cardp);ps(" Ard=");pi(st.n_card);ps(" Ar2=");pi(st.n_card2);ps(" Aac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Cardamine Admin Demo ===\n\n");card_init();
ps("Cardamine planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;card_planning(t,c,1460+(i*17),1449+(i*14),1429+(i*10),1411+(i*6),2020+(i%5));}
ps("\nCardamine execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;card_execution(t,c,1449+(i*15),1438+(i*12),1420+(i*8),1407+(i*5),2021+(i%4));}
ps("\nCardamine evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;card_evaluation(t,c,1441+(i*13),1430+(i*10),1414+(i*7),1403+(i*4),2022+(i%3));}
ps("\nCardamine accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;card_accessory(t,c,1433+(i*11),1424+(i*9),1410+(i*6),1400+(i*3),2023+(i%2));}
ps("\nCardamine marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;card_market(t,c,1427+(i*9),1418+(i*7),1405+(i*5),1397+(i*3),2024);}
ps("\n");card_report();card_state();ps("\n=== Demo Complete ===\n");return 0;}
