/* lemur_admin: Lemur (Lemur catta) ring-tailed tree primate (v1.0)
 * Lemur forest, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, tail_cm, jump_ht, fur_idx, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,tail_cm,jp_ht,fr_idx,age_yr,active;} lemur_t;
typedef struct{int n_forest,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_tail,t_jp,t_fr;} lemur_state_t;
static lemur_t frs[N],frf[N-2],frb[N-4],frh[N-6],frm[N-6]; static lemur_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(lemur_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int tc,int jh,int fi,int ay){if(*cnt>=mx)return -1;lemur_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->tail_cm=tc;x->jp_ht=jh;x->fr_idx=fi;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[LEMUR] Lemur ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" tc=");pi(tc);ps(" jh=");pi(jh);ps(" fi=");pi(fi);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int lemur_init(void){if(init)return -1;st.n_forest=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_tail=0;st.t_jp=0;st.t_fr=0;for(int i=0;i<N;i++)frs[i].active=0;for(int i=0;i<N-2;i++)frf[i].active=0;for(int i=0;i<N-4;i++)frb[i].active=0;for(int i=0;i<N-6;i++)frh[i].active=0;for(int i=0;i<N-6;i++)frm[i].active=0;init=1;ps("[LEMUR] Lemur initialized\n");return 0;}
int lemur_forest(int lc,int bl,int bw,int tc,int jh,int fi,int ay){return add(frs,&st.n_forest,&st.t_ln,N,lc,bl,bw,tc,jh,fi,ay);}
int lemur_feeding(int lc,int bl,int bw,int tc,int jh,int fi,int ay){return add(frf,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,tc,jh,fi,ay);}
int lemur_breeding(int lc,int bl,int bw,int tc,int jh,int fi,int ay){return add(frb,&st.n_breed,&st.t_tail,N-4,lc,bl,bw,tc,jh,fi,ay);}
int lemur_health(int lc,int bl,int bw,int tc,int jh,int fi,int ay){return add(frh,&st.n_health,&st.t_jp,N-6,lc,bl,bw,tc,jh,fi,ay);}
int lemur_market(int lc,int bl,int bw,int tc,int jh,int fi,int ay){return add(frm,&st.n_mkt,&st.t_fr,N-6,lc,bl,bw,tc,jh,fi,ay);}
void lemur_report(void){ps("[LEMUR] Forest: ");pi(st.n_forest);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Tail=");pi(st.t_tail);ps("\nHealth: ");pi(st.n_health);ps(" Jp=");pi(st.t_jp);ps("\nMkt: ");pi(st.n_mkt);ps(" Fr=");pi(st.t_fr);ps("\n");}
void lemur_state(void){ps("[LEMUR] Forest=");pi(st.n_forest);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Health=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Lemur Admin Demo ===\n\n");lemur_init();
ps("Lemur forest...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=40+(i*3),bw=2000+(i*150),tc=50+(i*4),jh=150+(i*15),fi=(i%8)+1,ay=(i%12)+1;lemur_forest(lc,bl,bw,tc,jh,fi,ay);}
ps("\nLemur feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+2,bl=42+(i*2),bw=2100+(i*120),tc=52+(i*3),jh=155+(i*12),fi=(i%7)+1,ay=(i%10)+1;lemur_feeding(lc,bl,bw,tc,jh,fi,ay);}
ps("\nLemur breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=44+(i*2),bw=2200+(i*100),tc=54+(i*3),jh=145+(i*18),fi=(i%6)+1,ay=(i%8)+1;lemur_breeding(lc,bl,bw,tc,jh,fi,ay);}
ps("\nLemur health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=39+(i*4),bw=1900+(i*180),tc=48+(i*5),jh=140+(i*20),fi=(i%9)+1,ay=(i%7)+1;lemur_health(lc,bl,bw,tc,jh,fi,ay);}
ps("\nLemur market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+1,bl=46+(i*2),bw=2300+(i*80),tc=56+(i*2),jh=160+(i*10),fi=(i%5)+1,ay=(i%6)+1;lemur_market(lc,bl,bw,tc,jh,fi,ay);}
ps("\n");lemur_report();lemur_state();ps("\n=== Demo Complete ===\n");return 0;}
