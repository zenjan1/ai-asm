/* document_management: Document management system (v1.0)
 * Document storage, access control, search, collaboration, workflow
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

#define MAX_DOCUMENTS  20
#define MAX_FOLDERS    8
#define MAX_USERS      10
#define MAX_TAGS       16
#define MAX_VERSIONS   12
#define MAX_WORKFLOWS  6
#define MAX_COMMENTS   16

typedef struct {
    int    doc_id;
    int    folder_id;
    int    owner_id;
    int    type;
    int    size_kb;
    int    n_versions;
    int    n_comments;
    int    n_tags;
    int    access_level;
    int    status;
    int    active;
} document_t;

typedef struct {
    int    folder_id;
    int    parent_id;
    int    n_documents;
    int    n_subfolders;
    int    active;
} folder_t;

typedef struct {
    int    user_id;
    int    role;
    int    n_docs_owned;
    int    n_docs_shared;
    int    active;
} user_t;

typedef struct {
    int    tag_id;
    int    n_documents;
    int    active;
} tag_t;

typedef struct {
    int    version_id;
    int    doc_id;
    int    version_num;
    int    size_kb;
    int    author_id;
    int    active;
} version_t;

typedef struct {
    int    workflow_id;
    int    type;
    int    n_steps;
    int    current_step;
    int    n_docs;
    int    active;
} workflow_t;

typedef struct {
    int    comment_id;
    int    doc_id;
    int    author_id;
    int    resolved;
    int    active;
} comment_t;

typedef struct {
    int    n_documents;
    int    n_folders;
    int    n_users;
    int    n_tags;
    int    n_versions;
    int    n_workflows;
    int    n_comments;
    double total_size_mb;
    int    total_access_events;
    int    audit_log_entries;
} dm_state_t;

static document_t documents[MAX_DOCUMENTS];
static folder_t folders[MAX_FOLDERS];
static user_t users[MAX_USERS];
static tag_t tags[MAX_TAGS];
static version_t versions[MAX_VERSIONS];
static workflow_t workflows[MAX_WORKFLOWS];
static comment_t comments[MAX_COMMENTS];
static dm_state_t dm;

static int initialized = 0;

static void print_str(const char* s) { host_print(s); }
static void print_int(int val) {
    char buf[32]; int i = 0;
    if (val < 0) { buf[i++] = '-'; val = -val; }
    if (val == 0) { buf[i++] = '0'; }
    else { int s = i; while (val > 0) { buf[i++] = '0' + (val % 10); val /= 10; }
           int e = i - 1; while (s < e) { char t = buf[s]; buf[s] = buf[e]; buf[e] = t; s++; e--; } }
    buf[i] = '\0'; host_print(buf);
}

int dm_init(void) {
    if (initialized) return -1;
    dm.n_documents = 0; dm.n_folders = 0; dm.n_users = 0;
    dm.n_tags = 0; dm.n_versions = 0; dm.n_workflows = 0;
    dm.n_comments = 0;
    dm.total_size_mb = 0.0; dm.total_access_events = 0;
    dm.audit_log_entries = 0;
    for (int i = 0; i < MAX_DOCUMENTS; i++) documents[i].active = 0;
    for (int i = 0; i < MAX_FOLDERS; i++) folders[i].active = 0;
    for (int i = 0; i < MAX_USERS; i++) users[i].active = 0;
    for (int i = 0; i < MAX_TAGS; i++) tags[i].active = 0;
    for (int i = 0; i < MAX_VERSIONS; i++) versions[i].active = 0;
    for (int i = 0; i < MAX_WORKFLOWS; i++) workflows[i].active = 0;
    for (int i = 0; i < MAX_COMMENTS; i++) comments[i].active = 0;
    initialized = 1;
    print_str("[DM] Document management initialized\n");
    return 0;
}

int dm_create_user(int role) {
    if (dm.n_users >= MAX_USERS) return -1;
    user_t* u = &users[dm.n_users];
    u->user_id = dm.n_users;
    u->role = role;
    u->n_docs_owned = 0;
    u->n_docs_shared = 0;
    u->active = 1;
    dm.n_users++;
    print_str("[DM] User "); print_int(dm.n_users - 1);
    print_str(" role="); print_int(role); print_str("\n");
    return dm.n_users - 1;
}

int dm_create_folder(int parent_id) {
    if (dm.n_folders >= MAX_FOLDERS) return -1;
    folder_t* f = &folders[dm.n_folders];
    f->folder_id = dm.n_folders;
    f->parent_id = parent_id;
    f->n_documents = 0;
    f->n_subfolders = 0;
    f->active = 1;
    dm.n_folders++;
    if (parent_id >= 0 && parent_id < dm.n_folders) {
        folders[parent_id].n_subfolders++;
    }
    print_str("[DM] Folder "); print_int(dm.n_folders - 1);
    print_str(" parent="); print_int(parent_id); print_str("\n");
    return dm.n_folders - 1;
}

int dm_create_tag(void) {
    if (dm.n_tags >= MAX_TAGS) return -1;
    tag_t* t = &tags[dm.n_tags];
    t->tag_id = dm.n_tags;
    t->n_documents = 0;
    t->active = 1;
    dm.n_tags++;
    print_str("[DM] Tag "); print_int(dm.n_tags - 1); print_str(" created\n");
    return dm.n_tags - 1;
}

int dm_upload_document(int folder_id, int owner_id, int type, int size_kb, int access_level) {
    if (dm.n_documents >= MAX_DOCUMENTS) return -1;
    if (folder_id >= dm.n_folders || owner_id >= dm.n_users) return -1;
    document_t* d = &documents[dm.n_documents];
    d->doc_id = dm.n_documents;
    d->folder_id = folder_id;
    d->owner_id = owner_id;
    d->type = type;
    d->size_kb = size_kb;
    d->n_versions = 1;
    d->n_comments = 0;
    d->n_tags = 0;
    d->access_level = access_level;
    d->status = 1;
    d->active = 1;
    dm.n_documents++;
    dm.total_size_mb += (double)size_kb / 1024.0;
    folders[folder_id].n_documents++;
    users[owner_id].n_docs_owned++;
    if (dm.n_versions < MAX_VERSIONS) {
        version_t* v = &versions[dm.n_versions];
        v->version_id = dm.n_versions;
        v->doc_id = dm.n_documents - 1;
        v->version_num = 1;
        v->size_kb = size_kb;
        v->author_id = owner_id;
        v->active = 1;
        dm.n_versions++;
    }
    dm.audit_log_entries++;
    print_str("[DM] Document "); print_int(dm.n_documents - 1);
    print_str(" F"); print_int(folder_id); print_str(" U"); print_int(owner_id);
    print_str(" type="); print_int(type);
    print_str(" size="); print_int(size_kb); print_str("KB");
    print_str(" access="); print_int(access_level); print_str("\n");
    return dm.n_documents - 1;
}

int dm_new_version(int doc_id, int author_id, int size_kb) {
    if (doc_id >= dm.n_documents || dm.n_versions >= MAX_VERSIONS) return -1;
    document_t* d = &documents[doc_id];
    version_t* v = &versions[dm.n_versions];
    v->version_id = dm.n_versions;
    v->doc_id = doc_id;
    v->version_num = d->n_versions + 1;
    v->size_kb = size_kb;
    v->author_id = author_id;
    v->active = 1;
    dm.n_versions++;
    d->n_versions++;
    dm.total_size_mb += (double)(size_kb - d->size_kb) / 1024.0;
    d->size_kb = size_kb;
    dm.audit_log_entries++;
    print_str("[DM] Version "); print_int(v->version_num);
    print_str(" doc="); print_int(doc_id);
    print_str(" U"); print_int(author_id);
    print_str(" size="); print_int(size_kb); print_str("KB\n");
    return v->version_id;
}

void dm_tag_document(int doc_id, int tag_id) {
    if (doc_id >= dm.n_documents || tag_id >= dm.n_tags) return;
    documents[doc_id].n_tags++;
    tags[tag_id].n_documents++;
    print_str("[DM] Tagged D"); print_int(doc_id);
    print_str(" with T"); print_int(tag_id);
    print_str(" (doc tags="); print_int(documents[doc_id].n_tags);
    print_str(", tag docs="); print_int(tags[tag_id].n_documents); print_str(")\n");
}

int dm_add_comment(int doc_id, int author_id) {
    if (dm.n_comments >= MAX_COMMENTS) return -1;
    if (doc_id >= dm.n_documents || author_id >= dm.n_users) return -1;
    comment_t* c = &comments[dm.n_comments];
    c->comment_id = dm.n_comments;
    c->doc_id = doc_id;
    c->author_id = author_id;
    c->resolved = 0;
    c->active = 1;
    dm.n_comments++;
    documents[doc_id].n_comments++;
    print_str("[DM] Comment "); print_int(dm.n_comments - 1);
    print_str(" on D"); print_int(doc_id);
    print_str(" by U"); print_int(author_id); print_str("\n");
    return dm.n_comments - 1;
}

void dm_resolve_comment(int comment_id) {
    if (comment_id >= dm.n_comments) return;
    comments[comment_id].resolved = 1;
    print_str("[DM] Comment "); print_int(comment_id); print_str(" resolved\n");
}

void dm_share_document(int doc_id, int user_id, int access_level) {
    if (doc_id >= dm.n_documents || user_id >= dm.n_users) return;
    documents[doc_id].access_level = access_level;
    users[user_id].n_docs_shared++;
    dm.audit_log_entries++;
    print_str("[DM] Shared D"); print_int(doc_id);
    print_str(" with U"); print_int(user_id);
    print_str(" access="); print_int(access_level); print_str("\n");
}

int dm_create_workflow(int type, int steps) {
    if (dm.n_workflows >= MAX_WORKFLOWS) return -1;
    workflow_t* w = &workflows[dm.n_workflows];
    w->workflow_id = dm.n_workflows;
    w->type = type;
    w->n_steps = steps;
    w->current_step = 1;
    w->n_docs = 0;
    w->active = 1;
    dm.n_workflows++;
    print_str("[DM] Workflow "); print_int(dm.n_workflows - 1);
    print_str(" type="); print_int(type);
    print_str(" steps="); print_int(steps); print_str("\n");
    return dm.n_workflows - 1;
}

void dm_advance_workflow(int workflow_id) {
    if (workflow_id >= dm.n_workflows) return;
    workflow_t* w = &workflows[workflow_id];
    if (w->current_step < w->n_steps) {
        w->current_step++;
        w->n_docs++;
        print_str("[DM] Workflow "); print_int(workflow_id);
        print_str(" step "); print_int(w->current_step);
        print_str("/"); print_int(w->n_steps); print_str("\n");
    } else {
        print_str("[DM] Workflow "); print_int(workflow_id); print_str(" COMPLETED\n");
    }
}

void dm_search_documents(int tag_filter, int owner_filter) {
    print_str("[DM] Search");
    if (tag_filter >= 0) { print_str(" tag="); print_int(tag_filter); }
    if (owner_filter >= 0) { print_str(" owner="); print_int(owner_filter); }
    print_str(":\n");
    int matches = 0;
    for (int i = 0; i < dm.n_documents; i++) {
        int match = 1;
        if (tag_filter >= 0 && documents[i].n_tags == 0) match = 0;
        if (owner_filter >= 0 && documents[i].owner_id != owner_filter) match = 0;
        if (match) {
            print_str("  D"); print_int(i);
            print_str(" type="); print_int(documents[i].type);
            print_str(" size="); print_int(documents[i].size_kb); print_str("KB");
            print_str(" tags="); print_int(documents[i].n_tags); print_str("\n");
            matches++;
        }
    }
    print_str("  Matches: "); print_int(matches); print_str("\n");
    dm.total_access_events++;
}

void dm_audit_log(void) {
    print_str("[DM] Audit log:\n");
    print_str("  Total documents: "); print_int(dm.n_documents); print_str("\n");
    print_str("  Total versions: "); print_int(dm.n_versions); print_str("\n");
    print_str("  Total comments: "); print_int(dm.n_comments); print_str("\n");
    print_str("  Access events: "); print_int(dm.total_access_events); print_str("\n");
    print_str("  Audit entries: "); print_int(dm.audit_log_entries); print_str("\n");
    int resolved = 0;
    for (int i = 0; i < dm.n_comments; i++) {
        if (comments[i].resolved) resolved++;
    }
    print_str("  Comments resolved: "); print_int(resolved);
    print_str("/"); print_int(dm.n_comments); print_str("\n");
}

void dm_print_state(void) {
    print_str("[DM] Documents="); print_int(dm.n_documents);
    print_str(" Folders="); print_int(dm.n_folders);
    print_str(" Users="); print_int(dm.n_users);
    print_str(" Tags="); print_int(dm.n_tags);
    print_str(" Versions="); print_int(dm.n_versions);
    print_str(" Workflows="); print_int(dm.n_workflows);
    print_str("\n");
    print_str("  Total size: "); print_int((int)(dm.total_size_mb * 1024)); print_str("KB\n");
    print_str("  Comments: "); print_int(dm.n_comments); print_str("\n");
    print_str("  Access events: "); print_int(dm.total_access_events); print_str("\n");
}

int main(void) {
    print_str("=== Document Management System Demo ===\n\n");
    dm_init();

    print_str("Creating users...\n");
    dm_create_user(1);
    dm_create_user(2);
    dm_create_user(2);
    dm_create_user(3);
    dm_create_user(2);

    print_str("\nCreating folder structure...\n");
    dm_create_folder(-1);
    dm_create_folder(0);
    dm_create_folder(0);
    dm_create_folder(1);
    dm_create_folder(2);
    dm_create_folder(-1);

    print_str("\nCreating tags...\n");
    dm_create_tag();
    dm_create_tag();
    dm_create_tag();
    dm_create_tag();
    dm_create_tag();

    print_str("\nUploading documents...\n");
    dm_upload_document(0, 0, 1, 256, 1);
    dm_upload_document(1, 0, 2, 512, 2);
    dm_upload_document(2, 1, 1, 128, 1);
    dm_upload_document(3, 2, 3, 1024, 3);
    dm_upload_document(4, 3, 2, 384, 2);
    dm_upload_document(5, 4, 1, 192, 1);
    dm_upload_document(1, 0, 2, 448, 2);
    dm_upload_document(2, 1, 3, 640, 3);
    dm_upload_document(3, 2, 1, 320, 1);

    print_str("\nCreating document versions...\n");
    dm_new_version(0, 1, 280);
    dm_new_version(0, 0, 310);
    dm_new_version(1, 2, 550);
    dm_new_version(3, 4, 1100);
    dm_new_version(5, 0, 220);

    print_str("\nTagging documents...\n");
    dm_tag_document(0, 0);
    dm_tag_document(0, 1);
    dm_tag_document(1, 0);
    dm_tag_document(2, 2);
    dm_tag_document(3, 1);
    dm_tag_document(3, 3);
    dm_tag_document(4, 2);
    dm_tag_document(5, 4);
    dm_tag_document(6, 0);
    dm_tag_document(7, 3);

    print_str("\nAdding comments...\n");
    dm_add_comment(0, 1);
    dm_add_comment(0, 2);
    dm_add_comment(1, 3);
    dm_add_comment(3, 0);
    dm_add_comment(3, 4);
    dm_add_comment(5, 1);
    dm_add_comment(7, 2);
    dm_add_comment(8, 3);

    print_str("\nResolving comments...\n");
    dm_resolve_comment(0);
    dm_resolve_comment(2);
    dm_resolve_comment(4);
    dm_resolve_comment(6);

    print_str("\nSharing documents...\n");
    dm_share_document(0, 1, 2);
    dm_share_document(0, 2, 2);
    dm_share_document(3, 0, 3);
    dm_share_document(5, 3, 2);

    print_str("\nCreating workflows...\n");
    dm_create_workflow(1, 3);
    dm_create_workflow(2, 4);
    dm_create_workflow(1, 5);

    print_str("\nAdvancing workflows...\n");
    dm_advance_workflow(0);
    dm_advance_workflow(0);
    dm_advance_workflow(0);
    dm_advance_workflow(1);
    dm_advance_workflow(1);
    dm_advance_workflow(2);
    dm_advance_workflow(2);

    print_str("\nSearching documents...\n");
    dm_search_documents(0, -1);
    dm_search_documents(-1, 0);
    dm_search_documents(2, -1);

    print_str("\nAudit log...\n");
    dm_audit_log();

    print_str("\nFinal state...\n");
    dm_print_state();
    print_str("\n=== Demo Complete ===\n");
    return 0;
}
