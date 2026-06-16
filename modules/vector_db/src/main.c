/* vector_db: Vector database for embeddings (v46.0) */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc"))) extern unsigned int host_alloc(unsigned int, unsigned int);
__attribute__((import_module("host"), import_name("print"))) extern void host_print(const char *);
__attribute__((import_module("host"), import_name("exit"))) extern void host_exit(int);
__attribute__((import_module("host"), import_name("get_argv"))) extern int host_get_argv(unsigned int, unsigned int);
#define MAX_VECTORS 1024
#define MAX_DIM 768
#define MAX_LABEL 64
typedef struct { int id; float vector[MAX_DIM]; int dim; char label[MAX_LABEL]; float similarity; } vector_t;
static vector_t vectors[MAX_VECTORS]; static int vector_count = 0; static int next_id = 1;
static int my_strlen(const char *s) { int l=0; while(s[l]) l++; return l; }
static void my_strncpy(char *d, const char *s, int n) { int i=0; while(i<n-1&&s[i]){d[i]=s[i];i++;} d[i]='\0'; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) { char b[32]; int p=0; if(v<0){b[p++]='-';v=-v;} if(v==0)b[p++]='0'; else{int d=0,t=v;while(t>0){d++;t/=10;}p+=d;b[p]='\0';p--;while(v>0){b[p--]='0'+(v%10);v/=10;}} host_print(b); }
static void pfloat(float v) { char b[32]; int i=(int)v; int f=(int)((v-i)*100); if(f<0)f=-f; char ib[16]; int p=0; if(i<0){b[p++]='-';i=-i;} if(i==0)ib[p++]='0'; else{int d=0,t=i;while(t>0){d++;t/=10;}p+=d;while(i>0){ib[--d]='0'+(i%10);i/=10;}} for(int j=0;j<p;j++)b[j]=ib[j]; b[p++]='.'; b[p++]='0'+(f/10); b[p++]='0'+(f%10); b[p]='\0'; host_print(b); }
int vec_insert(float *vec, int dim, const char *label) {
    if(vector_count>=MAX_VECTORS || dim>MAX_DIM) return -1;
    int idx = vector_count++;
    vectors[idx].id = next_id++;
    for(int i=0;i<dim;i++) vectors[idx].vector[i] = vec[i];
    vectors[idx].dim = dim;
    my_strncpy(vectors[idx].label, label, MAX_LABEL-1);
    pstr("  Inserted: "); pstr(label); pstr(" (dim="); pint(dim); pstr(")\n");
    return vectors[idx].id;
}
float vec_cosine_similarity(float *a, float *b, int dim) {
    float dot=0, norm_a=0, norm_b=0;
    for(int i=0;i<dim;i++) { dot+=a[i]*b[i]; norm_a+=a[i]*a[i]; norm_b+=b[i]*b[i]; }
    if(norm_a==0 || norm_b==0) return 0;
    return dot / (norm_a * norm_b);
}
int vec_search(float *query, int dim, int top_k) {
    pstr("  Search results (top "); pint(top_k); pstr("):\n");
    for(int i=0;i<vector_count;i++) {
        vectors[i].similarity = vec_cosine_similarity(vectors[i].vector, query, dim);
    }
    for(int k=0;k<top_k && k<vector_count;k++) {
        int best=-1; float best_sim=-1;
        for(int i=0;i<vector_count;i++) if(vectors[i].similarity>best_sim) { best=i; best_sim=vectors[i].similarity; }
        if(best>=0) {
            pstr("    ["); pint(vectors[best].id); pstr("] "); pstr(vectors[best].label);
            pstr(" sim="); pfloat(best_sim); pstr("\n");
            vectors[best].similarity = -2;
        }
    }
    return top_k;
}
int vec_stats(void) {
    pstr("  Vector DB stats:\n");
    pstr("    Vectors: "); pint(vector_count); pstr("/"); pint(MAX_VECTORS); pstr("\n");
    pstr("    Max dim: "); pint(MAX_DIM); pstr("\n");
    return vector_count;
}
void _start(void) {
    unsigned int buf = host_alloc(512,16); host_get_argv(buf,512);
    int help=0,test=0; unsigned int pos=0; char *av=(char*)buf;
    while(pos<512&&av[pos]) pos++; pos++;
    while(pos<512&&av[pos]) { char *a=&av[pos]; int l=my_strlen(a); if(l==2&&a[0]=='-'&&a[1]=='h') help=1; else if(l==2&&a[0]=='-'&&a[1]=='t') test=1; while(pos<512&&av[pos]) pos++; pos++; }
    pstr("Vector DB v46.0 - Embedding Storage\n");
    if(help) { pstr("Usage: vector_db [-h|-t]\n"); return; }
    if(test) {
        pstr("=== Vector DB Test ===\n\n");
        pstr("[1] Insert vectors\n");
        float v1[4]={1.0,0.0,0.0,0.0}, v2[4]={0.0,1.0,0.0,0.0}, v3[4]={0.7,0.7,0.0,0.0};
        vec_insert(v1, 4, "doc1_ai"); vec_insert(v2, 4, "doc2_ml"); vec_insert(v3, 4, "doc3_dl"); pstr("\n");
        pstr("[2] Stats\n"); vec_stats(); pstr("\n");
        pstr("[3] Search\n"); float q[4]={0.8,0.6,0.0,0.0}; vec_search(q, 4, 2);
        pstr("\n=== Done ===\n"); return;
    }
    pstr("Use -h for help, -t for test\n");
}
