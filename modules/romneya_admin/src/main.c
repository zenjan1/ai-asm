/* romneya_admin: Romneya Coulteri (California tree poppy) administration (v1.0)
 * Romneya planning, execution, evaluation, rhizome management, native habitat marketing
 * Features: rhizome spread control, bloom tracking, drought adaptation, pollinator habitat
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,cultivar,height,spread,blooms,water,sun,fragrance,active;} rom_t;
typedef struct{int n_plan,n_exec,n_eval,n_rhiz,n_mkt,t_ht,t_spread,t_blooms,t_water,t_frag;} rom_state_t;
static rom_t romps[N],romes[N-2],romvs[N-4],romrs[N-6],romms[N-6]; static rom_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(rom_t*a,int*cnt,int*sum,int mx,int cv,int ht,int sp,int bl,int wa,int su,int fr){if(*cnt>=mx)return -1;rom_t*x=&a[*cnt];x->id=*cnt;x->cultivar=cv;x->height=ht;x->spread=sp;x->blooms=bl;x->water=wa;x->sun=su;x->fragrance=fr;x->active=1;*sum+=ht;(*cnt)++;ps("[ROM] Romneya ");pi(*cnt-1);ps(" cv=");pi(cv);ps(" ht=");pi(ht);ps(" sp=");pi(sp);ps(" bl=");pi(bl);ps(" wa=");pi(wa);ps("\n");return *cnt-1;}
int rom_init(void){if(init)return -1;st.n_plan=0;st.n_exec=0;st.n_eval=0;st.n_rhiz=0;st.n_mkt=0;st.t_ht=0;st.t_spread=0;st.t_blooms=0;st.t_water=0;st.t_frag=0;for(int i=0;i<N;i++)romps[i].active=0;for(int i=0;i<N-2;i++)romes[i].active=0;for(int i=0;i<N-4;i++)romvs[i].active=0;for(int i=0;i<N-6;i++)romrs[i].active=0;for(int i=0;i<N-6;i++)romms[i].active=0;init=1;ps("[ROM] Romneya (California tree poppy) initialized\n");return 0;}
/* 1=Coulteri 2=Austrocalifornica 3=Hallii 4=Californica 5=Fremontii */
int rom_planning(int cv,int ht,int sp,int bl,int wa,int su,int fr){return add(romps,&st.n_plan,&st.t_ht,N,cv,ht,sp,bl,wa,su,fr);}
int rom_execution(int cv,int ht,int sp,int bl,int wa,int su,int fr){return add(romes,&st.n_exec,&st.t_spread,N-2,cv,ht,sp,bl,wa,su,fr);}
int rom_evaluation(int cv,int ht,int sp,int bl,int wa,int su,int fr){return add(romvs,&st.n_eval,&st.t_blooms,N-4,cv,ht,sp,bl,wa,su,fr);}
int rom_rhizome(int cv,int ht,int sp,int bl,int wa,int su,int fr){return add(romrs,&st.n_rhiz,&st.t_water,N-6,cv,ht,sp,bl,wa,su,fr);}
int rom_market(int cv,int ht,int sp,int bl,int wa,int su,int fr){return add(romms,&st.n_mkt,&st.t_frag,N-6,cv,ht,sp,bl,wa,su,fr);}
void rom_report(void){ps("[ROM] Plan: ");pi(st.n_plan);ps(" ht=");pi(st.t_ht);ps("\nExec: ");pi(st.n_exec);ps(" spread=");pi(st.t_spread);ps("\nEval: ");pi(st.n_eval);ps(" blooms=");pi(st.t_blooms);ps("\nRhiz: ");pi(st.n_rhiz);ps(" water=");pi(st.t_water);ps("\nMkt: ");pi(st.n_mkt);ps(" frag=");pi(st.t_frag);ps("\n");}
void rom_state(void){ps("[ROM] Plan=");pi(st.n_plan);ps(" Exec=");pi(st.n_exec);ps(" Eval=");pi(st.n_eval);ps(" Rhiz=");pi(st.n_rhiz);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Romneya (California Tree Poppy) Admin Demo ===\n\n");rom_init();
ps("Romneya planning (site selection)...\n");
for(int i=0;i<N;i++){int cv=(i%5)+1;rom_planning(cv,180+(i*15),120+(i*10),8+(i%6),2+(i%3),(i%4)+1,(i%3)+1);}
ps("\nRomneya execution (planting)...\n");
for(int i=0;i<N-2;i++){int cv=(i%4)+2;rom_execution(cv,195+(i*12),135+(i*9),10+(i%5),3+(i%3),(i%3)+2,(i%3)+1);}
ps("\nRomneya evaluation (bloom check)...\n");
for(int i=0;i<N-4;i++){int cv=(i%3)+1;rom_evaluation(cv,210+(i*10),150+(i*8),12+(i%4),2+(i%2),(i%3)+2,(i%2)+2);}
ps("\nRomneya rhizome control...\n");
for(int i=0;i<N-6;i++){int cv=(i%5)+1;rom_rhizome(cv,160+(i*14),100+(i*11),6+(i%5),1+(i%3),(i%4)+1,1);}
ps("\nRomneya native habitat marketing...\n");
for(int i=0;i<N-6;i++){int cv=(i%4)+1;rom_market(cv,220+(i*11),160+(i*8),14+(i%4),3+(i%2),(i%3)+2,(i%3)+1);}
ps("\n");rom_report();rom_state();ps("\n=== Demo Complete ===\n");return 0;}
