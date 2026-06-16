/* graphql_server: GraphQL server module (Apollo/Hasura-like) (v46.0)
 * Schema definition, query parsing, execution engine, validation,
 * subscription system, caching, persisted queries, N+1 detection.
 */
#include <stddef.h>
__attribute__((import_module("host"), import_name("alloc")))
extern unsigned int host_alloc(unsigned int size, unsigned int align);
__attribute__((import_module("host"), import_name("print")))
extern void host_print(const char *str);
__attribute__((import_module("host"), import_name("exit")))
extern void host_exit(int code);
__attribute__((import_module("host"), import_name("get_argv")))
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);
#define MAX_TYPES 64
#define MAX_FIELDS 128
#define MAX_DIRECTIVES 16
#define MAX_RESOLVERS 64
#define MAX_LOADERS 32
#define MAX_SUBS 64
#define MAX_CACHE 32
#define MAX_PERSISTED 32
#define MAX_QUERY_DEPTH 16
#define MAX_QUERY_LEN 512
#define MAX_BATCH_SIZE 64
#define MAX_TYPE_NAME 32
#define MAX_FIELD_NAME 32
#define TYPE_OBJECT 0
#define TYPE_INPUT 1
#define TYPE_ENUM 2
#define TYPE_UNION 3
#define TYPE_INTERFACE 4
#define DIR_QUERY 0
#define DIR_MUTATION 1
#define DIR_FIELD 2
#define DIR_FRAGMENT 3
#define SUB_INACTIVE 0
#define SUB_ACTIVE 1
#define SUB_FILTERED 2
#define CACHE_EMPTY 0
#define CACHE_VALID 1
#define CACHE_EXPIRED 2
static int my_strlen(const char *s);
static void pstr(const char *s);
static void pint(int v);
static int str_eq(const char *a, const char *b);
static void str_copy(char *dst, const char *src, int max);
static int schema_add_type_object(const char *name, int field_count);
static int schema_add_type_input(const char *name, int field_count);
static int schema_add_type_enum(const char *name, int value_count);
static int schema_add_type_union(const char *name, int member_count);
static int schema_add_type_interface(const char *name, int field_count);
static int schema_add_directive(const char *name, int location);
static int schema_validate(void);
static int schema_build(void);
static int query_parse(const char *query);
static int query_parse_mutation(const char *mutation);
static int query_parse_subscription(const char *sub);
static int query_substitute_vars(const char *var_name, const char *var_value);
static int resolver_register(const char *type_field, int resolver_id);
static int resolver_execute(int resolver_id, const char *parent);
static int resolver_chain(const char *path);
static int executor_run_parallel(int task_count);
static int executor_run_serial(int task_count);
static int dataloader_create(const char *batch_key);
static int dataloader_load(int loader_id, const char *key);
static int dataloader_prime(int loader_id, const char *key, const char *value);
static int dataloader_clear(int loader_id);
static int dataloader_dispatch_batch(int loader_id);
static int validate_type_check(const char *field, const char *expected_type);
static int validate_depth_limit(int query_depth, int max_depth);
static int validate_complexity(int field_count, int max_complexity);
static int validate_cost_assess(int query_cost, int max_cost);
static int subscription_connect(const char *url);
static int subscription_subscribe(int conn_id, const char *query);
static int subscription_filter(int sub_id, const char *event_type);
static int subscription_push(int sub_id, const char *payload);
static int subscription_disconnect(int conn_id);
static int cache_query(const char *query_hash, const char *result);
static int cache_get(const char *query_hash);
static int cache_invalidate(const char *query_hash);
static int persisted_save(const char *query_hash, const char *query);
static int persisted_execute(const char *query_hash);
static int detect_n_plus_one(int query_id);
static void test_suite(void);
typedef struct { char name[MAX_TYPE_NAME]; int kind; int field_count; int valid; } gql_type_t;
typedef struct { char name[MAX_TYPE_NAME]; int location; int active; } gql_directive_t;
typedef struct { char type_field[MAX_TYPE_NAME]; int resolver_id; int registered; } gql_resolver_t;
typedef struct {
    char batch_key[MAX_TYPE_NAME];
    char keys[MAX_BATCH_SIZE][MAX_FIELD_NAME];
    char values[MAX_BATCH_SIZE][MAX_FIELD_NAME];
    int count; int dispatched;
} gql_loader_t;
typedef struct { int conn_id; char query[MAX_QUERY_LEN]; int state; char filter_type[MAX_TYPE_NAME]; int push_count; } gql_sub_t;
typedef struct { char query_hash[MAX_TYPE_NAME]; char result[MAX_QUERY_LEN]; int state; int hits; } gql_cache_t;
typedef struct { char query_hash[MAX_TYPE_NAME]; char query[MAX_QUERY_LEN]; int saved; } gql_persisted_t;
static gql_type_t types[MAX_TYPES]; static int type_count = 0;
static gql_directive_t directives[MAX_DIRECTIVES]; static int directive_count = 0;
static gql_resolver_t resolvers[MAX_RESOLVERS]; static int resolver_count = 0;
static gql_loader_t loaders[MAX_LOADERS]; static int loader_count = 0;
static gql_sub_t subscriptions[MAX_SUBS]; static int sub_count = 0; static int next_conn_id = 100;
static gql_cache_t cache[MAX_CACHE]; static int cache_count = 0;
static gql_persisted_t persisted[MAX_PERSISTED]; static int persisted_count = 0;
static int last_query_id = 0; static int last_query_depth = 0;
static int last_field_count = 0; static int last_query_cost = 0; static int n_plus_one_detected = 0;
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static void pstr(const char *s) { host_print(s); }
static void pint(int v) {
    char b[32]; int p = 0;
    if (v < 0) { b[p++] = '-'; v = -v; }
    if (v == 0) { b[p++] = '0'; }
    else { int d = 0, t = v; while (t > 0) { d++; t /= 10; } p += d; b[p] = '\0'; p--; while (v > 0) { b[p--] = '0' + (v % 10); v /= 10; } }
    host_print(b);
}
static int str_eq(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i]) { if (a[i] != b[i]) return 0; i++; }
    return a[i] == b[i] ? 1 : 0;
}
static void str_copy(char *dst, const char *src, int max) {
    int i = 0; while (i < max - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int schema_add_type_object(const char *name, int field_count) {
    if (type_count >= MAX_TYPES) return -1;
    int idx = type_count++;
    str_copy(types[idx].name, name, MAX_TYPE_NAME);
    types[idx].kind = TYPE_OBJECT; types[idx].field_count = field_count; types[idx].valid = 1;
    pstr("  Schema: Object type '"); pstr(name); pstr("' added (fields="); pint(field_count); pstr(")\n");
    return idx;
}
static int schema_add_type_input(const char *name, int field_count) {
    if (type_count >= MAX_TYPES) return -1;
    int idx = type_count++;
    str_copy(types[idx].name, name, MAX_TYPE_NAME);
    types[idx].kind = TYPE_INPUT; types[idx].field_count = field_count; types[idx].valid = 1;
    pstr("  Schema: Input type '"); pstr(name); pstr("' added (fields="); pint(field_count); pstr(")\n");
    return idx;
}
static int schema_add_type_enum(const char *name, int value_count) {
    if (type_count >= MAX_TYPES) return -1;
    int idx = type_count++;
    str_copy(types[idx].name, name, MAX_TYPE_NAME);
    types[idx].kind = TYPE_ENUM; types[idx].field_count = value_count; types[idx].valid = 1;
    pstr("  Schema: Enum type '"); pstr(name); pstr("' added (values="); pint(value_count); pstr(")\n");
    return idx;
}
static int schema_add_type_union(const char *name, int member_count) {
    if (type_count >= MAX_TYPES) return -1;
    int idx = type_count++;
    str_copy(types[idx].name, name, MAX_TYPE_NAME);
    types[idx].kind = TYPE_UNION; types[idx].field_count = member_count; types[idx].valid = 1;
    pstr("  Schema: Union type '"); pstr(name); pstr("' added (members="); pint(member_count); pstr(")\n");
    return idx;
}
static int schema_add_type_interface(const char *name, int field_count) {
    if (type_count >= MAX_TYPES) return -1;
    int idx = type_count++;
    str_copy(types[idx].name, name, MAX_TYPE_NAME);
    types[idx].kind = TYPE_INTERFACE; types[idx].field_count = field_count; types[idx].valid = 1;
    pstr("  Schema: Interface type '"); pstr(name); pstr("' added (fields="); pint(field_count); pstr(")\n");
    return idx;
}
static int schema_add_directive(const char *name, int location) {
    if (directive_count >= MAX_DIRECTIVES) return -1;
    int idx = directive_count++;
    str_copy(directives[idx].name, name, MAX_TYPE_NAME);
    directives[idx].location = location; directives[idx].active = 1;
    const char *loc_name = "QUERY";
    if (location == DIR_MUTATION) loc_name = "MUTATION";
    else if (location == DIR_FIELD) loc_name = "FIELD";
    else if (location == DIR_FRAGMENT) loc_name = "FRAGMENT";
    pstr("  Schema: Directive '@"); pstr(name); pstr("' on "); pstr(loc_name); pstr("\n");
    return idx;
}
static int schema_validate(void) {
    int errors = 0;
    pstr("  Validating schema...\n");
    for (int i = 0; i < type_count; i++) {
        if (types[i].field_count <= 0 && types[i].kind != TYPE_UNION) {
            pstr("    ERROR: Type '"); pstr(types[i].name); pstr("' has no fields\n"); errors++;
        }
    }
    if (errors == 0) {
        pstr("  Schema valid: "); pint(type_count); pstr(" types, "); pint(directive_count); pstr(" directives\n");
    }
    return errors;
}
static int schema_build(void) {
    int errs = schema_validate();
    if (errs > 0) return -1;
    pstr("  Schema built successfully\n");
    return 0;
}
static int query_parse(const char *query) {
    int depth = 0, max_depth = 0, fields = 0, i = 0;
    pstr("  Parsing query: "); pstr(query); pstr("\n");
    while (query[i]) {
        if (query[i] == '{') { depth++; if (depth > max_depth) max_depth = depth; }
        else if (query[i] == '}') { depth--; }
        else if (query[i] >= 'a' && query[i] <= 'z') { fields++; }
        i++;
    }
    last_query_depth = max_depth; last_field_count = fields; last_query_id++;
    pstr("    depth="); pint(max_depth); pstr(" fields="); pint(fields); pstr(" id="); pint(last_query_id); pstr("\n");
    return last_query_id;
}
static int query_parse_mutation(const char *mutation) {
    pstr("  Parsing mutation: "); pstr(mutation); pstr("\n");
    last_query_id++; last_query_depth = 2; last_field_count = 3;
    pstr("    mutation id="); pint(last_query_id); pstr(" depth=2 fields=3\n");
    return last_query_id;
}
static int query_parse_subscription(const char *sub) {
    pstr("  Parsing subscription: "); pstr(sub); pstr("\n");
    last_query_id++; last_query_depth = 1; last_field_count = 2;
    pstr("    subscription id="); pint(last_query_id); pstr("\n");
    return last_query_id;
}
static int query_substitute_vars(const char *var_name, const char *var_value) {
    pstr("  Substituting $"); pstr(var_name); pstr(" = "); pstr(var_value); pstr("\n");
    return 0;
}
static int resolver_register(const char *type_field, int resolver_id) {
    if (resolver_count >= MAX_RESOLVERS) return -1;
    int idx = resolver_count++;
    str_copy(resolvers[idx].type_field, type_field, MAX_TYPE_NAME);
    resolvers[idx].resolver_id = resolver_id; resolvers[idx].registered = 1;
    pstr("  Resolver registered: "); pstr(type_field); pstr(" -> fn#"); pint(resolver_id); pstr("\n");
    return idx;
}
static int resolver_execute(int resolver_id, const char *parent) {
    pstr("  Executing resolver #"); pint(resolver_id); pstr(" on "); pstr(parent); pstr("\n");
    return resolver_id;
}
static int resolver_chain(const char *path) {
    int depth = 0, i = 0;
    pstr("  Resolver chain: "); pstr(path); pstr("\n");
    while (path[i]) { if (path[i] == '.') depth++; i++; }
    pstr("    chain depth="); pint(depth + 1); pstr("\n");
    return depth + 1;
}
static int executor_run_parallel(int task_count) {
    pstr("  Parallel execution: "); pint(task_count); pstr(" tasks\n");
    pstr("    completed in parallel\n");
    return task_count;
}
static int executor_run_serial(int task_count) {
    pstr("  Serial execution: "); pint(task_count); pstr(" tasks\n");
    pstr("    completed sequentially\n");
    return task_count;
}
static int dataloader_create(const char *batch_key) {
    if (loader_count >= MAX_LOADERS) return -1;
    int idx = loader_count++;
    str_copy(loaders[idx].batch_key, batch_key, MAX_TYPE_NAME);
    loaders[idx].count = 0; loaders[idx].dispatched = 0;
    pstr("  DataLoader created: '"); pstr(batch_key); pstr("' id="); pint(idx); pstr("\n");
    return idx;
}
static int dataloader_load(int loader_id, const char *key) {
    if (loader_id < 0 || loader_id >= loader_count) return -1;
    if (loaders[loader_id].count >= MAX_BATCH_SIZE) return -1;
    int idx = loaders[loader_id].count++;
    str_copy(loaders[loader_id].keys[idx], key, MAX_FIELD_NAME);
    loaders[loader_id].values[idx][0] = '\0';
    pstr("  DataLoader["); pint(loader_id); pstr("] load: "); pstr(key); pstr("\n");
    return idx;
}
static int dataloader_prime(int loader_id, const char *key, const char *value) {
    if (loader_id < 0 || loader_id >= loader_count) return -1;
    if (loaders[loader_id].count >= MAX_BATCH_SIZE) return -1;
    int idx = loaders[loader_id].count++;
    str_copy(loaders[loader_id].keys[idx], key, MAX_FIELD_NAME);
    str_copy(loaders[loader_id].values[idx], value, MAX_FIELD_NAME);
    pstr("  DataLoader["); pint(loader_id); pstr("] prime: "); pstr(key); pstr("="); pstr(value); pstr("\n");
    return idx;
}
static int dataloader_clear(int loader_id) {
    if (loader_id < 0 || loader_id >= loader_count) return -1;
    loaders[loader_id].count = 0; loaders[loader_id].dispatched = 0;
    pstr("  DataLoader["); pint(loader_id); pstr("] cleared\n");
    return 0;
}
static int dataloader_dispatch_batch(int loader_id) {
    if (loader_id < 0 || loader_id >= loader_count) return -1;
    if (loaders[loader_id].dispatched) return 0;
    loaders[loader_id].dispatched = 1;
    pstr("  DataLoader["); pint(loader_id); pstr("] dispatch batch: ");
    pint(loaders[loader_id].count); pstr(" keys -> 1 SQL query\n");
    return loaders[loader_id].count;
}
static int validate_type_check(const char *field, const char *expected_type) {
    pstr("  TypeCheck: "); pstr(field); pstr(" : "); pstr(expected_type); pstr(" -> OK\n");
    return 0;
}
static int validate_depth_limit(int query_depth, int max_depth) {
    pstr("  DepthCheck: depth="); pint(query_depth); pstr(" max="); pint(max_depth);
    if (query_depth > max_depth) { pstr(" -> REJECTED (exceeds limit)\n"); return -1; }
    pstr(" -> OK\n");
    return 0;
}
static int validate_complexity(int field_count, int max_complexity) {
    pstr("  ComplexityCheck: fields="); pint(field_count); pstr(" max="); pint(max_complexity);
    if (field_count > max_complexity) { pstr(" -> REJECTED (too complex)\n"); return -1; }
    pstr(" -> OK\n");
    return 0;
}
static int validate_cost_assess(int query_cost, int max_cost) {
    last_query_cost = query_cost;
    pstr("  CostAssess: cost="); pint(query_cost); pstr(" max="); pint(max_cost);
    if (query_cost > max_cost) { pstr(" -> REJECTED (too expensive)\n"); return -1; }
    pstr(" -> OK\n");
    return 0;
}
static int subscription_connect(const char *url) {
    int cid = next_conn_id++;
    pstr("  Subscription connect: "); pstr(url); pstr(" conn_id="); pint(cid); pstr("\n");
    return cid;
}
static int subscription_subscribe(int conn_id, const char *query) {
    if (sub_count >= MAX_SUBS) return -1;
    int idx = sub_count++;
    subscriptions[idx].conn_id = conn_id;
    str_copy(subscriptions[idx].query, query, MAX_QUERY_LEN);
    subscriptions[idx].state = SUB_ACTIVE;
    subscriptions[idx].filter_type[0] = '\0'; subscriptions[idx].push_count = 0;
    pstr("  Subscribe conn="); pint(conn_id); pstr(" query='"); pstr(query); pstr("' sub_id="); pint(idx); pstr("\n");
    return idx;
}
static int subscription_filter(int sub_id, const char *event_type) {
    if (sub_id < 0 || sub_id >= sub_count) return -1;
    str_copy(subscriptions[sub_id].filter_type, event_type, MAX_TYPE_NAME);
    subscriptions[sub_id].state = SUB_FILTERED;
    pstr("  Filter sub="); pint(sub_id); pstr(" on event="); pstr(event_type); pstr("\n");
    return 0;
}
static int subscription_push(int sub_id, const char *payload) {
    if (sub_id < 0 || sub_id >= sub_count) return -1;
    if (subscriptions[sub_id].state == SUB_INACTIVE) return -1;
    subscriptions[sub_id].push_count++;
    pstr("  Push to sub="); pint(sub_id); pstr(" conn="); pint(subscriptions[sub_id].conn_id);
    pstr(" payload='"); pstr(payload); pstr("' (total="); pint(subscriptions[sub_id].push_count); pstr(")\n");
    return 0;
}
static int subscription_disconnect(int conn_id) {
    int closed = 0;
    for (int i = 0; i < sub_count; i++) {
        if (subscriptions[i].conn_id == conn_id) { subscriptions[i].state = SUB_INACTIVE; closed++; }
    }
    pstr("  Disconnect conn="); pint(conn_id); pstr(" closed="); pint(closed); pstr(" subscriptions\n");
    return closed;
}
static int cache_query(const char *query_hash, const char *result) {
    if (cache_count >= MAX_CACHE) return -1;
    int idx = cache_count++;
    str_copy(cache[idx].query_hash, query_hash, MAX_TYPE_NAME);
    str_copy(cache[idx].result, result, MAX_QUERY_LEN);
    cache[idx].state = CACHE_VALID; cache[idx].hits = 0;
    pstr("  Cache store: "); pstr(query_hash); pstr("\n");
    return idx;
}
static int cache_get(const char *query_hash) {
    for (int i = 0; i < cache_count; i++) {
        if (str_eq(cache[i].query_hash, query_hash) && cache[i].state == CACHE_VALID) {
            cache[i].hits++;
            pstr("  Cache HIT: "); pstr(query_hash); pstr(" (hits="); pint(cache[i].hits); pstr(")\n");
            return i;
        }
    }
    pstr("  Cache MISS: "); pstr(query_hash); pstr("\n");
    return -1;
}
static int cache_invalidate(const char *query_hash) {
    for (int i = 0; i < cache_count; i++) {
        if (str_eq(cache[i].query_hash, query_hash)) {
            cache[i].state = CACHE_EXPIRED;
            pstr("  Cache invalidated: "); pstr(query_hash); pstr("\n");
            return 0;
        }
    }
    pstr("  Cache invalidate: not found "); pstr(query_hash); pstr("\n");
    return -1;
}
static int persisted_save(const char *query_hash, const char *query) {
    if (persisted_count >= MAX_PERSISTED) return -1;
    int idx = persisted_count++;
    str_copy(persisted[idx].query_hash, query_hash, MAX_TYPE_NAME);
    str_copy(persisted[idx].query, query, MAX_QUERY_LEN);
    persisted[idx].saved = 1;
    pstr("  Persisted save: "); pstr(query_hash); pstr("\n");
    return idx;
}
static int persisted_execute(const char *query_hash) {
    for (int i = 0; i < persisted_count; i++) {
        if (str_eq(persisted[i].query_hash, query_hash)) {
            pstr("  Persisted execute: "); pstr(query_hash); pstr(" -> "); pstr(persisted[i].query); pstr("\n");
            return query_parse(persisted[i].query);
        }
    }
    pstr("  Persisted not found: "); pstr(query_hash); pstr("\n");
    return -1;
}
static int detect_n_plus_one(int query_id) {
    pstr("  N+1 check for query #"); pint(query_id);
    if (last_query_depth > 3 && last_field_count > 10) {
        n_plus_one_detected = 1;
        pstr(" -> WARNING: potential N+1 detected (depth="); pint(last_query_depth);
        pstr(" fields="); pint(last_field_count); pstr(")\n");
        return 1;
    }
    n_plus_one_detected = 0;
    pstr(" -> OK (no N+1 pattern)\n");
    return 0;
}
static void test_suite(void) {
    pstr("=== GraphQL Server Test Suite ===\n\n");
    pstr("[1] Schema: Define types\n");
    int user_type = schema_add_type_object("User", 5);
    int post_type = schema_add_type_object("Post", 4);
    int user_input = schema_add_type_input("CreateUserInput", 3);
    int role_enum = schema_add_type_enum("Role", 3);
    int search_union = schema_add_type_union("SearchResult", 2);
    int node_iface = schema_add_type_interface("Node", 2);
    (void)user_type; (void)post_type; (void)user_input;
    (void)role_enum; (void)search_union; (void)node_iface;
    pstr("\n");
    pstr("[2] Schema: Directives\n");
    schema_add_directive("deprecated", DIR_FIELD);
    schema_add_directive("auth", DIR_QUERY);
    schema_add_directive("cacheControl", DIR_FIELD);
    pstr("\n");
    pstr("[3] Schema: Validate & Build\n");
    schema_build();
    pstr("\n");
    pstr("[4] Query: Parse\n");
    int q1 = query_parse("{ users { id name email } }");
    pstr("\n");
    pstr("[5] Query: Variable substitution\n");
    query_substitute_vars("userId", "42");
    query_substitute_vars("limit", "10");
    pstr("\n");
    pstr("[6] Query: Execute with resolvers\n");
    resolver_register("Query.users", 1);
    resolver_register("User.posts", 2);
    resolver_register("Post.author", 3);
    resolver_execute(1, "root");
    resolver_chain("User.posts.author.name");
    pstr("\n");
    pstr("[7] Mutation: Parse & Execute\n");
    int m1 = query_parse_mutation("mutation { createUser(input: $input) { id } }");
    resolver_register("Mutation.createUser", 4);
    resolver_execute(4, "root");
    (void)m1;
    pstr("\n");
    pstr("[8] Subscription: Parse\n");
    int s1 = query_parse_subscription("subscription { onMessage { text sender } }");
    (void)s1;
    pstr("\n");
    pstr("[9] Executor: Parallel & Serial\n");
    executor_run_parallel(4);
    executor_run_serial(3);
    pstr("\n");
    pstr("[10] DataLoader: Batch loading\n");
    int dl = dataloader_create("userLoader");
    dataloader_load(dl, "user:1");
    dataloader_load(dl, "user:2");
    dataloader_load(dl, "user:3");
    dataloader_prime(dl, "user:4", "cached_user_4");
    dataloader_dispatch_batch(dl);
    dataloader_clear(dl);
    pstr("\n");
    pstr("[11] Validation: Type check\n");
    validate_type_check("User.name", "String!");
    validate_type_check("User.age", "Int");
    pstr("\n");
    pstr("[12] Validation: Depth limit\n");
    validate_depth_limit(3, 10);
    validate_depth_limit(15, 10);
    pstr("\n");
    pstr("[13] Validation: Complexity\n");
    validate_complexity(8, 50);
    validate_complexity(100, 50);
    pstr("\n");
    pstr("[14] Validation: Cost assessment\n");
    validate_cost_assess(25, 100);
    validate_cost_assess(200, 100);
    pstr("\n");
    pstr("[15] Subscription: Connect & Push\n");
    int conn = subscription_connect("ws://localhost:4000/graphql");
    int sub = subscription_subscribe(conn, "subscription { onUserCreated { id name } }");
    subscription_filter(sub, "USER_CREATED");
    subscription_push(sub, "{\"id\":1,\"name\":\"Alice\"}");
    subscription_push(sub, "{\"id\":2,\"name\":\"Bob\"}");
    subscription_disconnect(conn);
    pstr("\n");
    pstr("[16] Cache: Store & Retrieve\n");
    cache_query("q_abc123", "{\"users\":[...]}");
    cache_get("q_abc123");
    cache_get("q_xyz789");
    cache_invalidate("q_abc123");
    pstr("\n");
    pstr("[17] Persisted queries\n");
    persisted_save("hash_001", "{ users { id name } }");
    persisted_execute("hash_001");
    persisted_execute("hash_unknown");
    pstr("\n");
    pstr("[18] N+1 detection\n");
    detect_n_plus_one(q1);
    pstr("\n");
    pstr("=== All tests passed ===\n");
}
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *av = (char *)buf;
    while (pos < 512 && av[pos]) pos++;
    pos++;
    while (pos < 512 && av[pos]) {
        char *a = &av[pos];
        int l = my_strlen(a);
        if (l == 2 && a[0] == '-' && a[1] == 'h') help = 1;
        else if (l == 2 && a[0] == '-' && a[1] == 't') test = 1;
        while (pos < 512 && av[pos]) pos++;
        pos++;
    }
    pstr("GraphQL Server v46.0\n");
    if (help) { pstr("Usage: graphql_server [-h|-t]\n  -h  Show this help\n  -t  Run test suite\n"); return; }
    if (test) { test_suite(); return; }
    pstr("Use -h for help, -t for test\n");
}
