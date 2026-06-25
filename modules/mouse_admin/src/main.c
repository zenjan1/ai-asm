/* mouse_admin: Mouse (Mus musculus) house small rodent (v1.0)
 * Mouse house, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, run_speed, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,rn_sp,fr_idx,age_yr,active;} mouse_t;
typedef struct{int n_house,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_rn,t_fr;} mouse_state_t;
static mouse_t hse[N],hsf[N-2],hsb[N-4],hsh[N-6],hsm[N-6]; static mouse_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(mouse_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int rs,int fi,int ay){if(*cnt>=mx)return -1;mouse_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->rn_sp=rs;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[MOUSE] Mouse ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" rs=");pi(rs);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int mouse_init(void){if(init)return -1;st.n_house=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_rn=0;st.t_fr=0;for(int i=0;i<N;i++)hse[i].active=0;for(int i=0;i<N-2;i++)hsf[i].active=0;for(int i=0;i<N-4;i++)hsb[i].active=0;for(int i=0;i<N-6;i++)hsh[i].active=0;for(int i=0;i<N-6;i++)hsm[i].active=0;init=1;ps("[MOUSE] Mouse initialized\n");return 0;}
int mouse_house(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(hse,&st.n_house,&st.t_ln,N,lc,bl,bw,tc,rs,fi,ay);}
int mouse_feeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(hsf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,rs,fi,ay);}
int mouse_breeding(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(hsb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,rs,fi,ay);}
int mouse_health(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(hsh,&st.n_health,&st.t_rn,N-6,lc,bl,bw,tc,rs,fi,ay);}
int mouse_market(int lc,int bl,int bw,int tc,int rs,int fi,int ay){return add(hsm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,rs,fi,ay);}
void mouse_report(void){ps("[MOUSE] House: ");pi(st.n_house);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Rn=");pi(st.t_rn);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void mouse_state(void){ps("[MOUSE] House=");pi(st.n_house);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Mouse Admin Demo ===\n\n");mouse_init();
ps("Mouse house...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=8+(i*1),bw=25+(i*3),tc=7+(i*1),rs=4+(i*1),fi=(i%6)+1,ay=(i%2)+1;mouse_house(lc,bl,bw,tc,rs,fi,ay);}
ps("\nMouse feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=9+(i*1),bw=26+(i*2),tc=7+(i*1),rs=4+(i*1),fi=(i%5)+1,ay=(i%2)+1;mouse_feeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nMouse breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=10+(i*1),bw=27+(i*2),tc=8+(i*1),rs=3+(i*1),fi=(i%4)+1,ay=(i%2)+1;mouse_breeding(lc,bl,bw,tc,rs,fi,ay);}
ps("\nMouse health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=7+(i*1),bw=24+(i*3),tc=6+(i*1),rs=3+(i*2),fi=(i%7)+1,ay=(i%2)+1;mouse_health(lc,bl,bw,tc,rs,fi,ay);}
ps("\nMouse market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=11+(i*1),bw=28+(i*2),tc=8+(i*1),rs=5+(i*1),fi=(i%3)+1,ay=(i%1)+1;mouse_market(lc,bl,bw,tc,rs,fi,ay);}
ps("\n");mouse_report();mouse_state();ps("\n=== Demo Complete ===\n");return 0;}
