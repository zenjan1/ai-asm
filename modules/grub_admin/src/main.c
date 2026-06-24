/* grub_admin: Grub (Scarabaeidae larva) beetle larva insect (v1.0)
 * Grub soil, feeding, breeding, health, market
 * Features: body_len_cm, body_wt_g, segment_ct, curl_idx, chitin_phase, age_year
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char*);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define N 16
typedef struct{int id,location,bdy_ln,bdy_wt,seg_ct,curl_idx,cht_ph,age_yr,active;} grub_t;
typedef struct{int n_soil,n_feed,n_breed,n_health,n_mkt,t_ln,t_wt,t_seg,t_curl,t_cht;} grub_state_t;
static grub_t gpl[N],gfd[N-2],gbr[N-4],ghl[N-6],gmk[N-6]; static grub_state_t st; static int init;
static void ps(const char*s){host_print(s);} static void pi(int v){char b[32];int i=0;if(v<0){b[i++]='-';v=-v;}if(v==0){b[i++]='0';}else{int s=i;while(v>0){b[i++]='0'+(v%10);v/=10;}int e=i-1;while(s<e){char t=b[s];b[s]=b[e];b[e]=t;s++;e--;}}b[i]='\0';host_print(b);}
static int add(grub_t*a,int*cnt,int*sum,int mx,int lc,int bl,int bw,int sc,int ci,int cp,int ay){if(*cnt>=mx)return -1;grub_t*x=&a[*cnt];x->id=*cnt;x->location=lc;x->bdy_ln=bl;x->bdy_wt=bw;x->seg_ct=sc;x->curl_idx=ci;x->cht_ph=cp;x->age_yr=ay;x->active=1;*sum+=bl;(*cnt)++;ps("[GRUB] Grub ");pi(*cnt-1);ps(" lc=");pi(lc);ps(" bl=");pi(bl);ps(" bw=");pi(bw);ps(" sc=");pi(sc);ps(" ci=");pi(ci);ps(" cp=");pi(cp);ps(" ay=");pi(ay);ps("\n");return *cnt-1;}
int grub_init(void){if(init)return -1;st.n_soil=0;st.n_feed=0;st.n_breed=0;st.n_health=0;st.n_mkt=0;st.t_ln=0;st.t_wt=0;st.t_seg=0;st.t_curl=0;st.t_cht=0;for(int i=0;i<N;i++)gpl[i].active=0;for(int i=0;i<N-2;i++)gfd[i].active=0;for(int i=0;i<N-4;i++)gbr[i].active=0;for(int i=0;i<N-6;i++)ghl[i].active=0;for(int i=0;i<N-6;i++)gmk[i].active=0;init=1;ps("[GRUB] Grub initialized\n");return 0;}
int grub_soil(int lc,int bl,int bw,int sc,int ci,int cp,int ay){return add(gpl,&st.n_soil,&st.t_ln,N,lc,bl,bw,sc,ci,cp,ay);}
int grub_feeding(int lc,int bl,int bw,int sc,int ci,int cp,int ay){return add(gfd,&st.n_feed,&st.t_wt,N-2,lc,bl,bw,sc,ci,cp,ay);}
int grub_breeding(int lc,int bl,int bw,int sc,int ci,int cp,int ay){return add(gbr,&st.n_breed,&st.t_seg,N-4,lc,bl,bw,sc,ci,cp,ay);}
int grub_health(int lc,int bl,int bw,int sc,int ci,int cp,int ay){return add(ghl,&st.n_health,&st.t_curl,N-6,lc,bl,bw,sc,ci,cp,ay);}
int grub_market(int lc,int bl,int bw,int sc,int ci,int cp,int ay){return add(gmk,&st.n_mkt,&st.t_cht,N-6,lc,bl,bw,sc,ci,cp,ay);}
void grub_report(void){ps("[GRUB] Soil: ");pi(st.n_soil);ps(" Ln=");pi(st.t_ln);ps("\nFeed: ");pi(st.n_feed);ps(" Wt=");pi(st.t_wt);ps("\nBreed: ");pi(st.n_breed);ps(" Seg=");pi(st.t_seg);ps("\nHlth: ");pi(st.n_health);ps(" Curl=");pi(st.t_curl);ps("\nMkt: ");pi(st.n_mkt);ps(" Cht=");pi(st.t_cht);ps("\n");}
void grub_state(void){ps("[GRUB] Soil=");pi(st.n_soil);ps(" Feed=");pi(st.n_feed);ps(" Breed=");pi(st.n_breed);ps(" Hlth=");pi(st.n_health);ps(" Mkt=");pi(st.n_mkt);ps("\n");}
int main(void){
ps("=== Grub Admin Demo ===\n\n");grub_init();
/* 1=lawn 2=garden 3=field 4=rot 5=compost */
ps("Grub soil...\n");for(int i=0;i<N;i++){int lc=(i%5)+1,bl=2+(i),bw=1+(i),sc=8+(i%4),ci=(i%3)+1,cp=(i%5)+1,ay=(i%3)+1;grub_soil(lc,bl,bw,sc,ci,cp,ay);}
ps("\nGrub feeding...\n");for(int i=0;i<N-2;i++){int lc=(i%4)+1,bl=3+(i),bw=2+(i),sc=9+(i%3),ci=(i%4)+1,cp=(i%4)+1,ay=(i%3)+1;grub_feeding(lc,bl,bw,sc,ci,cp,ay);}
ps("\nGrub breeding...\n");for(int i=0;i<N-4;i++){int lc=(i%3)+1,bl=2+(i*2),bw=1+(i),sc=7+(i%5),ci=(i%3)+2,cp=(i%3)+2,ay=(i%3)+1;grub_breeding(lc,bl,bw,sc,ci,cp,ay);}
ps("\nGrub health...\n");for(int i=0;i<N-6;i++){int lc=(i%5)+1,bl=4+(i),bw=2+(i),sc=10+(i%3),ci=(i%5)+1,cp=(i%4)+1,ay=(i%4)+1;grub_health(lc,bl,bw,sc,ci,cp,ay);}
ps("\nGrub market...\n");for(int i=0;i<N-6;i++){int lc=(i%4)+2,bl=5+(i),bw=3+(i),sc=11+(i%2),ci=(i%4)+1,cp=(i%3)+1,ay=(i%3)+1;grub_market(lc,bl,bw,sc,ci,cp,ay);}
ps("\n");grub_report();grub_state();ps("\n=== Demo Complete ===\n");return 0;}
