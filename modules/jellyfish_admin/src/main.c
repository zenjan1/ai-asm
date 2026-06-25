/* jellyfish_admin: Jellyfish (Medusozoa) marine invertebrate (v1.0)
 * Jellyfish ocean, feeding, breeding, health, market
 * Features: bell_dia_cm, body_wt_g, tentacle_cm, drift_speed, sting_idx, age_week
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bl_dia,bdy_wt,ten_cm,dr_sp,st_idx,age_wk,active;} jlf_t;
typedef struct{int n_ocean,n_feed,n_breed,n_health,n_mkt,t_dia,t_wt,t_ten,t_dr,t_st;} jlf_state_t;
static jlf_t ocn[N],ocf[N-2],ocb[N-4],och[N-6],ocm[N-6]; static jlf_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(jlf_t*a,int*cnt,int*sum,int mx,int lc,int bd,int bw,int tc,int ds,int si,int aw){if(*cnt>=mx)return -1;jlf_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bl_dia=bd;x->bdy_wt=bw;x->ten_cm=tc;x->dr_sp=ds;x->st_idx=si;x->age_wk=aw;x->active=1;*sum+=bd;(*cnt)++;ps("[JLF] Jellyfish ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bd=");pi(bd);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" ds=");pi(ds);ps(" si=");pi(si);ps(" aw=");pi(aw);ps("\n");return *cnt-1;}
int jellyfish_init(void){if(init)return -1;st.n_ocean=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_dia=0;st.t_wt=0;st.t_ten=0;st.t_dr=0;st.t_st=0;for(int i=0;i<N;i++)ocn[i].active=0;for(int i=0;i<N-2;i++)ocf[i].active=0;for(int i=0;i<N-4;i++)ocb[i].active=0;for(int i=0;i<N-6;i++)och[i].active=0;for(int i=0;i<N-6;i++)ocm[i].active=0;init=1;ps("[JLF] Jellyfish initialized\n");return 0;}
int jellyfish_ocean(int lc,int bd,int bw,int tc,int ds,int si,int aw){return add(ocn,&st.n_ocean,&st.t_dia,N,lc,bd,bw,tc,ds,si,aw);}
int jellyfish_feeding(int lc,int bd,int bw,int tc,int ds,int si,int aw){return add(ocf,&st.n_feed,&st.t_wt,N-2,lc,bd,bw,tc,ds,si,aw);}
int jellyfish_breeding(int lc,int bd,int bw,int tc,int ds,int si,int aw){return add(ocb,&st.n_breed,&st.t_ten,N-4,lc,bd,bw,tc,ds,si,aw);}
int jellyfish_health(int lc,int bd,int bw,int tc,int ds,int si,int aw){return add(och,&st.n_health,&st.t_dr,N-6,lc,bd,bw,tc,ds,si,aw);}
int jellyfish_market(int lc,int bd,int bw,int tc,int ds,int si,int aw){return add(ocm,&st.n_mkt,&st.t_st,N-6,lc,bd,bw,tc,ds,si,aw);}
void jellyfish_report(void){ps("[JLF] Ocean: ");pi(st.n_ocean);ps(" Dia=");pi(st.t_dia);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Ten=");pi(st.t_ten);ps("\nHealth: ");pi(st.n_health);ps(" Dr=");pi(st.t_dr);ps("\nMkt: ");pi(st.n_mkt);ps(" St=");pi(st.t_st);ps("\n");}
void jellyfish_state(void){ps("[JLF] Ocean=");pi(st.n_ocean);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Jellyfish Admin Demo ===\n\n");jellyfish_init();
ps("Jellyfish ocean...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bd=20+(i*3),bw=100+(i*20),tc=30+(i*5),ds=1+(i*1),si=(i%8)+1,aw=(i%12)+1;jellyfish_ocean(lc,bd,bw,tc,ds,si,aw);}
ps("\nJellyfish feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bd=22+(i*2),bw=110+(i*15),tc=32+(i*4),ds=1+(i*1),si=(i%7)+1,aw=(i%10)+1;jellyfish_feeding(lc,bd,bw,tc,ds,si,aw);}
ps("\nJellyfish breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bd=24+(i*2),bw=120+(i*12),tc=34+(i*3),ds=1+(i*1),si=(i%6)+1,aw=(i%8)+1;jellyfish_breeding(lc,bd,bw,tc,ds,si,aw);}
ps("\nJellyfish health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bd=19+(i*4),bw=95+(i*25),tc=28+(i*6),ds=0+(i*2),si=(i%9)+1,aw=(i%6)+1;jellyfish_health(lc,bd,bw,tc,ds,si,aw);}
ps("\nJellyfish market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bd=26+(i*2),bw=130+(i*10),tc=36+(i*3),ds=2+(i*1),si=(i%5)+1,aw=(i%5)+1;jellyfish_market(lc,bd,bw,tc,ds,si,aw);}
ps("\n");jellyfish_report();jellyfish_state();ps("\n=== Demo Complete ===\n");return 0;}
