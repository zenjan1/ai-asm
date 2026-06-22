/* carthamus_admin: Carthamus management technology administration (v1.0)
 * Carthamus planning, carthamus execution, carthamus evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cart_t;
typedef struct{int n_cartp,n_cart,n_cart2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} cart_state_t;
static cart_t carts[N],carte[N-2],cart2[N-4],cartac[N-6],cartam[N-6]; static cart_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]=45;v=-v;}if(v==0){b[i++]=48;}else{int s=i;while(v>0){b[i++]=48+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]=0;host_print(b);}
static int add(cart_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cart_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CART] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cart_init(void){if(init)return -1;st.n_cartp=0;st.n_cart=0;st.n_cart2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)carts[i].active=0;for(int i=0;i<N-2;i++)carte[i].active=0;for(int i=0;i<N-4;i++)cart2[i].active=0;for(int i=0;i<N-6;i++)cartac[i].active=0;for(int i=0;i<N-6;i++)cartam[i].active=0;init=1;ps("[CART] Carthamus initialized\n");return 0;}
int cart_planning(int t,int c,int a,int b,int d,int e,int y){return add(carts,&st.n_cartp,&st.t_f1,N,t,c,a,b,d,e,y);}
int cart_execution(int t,int c,int a,int b,int d,int e,int y){return add(carte,&st.n_cart,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cart_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(cart2,&st.n_cart2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cart_accessory(int t,int c,int a,int b,int d,int e,int y){return add(cartac,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cart_market(int t,int c,int a,int b,int d,int e,int y){return add(cartam,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cart_report(void){ps("[CART] Artp: ");pi(st.n_cartp);ps(" PCS=");pi(st.t_f1);ps("\nArte: ");pi(st.n_cart);ps(" PCS=");pi(st.t_f2);ps("\nArt2: ");pi(st.n_cart2);ps(" PCS=");pi(st.t_f3);ps("\nArtac: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void cart_state(void){ps("[CART] Artp=");pi(st.n_cartp);ps(" Art=");pi(st.n_cart);ps(" Art2=");pi(st.n_cart2);ps(" Artac=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Carthamus Admin Demo ===\n\n");cart_init();
ps("Carthamus planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cart_planning(t,c,1463+(i*17),1452+(i*14),1432+(i*10),1414+(i*6),2020+(i%5));}
ps("\nCarthamus execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cart_execution(t,c,1452+(i*15),1441+(i*12),1423+(i*8),1410+(i*5),2021+(i%4));}
ps("\nCarthamus evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cart_evaluation(t,c,1444+(i*13),1433+(i*10),1417+(i*7),1406+(i*4),2022+(i%3));}
ps("\nCarthamus accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cart_accessory(t,c,1436+(i*11),1427+(i*9),1413+(i*6),1403+(i*3),2023+(i%2));}
ps("\nCarthamus marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cart_market(t,c,1430+(i*9),1421+(i*7),1408+(i*5),1400+(i*3),2024);}
ps("\n");cart_report();cart_state();ps("\n=== Demo Complete ===\n");return 0;}
