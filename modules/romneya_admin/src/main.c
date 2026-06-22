/* romneya_admin: Romneya management technology administration (v1.0)
 * Romneya planning, romneya execution, romneya evaluation, accessories, marketing
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,type,cat,f1,f2,f3,f4,year,active;} rom_t;
typedef struct{int n_romp,n_rome,n_rom2,n_ac,n_mk,t_f1,t_f2,t_f3,t_f4,t_f5;} rom_state_t;
static rom_t romps[N],romss[N-2],romvss[N-4],romas[N-6],rommks[N-6]; static rom_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rom_t*a,int*cnt,int*sum,int mx,int t,int c,int a1,int a2,int a3,int a4,int y){if(*cnt>=mx)return -1;rom_t*x=&a[*cnt];x->id=*cnt;x->type=t;x->cat=c;x->f1=a1;x->f2=a2;x->f3=a3;x->f4=a4;x->year=y;x->active=1;*sum+=a1;(*cnt)++;ps("[ROM] Sub ");pi(*cnt-1);ps(" t=");pi(t);ps(" c=");pi(c);ps(" a=");pi(a1);ps(" b=");pi(a2);ps(" d=");pi(a3);ps(" e=");pi(a4);ps("\n");return *cnt-1;}
int rom_init(void){if(init)return -1;st.n_romp=0;st.n_rome=0;st.n_rom2=0;st.n_ac=0;st.n_mk=0;st.t_f1=0;st.t_f2=0;st.t_f3=0;st.t_f4=0;st.t_f5=0;for(int i=0;i<N;i++)romps[i].active=0;for(int i=0;i<N-2;i++)romss[i].active=0;for(int i=0;i<N-4;i++)romvss[i].active=0;for(int i=0;i<N-6;i++)romas[i].active=0;for(int i=0;i<N-6;i++)rommks[i].active=0;init=1;ps("[ROM] Romneya initialized\n");return 0;}
int rom_planning(int t,int c,int a,int b,int d,int e,int y){return add(romps,&st.n_romp,&st.t_f1,N,t,c,a,b,d,e,y);}
int rom_execution(int t,int c,int a,int b,int d,int e,int y){return add(romss,&st.n_rome,&st.t_f2,N-2,t,c,a,b,d,e,y);}
int rom_evaluation(int t,int c,int a,int b,int d,int e,int y){return add(romvss,&st.n_rom2,&st.t_f3,N-4,t,c,a,b,d,e,y);}
int rom_accessory(int t,int c,int a,int b,int d,int e,int y){return add(romas,&st.n_ac,&st.t_f4,N-6,t,c,a,b,d,e,y);}
int rom_market(int t,int c,int a,int b,int d,int e,int y){return add(rommks,&st.n_mk,&st.t_f5,N-6,t,c,a,b,d,e,y);}
void rom_report(void){ps("[ROM] Rompp: ");pi(st.n_romp);ps(" PCS=");pi(st.t_f1);ps("\nRome: ");pi(st.n_rome);ps(" PCS=");pi(st.t_f2);ps("\nRomv: ");pi(st.n_rom2);ps(" PCS=");pi(st.t_f3);ps("\nRomc: ");pi(st.n_ac);ps(" PCS=");pi(st.t_f4);ps("\nMk: ");pi(st.n_mk);ps(" USD=");pi(st.t_f5);ps("\n");}
void rom_state(void){ps("[ROM] Rompp=");pi(st.n_romp);ps(" Rome=");pi(st.n_rome);ps(" Romv=");pi(st.n_rom2);ps(" Romc=");pi(st.n_ac);ps(" Mk=");pi(st.n_mk);ps("\n");}
int main(void){
ps("=== Romneya Admin Demo ===\n\n");rom_init();
ps("Romneya planning...\n");for(int i=0;i<N;i++){int t=(i%5)+1,c=(i%4)+1;rom_planning(t,c,1181+(i*17),1170+(i*14),1150+(i*10),1132+(i*6),2020+(i%5));}
ps("\nRomneya execution...\n");for(int i=0;i<N-2;i++){int t=(i%4)+1,c=(i%5)+1;rom_execution(t,c,1170+(i*15),1159+(i*12),1141+(i*8),1128+(i*5),2021+(i%4));}
ps("\nRomneya evaluation...\n");for(int i=0;i<N-4;i++){int t=(i%4)+1,c=(i%5)+1;rom_evaluation(t,c,1162+(i*13),1151+(i*10),1135+(i*7),1124+(i*4),2022+(i%3));}
ps("\nRomneya accessories...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rom_accessory(t,c,1154+(i*11),1145+(i*9),1131+(i*6),1121+(i*3),2023+(i%2));}
ps("\nRomneya marketing...\n");for(int i=0;i<N-6;i++){int t=(i%4)+1,c=(i%5)+1;rom_market(t,c,1148+(i*9),1139+(i*7),1126+(i*5),1118+(i*3),2024);}
ps("\n");rom_report();rom_state();ps("\n=== Demo Complete ===\n");return 0;}
