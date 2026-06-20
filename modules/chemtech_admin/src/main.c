/* chemtech_admin: Chemical technology administration (v1.0)
 * Basic chemicals, fine chemicals, new chemical materials, chemical equipment, chemical safety
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} cht_t;
typedef struct{int n_bs,n_fn,n_nm,n_eq,n_sf,t_f1,t_f2,t_f3,t_f4,t_f5;} cht_state_t;
static cht_t bss[N],fns[N-2],nms[N-4],eqs[N-6],sfs[N-6]; static cht_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cht_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;cht_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[CHT] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int cht_init(void){if(init)return -1;st.n_bs=0;st.n_fn=0;st.n_nm=0;st.n_eq=0;st.n_sf=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)bss[i].active=0;for(int i=0;i<N-2;i++)fns[i].active=0;for(int i=0;i<N-4;i++)nms[i].active=0;for(int i=0;i<N-6;i++)eqs[i].active=0;for(int i=0;i<N-6;i++)sfs[i].active=0;init=1;ps("[CHT] Chemtech initialized\n");return 0;}
int cht_basic(int t,int c,int a,int b,int d,int e,int y){return add(bss,&st.n_bs,&st.t_f1,N,t,c,a,b,d,e,y);}
int cht_fine(int t,int c,int a,int b,int d,int e,int y){return add(fns,&st.n_fn,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int cht_newmaterial(int t,int c,int a,int b,int d,int e,int y){return add(nms,&st.n_nm,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int cht_equipment(int t,int c,int a,int b,int d,int e,int y){return add(eqs,&st.n_eq,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int cht_safety(int t,int c,int a,int b,int d,int e,int y){return add(sfs,&st.n_sf,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void cht_report(void){ps("[CHT] Basic: ");pi(st.n_bs);ps(" Ton=");pi(st.t_f1);ps("\nFine: ");pi(st.n_fn);ps(" Spec=");pi(st.t_f2);ps("\nNewMat: ");pi(st.n_nm);ps(" Poly=");pi(st.t_f3);ps("\nEquip: ");pi(st.n_eq);ps(" React=");pi(st.t_f4);ps("\nSafe: ");pi(st.n_sf);ps(" HSE=");pi(st.t_f5);ps("\n");}
void cht_state(void){ps("[CHT] Bs=");pi(st.n_bs);ps(" Fn=");pi(st.n_fn);ps(" Nm=");pi(st.n_nm);ps(" Eq=");pi(st.n_eq);ps(" Sf=");pi(st.n_sf);ps("\n");}
int main(void){
ps("=== Chemical Tech Admin Demo ===\n\n");cht_init();
ps("Basic chemicals...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;cht_basic(t,c,161+(i*17),146+(i*14),126+(i*10),109+(i*6),2020+(i%5));}
ps("\nFine chemicals...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;cht_fine(t,c,150+(i*15),136+(i*12),118+(i*8),105+(i*5),2021+(i%4));}
ps("\nNew chemical materials...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;cht_newmaterial(t,c,142+(i*13),128+(i*10),112+(i*7),101+(i*4),2022+(i%3));}
ps("\nChemical equipment...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cht_equipment(t,c,134+(i*11),122+(i*9),108+(i*6),98+(i*3),2023+(i%2));}
ps("\nChemical safety...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;cht_safety(t,c,128+(i*9),117+(i*7),104+(i*5),96+(i*3),2024);}
ps("\n");cht_report();cht_state();ps("\n=== Demo Complete ===\n");return 0;}
