/* buffalo_admin: Buffalo (Bison bison) herd and ranch management (v1.0)
 * Buffalo grazing, breeding, veterinary, roundup, market
 * Features: herd count, avg weight, grazing acres, milk yield, hide quality, roundup season
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,herd_ct,avg_wt,graze_ac,milk_yd,hide_qual,rnd_sn,active;} buff_t;
typedef struct{int n_graze,n_breed,n_vet,n_round,n_mkt,t_herd,t_wt,t_graze,t_milk,t_hide;} buff_state_t;
static buff_t buffps[N],buffbs[N-2],buffvs[N-4],buffrs[N-6],buffms[N-6]; static buff_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(buff_t*a,int*cnt,int*sum,int mx,int lc,int hc,int aw,int ga,int my,int hq,int rs){if(*cnt>=mx)return -1;buff_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->herd_ct=hc;x->avg_wt=aw;x->graze_ac=ga;x->milk_yd=my;x->hide_qual=hq;x->rnd_sn=rs;x->active=1;*sum+=hc;(*cnt)++;ps("[BUFF] Buffalo ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" hc=");pi(hc);ps(" aw=");pi(aw);ps(" ga=");pi(ga);ps(" my=");pi(my);ps(" hq=");pi(hq);ps(" rs=");pi(rs);ps("\n");return *cnt-1;}
int buff_init(void){if(init)return -1;st.n_graze=0;st.n_breed=0;st.n_vet=0;st.n_round=0;st.n_mkt=0;st.t_herd=0;st.t_wt=0;st.t_graze=0;st.t_milk=0;st.t_hide=0;for(int i=0;i<N;i++)buffps[i].active=0;for(int i=0;i<N-2;i++)buffbs[i].active=0;for(int i=0;i<N-4;i++)buffvs[i].active=0;for(int i=0;i<N-6;i++)buffrs[i].active=0;for(int i=0;i<N-6;i++)buffms[i].active=0;init=1;ps("[BUFF] Buffalo initialized\n");return 0;}
int buff_grazing(int lc,int hc,int aw,int ga,int my,int hq,int rs){return add(buffps,&st.n_graze,&st.t_herd,N,lc,hc,aw,ga,my,hq,rs);}
int buff_breeding(int lc,int hc,int aw,int ga,int my,int hq,int rs){return add(buffbs,&st.n_breed,&st.t_wt,N-2,lc,hc,aw,ga,my,hq,rs);}
int buff_veterinary(int lc,int hc,int aw,int ga,int my,int hq,int rs){return add(buffvs,&st.n_vet,&st.t_graze,N-4,lc,hc,aw,ga,my,hq,rs);}
int buff_roundup(int lc,int hc,int aw,int ga,int my,int hq,int rs){return add(buffrs,&st.n_round,&st.t_milk,N-6,lc,hc,aw,ga,my,hq,rs);}
int buff_market(int lc,int hc,int aw,int ga,int my,int hq,int rs){return add(buffms,&st.n_mkt,&st.t_hide,N-6,lc,hc,aw,ga,my,hq,rs);}
void buff_report(void){ps("[BUFF] Graze: ");pi(st.n_graze);ps(" Herd=");pi(st.t_herd);ps("\nBreed: ");pi(st.n_breed);ps(" Wt=");pi(st.t_wt);ps("\nVet: ");pi(st.n_vet);ps(" Graze=");pi(st.t_graze);ps("\nRound: ");pi(st.n_round);ps(" Milk=");pi(st.t_milk);ps("\nMkt: ");pi(st.n_mkt);ps(" Hide=");pi(st.t_hide);ps("\n");}
void buff_state(void){ps("[BUFF] Graze=");pi(st.n_graze);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Round=");pi(st.n_round);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Buffalo Admin Demo ===\n\n");buff_init();
/* 1=prairie 2=ranch 3=reservation 4=wilderness 5=farm */
ps("Buffalo grazing...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,hc=20+(i*5),aw=400+(i*30),ga=50+(i*10),my=5+(i*2),hq=(i%5)+1,rs=(i%4)+1;buff_grazing(lc,hc,aw,ga,my,hq,rs);}
ps("\nBuffalo breeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,hc=25+(i*4),aw=450+(i*25),ga=60+(i*8),my=6+(i*2),hq=(i%4)+2,rs=(i%4)+1;buff_breeding(lc,hc,aw,ga,my,hq,rs);}
ps("\nBuffalo veterinary...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,hc=30+(i*3),aw=500+(i*20),ga=70+(i*6),my=7+(i*2),hq=(i%3)+3,rs=(i%3)+2;buff_veterinary(lc,hc,aw,ga,my,hq,rs);}
ps("\nBuffalo roundup...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,hc=15+(i*6),aw=350+(i*35),ga=40+(i*12),my=4+(i*3),hq=(i%5)+1,rs=(i%4)+1;buff_roundup(lc,hc,aw,ga,my,hq,rs);}
ps("\nBuffalo market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,hc=35+(i*3),aw=550+(i*15),ga=80+(i*5),my=8+(i*2),hq=(i%3)+3,rs=(i%3)+2;buff_market(lc,hc,aw,ga,my,hq,rs);}
ps("\n");buff_report();buff_state();ps("\n=== Demo Complete ===\n");return 0;}
