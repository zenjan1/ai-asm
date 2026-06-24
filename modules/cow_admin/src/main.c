/* cow_admin: Cow (Bos taurus) cattle livestock management (v1.0)
 * Cow feeding, milking, breeding, veterinary, market
 * Features: body_wt_kg, milk_yield_l, fat_pct, feed_kg, calf_count, breed_idx
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_wt,milk_yd,fat_pct,feed_kg,calf_ct,breed_idx,active;} cow_t;
typedef struct{int n_feed,n_milk,n_breed,n_vet,n_mkt,t_wt,t_milk,t_fat,t_feed,t_calf;} cow_state_t;
static cow_t cowfd[N],cowmk[N-2],cowbr[N-4],cowvt[N-6],cowmt[N-6]; static cow_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(cow_t*a,int*cnt,int*sum,int mx,int lc,int bw,int my,int fp,int fk,int cc,int bi){if(*cnt>=mx)return -1;cow_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_wt=bw;x->milk_yd=my;x->fat_pct=fp;x->feed_kg=fk;x->calf_ct=cc;x->breed_idx=bi;x->active=1;*sum+=bw;(*cnt)++;ps("[COW] Cow ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bw=");pi(bw);ps(" my=");pi(my);ps(" fp=");pi(fp);ps(" fk=");pi(fk);ps(" cc=");pi(cc);ps(" bi=");pi(bi);ps("\n");return *cnt-1;}
int cow_init(void){if(init)return -1;st.n_feed=0;st.n_milk=0;st.n_breed=0;st.n_vet=0;st.n_mkt=0;st.t_wt=0;st.t_milk=0;st.t_fat=0;st.t_feed=0;st.t_calf=0;for(int i=0;i<N;i++)cowfd[i].active=0;for(int i=0;i<N-2;i++)cowmk[i].active=0;for(int i=0;i<N-4;i++)cowbr[i].active=0;for(int i=0;i<N-6;i++)cowvt[i].active=0;for(int i=0;i<N-6;i++)cowmt[i].active=0;init=1;ps("[COW] Cow initialized\n");return 0;}
int cow_feeding(int lc,int bw,int my,int fp,int fk,int cc,int bi){return add(cowfd,&st.n_feed,&st.t_wt,N,lc,bw,my,fp,fk,cc,bi);}
int cow_milking(int lc,int bw,int my,int fp,int fk,int cc,int bi){return add(cowmk,&st.n_milk,&st.t_milk,N-2,lc,bw,my,fp,fk,cc,bi);}
int cow_breeding(int lc,int bw,int my,int fp,int fk,int cc,int bi){return add(cowbr,&st.n_breed,&st.t_fat,N-4,lc,bw,my,fp,fk,cc,bi);}
int cow_veterinary(int lc,int bw,int my,int fp,int fk,int cc,int bi){return add(cowvt,&st.n_vet,&st.t_feed,N-6,lc,bw,my,fp,fk,cc,bi);}
int cow_market(int lc,int bw,int my,int fp,int fk,int cc,int bi){return add(cowmt,&st.n_mkt,&st.t_calf,N-6,lc,bw,my,fp,fk,cc,bi);}
void cow_report(void){ps("[COW] Feed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nMilk: ");pi(st.n_milk);ps(" Yield=");pi(st.t_milk);ps("\nBreed: ");pi(st.n_breed);ps(" Fat=");pi(st.t_fat);ps("\nVet: ");pi(st.n_vet);ps(" Feed=");pi(st.t_feed);ps("\nMkt: ");pi(st.n_mkt);ps(" Calf=");pi(st.t_calf);ps("\n");}
void cow_state(void){ps("[COW] Feed=");pi(st.n_feed);ps(" Milk=");pi(st.n_milk);ps(" Breed=");pi(st.n_breed);ps(" Vet=");pi(st.n_vet);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Cow Admin Demo ===\n\n");cow_init();
/* 1=pasture 2=barn 3=dairy 4=feedlot 5=market */
ps("Cow feeding...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bw=400+(i*30),my=15+(i*3),fp=3+(i%4),fk=10+(i*2),cc=(i%4),bi=(i%5)+1;cow_feeding(lc,bw,my,fp,fk,cc,bi);}
ps("\nCow milking...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bw=420+(i*25),my=17+(i*3),fp=4+(i%3),fk=11+(i*2),cc=(i%3),bi=(i%4)+1;cow_milking(lc,bw,my,fp,fk,cc,bi);}
ps("\nCow breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bw=440+(i*20),my=19+(i*2),fp=4+(i%3),fk=12+(i*2),cc=1+(i%3),bi=(i%3)+1;cow_breeding(lc,bw,my,fp,fk,cc,bi);}
ps("\nCow veterinary...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bw=380+(i*35),my=13+(i*4),fp=3+(i%5),fk=9+(i*3),cc=(i%5),bi=(i%6)+1;cow_veterinary(lc,bw,my,fp,fk,cc,bi);}
ps("\nCow market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bw=460+(i*18),my=21+(i*2),fp=5+(i%2),fk=13+(i*2),cc=2+(i%2),bi=(i%3)+3;cow_market(lc,bw,my,fp,fk,cc,bi);}
ps("\n");cow_report();cow_state();ps("\n=== Demo Complete ===\n");return 0;}
