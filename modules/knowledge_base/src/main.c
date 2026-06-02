/* knowledge_base: Knowledge base management (v38.0) */

#include <stddef.h>

extern unsigned int host_alloc(unsigned int size, unsigned int align);
extern void host_print(const char *str);
extern void host_exit(int code);
extern int host_get_argv(unsigned int buf_off, unsigned int max_len);

/* Limits */
#define MAX_ENTRIES     256
#define MAX_TITLE       128
#define MAX_CONTENT     1024
#define MAX_CATEGORY    64
#define MAX_TAGS        8
#define MAX_TAG_LEN     32
#define MAX_IMPORT      128

/* KB entry */
typedef struct {
    int   entry_id;
    char  title[MAX_TITLE];
    char  content[MAX_CONTENT];
    char  category[MAX_CATEGORY];
    char  tags[MAX_TAGS][MAX_TAG_LEN];
    int   tag_count;
    int   created_at;
    int   updated_at;
    int   views;
    int   deleted;
} kb_entry_t;

/* KB stats */
typedef struct {
    int total_entries;
    int active_entries;
    int deleted_entries;
    int categories;
    int total_views;
    int total_tags;
} kb_stats_t;

static kb_entry_t entries[MAX_ENTRIES];
static int entry_count = 0;
static int next_id = 1;
static int clock = 0;

/* String utilities */
static int my_strlen(const char *s) { int l = 0; while (s[l]) l++; return l; }
static int my_strcmp(const char *a, const char *b) {
    while (*a && *b) { if (*a != *b) return *a - *b; a++; b++; } return *a - *b;
}
static int my_strncmp(const char *a, const char *b, int n) {
    for (int i = 0; i < n; i++) {
        if (!a[i] || !b[i]) return (a[i] ? 1 : 0) - (b[i] ? 1 : 0);
        if (a[i] != b[i]) return a[i] - b[i];
    }
    return 0;
}
static void my_strncpy(char *dst, const char *src, int n) {
    int i = 0; while (i < n - 1 && src[i]) { dst[i] = src[i]; i++; } dst[i] = '\0';
}
static int my_strstr(const char *haystack, const char *needle) {
    int hl = my_strlen(haystack), nl = my_strlen(needle);
    if (nl == 0 || nl > hl) return 0;
    for (int i = 0; i <= hl - nl; i++) {
        int j = 0; while (j < nl && haystack[i+j] == needle[j]) j++;
        if (j == nl) return 1;
    }
    return 0;
}

static void print_str(const char *str) { host_print(str); }
static void print_int(int val) {
    char buf[32]; int pos = 0;
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    if (val == 0) buf[pos++] = '0';
    else { int d = 0, t = val; while (t > 0) { d++; t /= 10; } pos += d; buf[pos] = '\0'; pos--;
        while (val > 0) { buf[pos--] = '0' + (val % 10); val /= 10; } }
    host_print(buf);
}

/* Create entry */
int kb_create(const char *title, const char *content, const char *category) {
    if (entry_count >= MAX_ENTRIES) return -1;
    int idx = entry_count++;
    entries[idx].entry_id = next_id++;
    my_strncpy(entries[idx].title, title, MAX_TITLE - 1);
    my_strncpy(entries[idx].content, content, MAX_CONTENT - 1);
    my_strncpy(entries[idx].category, category, MAX_CATEGORY - 1);
    entries[idx].tag_count = 0;
    entries[idx].created_at = ++clock;
    entries[idx].updated_at = clock;
    entries[idx].views = 0;
    entries[idx].deleted = 0;
    return entries[idx].entry_id;
}

/* Add tag to entry */
int kb_add_tag(int entry_id, const char *tag) {
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].entry_id == entry_id && !entries[i].deleted) {
            if (entries[i].tag_count >= MAX_TAGS) return -1;
            int t = entries[i].tag_count;
            my_strncpy(entries[i].tags[t], tag, MAX_TAG_LEN - 1);
            entries[i].tag_count++;
            entries[i].updated_at = ++clock;
            return 0;
        }
    }
    return -1;
}

/* Update entry */
int kb_update(int entry_id, const char *title, const char *content, const char *category) {
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].entry_id == entry_id && !entries[i].deleted) {
            if (title) my_strncpy(entries[i].title, title, MAX_TITLE - 1);
            if (content) my_strncpy(entries[i].content, content, MAX_CONTENT - 1);
            if (category) my_strncpy(entries[i].category, category, MAX_CATEGORY - 1);
            entries[i].updated_at = ++clock;
            return 0;
        }
    }
    return -1;
}

/* Delete entry (soft) */
int kb_delete(int entry_id) {
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].entry_id == entry_id && !entries[i].deleted) {
            entries[i].deleted = 1;
            entries[i].updated_at = ++clock;
            return 0;
        }
    }
    return -1;
}

/* Get entry */
int kb_get(int entry_id, kb_entry_t *out) {
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].entry_id == entry_id && !entries[i].deleted) {
            out->entry_id = entries[i].entry_id;
            my_strncpy(out->title, entries[i].title, MAX_TITLE - 1);
            my_strncpy(out->content, entries[i].content, MAX_CONTENT - 1);
            my_strncpy(out->category, entries[i].category, MAX_CATEGORY - 1);
            out->tag_count = entries[i].tag_count;
            for (int t = 0; t < entries[i].tag_count; t++)
                my_strncpy(out->tags[t], entries[i].tags[t], MAX_TAG_LEN - 1);
            out->created_at = entries[i].created_at;
            out->updated_at = entries[i].updated_at;
            out->views = entries[i].views + 1; /* increment view count */
            entries[i].views = out->views;
            return 0;
        }
    }
    return -1;
}

/* Search by category */
int kb_search_category(const char *category, int *result_ids, int max_results) {
    int found = 0;
    for (int i = 0; i < entry_count && found < max_results; i++) {
        if (!entries[i].deleted && my_strcmp(entries[i].category, category) == 0) {
            result_ids[found++] = entries[i].entry_id;
        }
    }
    return found;
}

/* Search by keyword */
int kb_search_keyword(const char *keyword, int *result_ids, int max_results) {
    int found = 0;
    for (int i = 0; i < entry_count && found < max_results; i++) {
        if (entries[i].deleted) continue;
        if (my_strstr(entries[i].title, keyword) ||
            my_strstr(entries[i].content, keyword)) {
            result_ids[found++] = entries[i].entry_id;
        } else {
            for (int t = 0; t < entries[i].tag_count; t++) {
                if (my_strstr(entries[i].tags[t], keyword)) {
                    result_ids[found++] = entries[i].entry_id;
                    break;
                }
            }
        }
    }
    return found;
}

/* List categories */
int kb_list_categories(char categories[][MAX_CATEGORY], int max_cats) {
    int count = 0;
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].deleted) continue;
        /* Check if already listed */
        int dup = 0;
        for (int c = 0; c < count; c++) {
            if (my_strcmp(categories[c], entries[i].category) == 0) { dup = 1; break; }
        }
        if (!dup && count < max_cats) {
            my_strncpy(categories[count], entries[i].category, MAX_CATEGORY - 1);
            count++;
        }
    }
    return count;
}

/* Get statistics */
int kb_get_stats(kb_stats_t *stats) {
    stats->total_entries = entry_count;
    stats->active_entries = 0;
    stats->deleted_entries = 0;
    stats->total_views = 0;
    stats->total_tags = 0;
    char cats[MAX_ENTRIES][MAX_CATEGORY];
    stats->categories = kb_list_categories(cats, MAX_ENTRIES);
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].deleted) stats->deleted_entries++;
        else {
            stats->active_entries++;
            stats->total_views += entries[i].views;
            stats->total_tags += entries[i].tag_count;
        }
    }
    return stats->active_entries;
}

/* Import from text (simple key=value format) */
int kb_import(const char *data, const char *default_category) {
    int imported = 0;
    int len = my_strlen(data);
    int pos = 0;
    while (pos < len && imported < MAX_IMPORT) {
        /* Skip whitespace */
        while (pos < len && (data[pos] == '\n' || data[pos] == '\r' || data[pos] == ' ')) pos++;
        if (pos >= len) break;
        /* Read title (until '=') */
        char title[MAX_TITLE]; int tl = 0;
        while (pos < len && data[pos] != '=' && tl < MAX_TITLE - 1) title[tl++] = data[pos++];
        title[tl] = '\0';
        if (data[pos] == '=') pos++; /* skip '=' */
        /* Read content (until newline) */
        char content[MAX_CONTENT]; int cl = 0;
        while (pos < len && data[pos] != '\n' && cl < MAX_CONTENT - 1) content[cl++] = data[pos++];
        content[cl] = '\0';
        if (tl > 0 && cl > 0) {
            kb_create(title, content, default_category);
            imported++;
        }
    }
    return imported;
}

/* List all entries */
int kb_list(void) {
    print_str("  KB Entries\n");
    print_str("  ==========================================================\n");
    for (int i = 0; i < entry_count; i++) {
        if (entries[i].deleted) continue;
        print_str("  #"); print_int(entries[i].entry_id);
        print_str(" ["); print_str(entries[i].category); print_str("] ");
        print_str(entries[i].title);
        print_str(" (views="); print_int(entries[i].views);
        print_str(", tags="); print_int(entries[i].tag_count);
        print_str(")\n");
    }
    return entry_count;
}

/* ===== CLI entry ===== */
void _start(void) {
    unsigned int buf = host_alloc(512, 16);
    host_get_argv(buf, 512);
    int help = 0, test = 0;
    unsigned int pos = 0;
    char *argv_ptr = (char *)buf;
    while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    while (pos < 512 && argv_ptr[pos]) {
        char *arg = &argv_ptr[pos];
        if (my_strcmp(arg, "-h") == 0 || my_strcmp(arg, "--help") == 0) help = 1;
        else if (my_strcmp(arg, "-t") == 0 || my_strcmp(arg, "--test") == 0) test = 1;
        while (pos < 512 && argv_ptr[pos]) pos++; pos++;
    }
    print_str("Knowledge Base v38.0 - Knowledge Base Management\n");
    if (help) { print_str("Usage: knowledge_base [options]\n  -h, --help    Show help\n  -t, --test    Run test\n"); return; }
    if (test) {
        print_str("=== Knowledge Base Test ===\n\n");
        /* CRUD operations */
        int id1 = kb_create("What is AI?", "Artificial Intelligence (AI) is the simulation of human intelligence by machines.", "AI_Basics");
        int id2 = kb_create("Machine Learning Overview", "ML is a subset of AI that enables systems to learn from data.", "AI_Basics");
        int id3 = kb_create("Neural Networks", "Neural networks are computing systems inspired by biological brains.", "Deep_Learning");
        int id4 = kb_create("Natural Language Processing", "NLP enables computers to understand and generate human language.", "AI_Applications");
        print_str("Created "); print_int(entry_count); print_str(" entries\n\n");
        /* Add tags */
        kb_add_tag(id1, "ai"); kb_add_tag(id1, "basics");
        kb_add_tag(id2, "ml"); kb_add_tag(id2, "data");
        kb_add_tag(id3, "neural"); kb_add_tag(id3, "deep-learning");
        kb_add_tag(id4, "nlp"); kb_add_tag(id4, "language");
        /* List */
        kb_list();
        print_str("\n");
        /* Search */
        int results[16];
        int found = kb_search_keyword("learning", results, 16);
        print_str("Search 'learning': found "); print_int(found); print_str(" entries\n");
        found = kb_search_category("AI_Basics", results, 16);
        print_str("Category 'AI_Basics': "); print_int(found); print_str(" entries\n\n");
        /* Update */
        kb_update(id1, "What is AI? (Updated)", NULL, NULL);
        print_str("Updated entry #"); print_int(id1); print_str("\n");
        /* Delete */
        kb_delete(id4);
        print_str("Deleted entry #"); print_int(id4); print_str("\n\n");
        /* Import */
        const char *import_data = "Transformer Models=Architecture using self-attention mechanisms for sequence processing.\nGANs=Generative Adversarial Networks generate realistic data through competing networks.";
        int imported = kb_import(import_data, "AI_Models");
        print_str("Imported "); print_int(imported); print_str(" entries\n\n");
        /* Stats */
        kb_stats_t stats;
        kb_get_stats(&stats);
        print_str("  Statistics\n");
        print_str("  ==========================================================\n");
        print_str("  Active entries: "); print_int(stats.active_entries); print_str("\n");
        print_str("  Deleted entries: "); print_int(stats.deleted_entries); print_str("\n");
        print_str("  Categories: "); print_int(stats.categories); print_str("\n");
        print_str("  Total views: "); print_int(stats.total_views); print_str("\n");
        print_str("  Total tags: "); print_int(stats.total_tags); print_str("\n");
        print_str("\n=== Knowledge Base Test Complete ===\n");
        return;
    }
    print_str("Use -h for help, -t for test\n");
}
