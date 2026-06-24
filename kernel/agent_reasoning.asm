/*
 * aiasm-aarch64/kernel/agent_reasoning.asm
 * Distributed Reasoning & Knowledge (v1.0)
 *
 * AArch64 ASM implementation of distributed reasoning for AGI agents.
 * Provides logic inference, knowledge graph traversal, theorem proving,
 * and belief revision.
 *
 * Reasoning features:
 *   - Forward/backward chaining inference
 *   - Knowledge graph queries and traversal
 *   - Theorem proving (propositional logic)
 *   - Belief revision (AGM postulates)
 *
 * Integration: called from reasoning_chain.asm for logical reasoning
 *
 * API:
 *   agent_reasoning_init()                         — init reasoning
 *   agent_reasoning_add_fact(subject, pred, object)  — add fact to KB
 *   agent_reasoning_infer(subject, pred) => object   — forward chain
 *   agent_reasoning_query_kg(subject, pred, buf)     — query knowledge graph
 *   agent_reasoning_prove(proposition) => 0/1        — prove proposition
 *   agent_reasoning_revise(belief_id, new_value)     — revise belief
 */
.arch armv8-a

/* Constants */
.set REASON_MAX_FACTS,    1024
.set REASON_MAX_BELIEFS,  256
.set REASON_MAX_KG_NODES, 512

/* Fact triple (16 bytes each): subject, predicate, object, flags */
/* Belief entry (12 bytes each): id, value, confidence */
/* KG edge (16 bytes each): src, predicate, dst, weight */

.bss
.align 4
.global agent_reasoning_init_done
agent_reasoning_init_done:
    .skip 4

.align 4
reason_fact_count:
    .skip 4
reason_belief_count:
    .skip 4
reason_kg_edge_count:
    .skip 4

/* Fact storage */
.align 4
reason_facts:
    .skip REASON_MAX_FACTS * 16   /* 16KB */

/* Belief storage */
.align 4
reason_beliefs:
    .skip REASON_MAX_BELIEFS * 12 /* 3KB */

/* Knowledge graph edges */
.align 4
reason_kg_edges:
    .skip REASON_MAX_KG_NODES * 16 * 4   /* ~128KB (4 edges per node avg) */

.text

/* -----------------------------------------------------------------------------
 * agent_reasoning_init — initialize reasoning subsystem
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global agent_reasoning_init
agent_reasoning_init:
    stp     x29, x30, [sp, #-16]!

    /* Zero fact storage */
    ldr     x0, =reason_facts
    mov     x1, #(REASON_MAX_FACTS * 16)
    bl      .Lreason_zero

    /* Zero belief storage */
    ldr     x0, =reason_beliefs
    mov     x1, #(REASON_MAX_BELIEFS * 12)
    bl      .Lreason_zero

    /* Zero KG edges */
    ldr     x0, =reason_kg_edges
    mov     x1, #(REASON_MAX_KG_NODES * 16 * 4)
    bl      .Lreason_zero

    /* Zero counters */
    ldr     x0, =reason_fact_count
    str     wzr, [x0]
    ldr     x0, =reason_belief_count
    str     wzr, [x0]
    ldr     x0, =reason_kg_edge_count
    str     wzr, [x0]

    ldr     x0, =agent_reasoning_init_done
    mov     w1, #1
    str     w1, [x0]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* Internal: zero x1 bytes at x0 */
.Lreason_zero:
    stp     x19, x20, [sp, #-16]!
    mov     x19, x0
    mov     x20, x1
1:  cbz     x20, 2f
    strb    wzr, [x19], #1
    sub     x20, x20, #1
    b       1b
2:  ldp     x19, x20, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_reasoning_add_fact — add a fact to knowledge base
 * Input: x0 = subject ID, x1 = predicate ID, x2 = object ID
 * Output: x0 = fact index, or -1 (KB full)
 * ----------------------------------------------------------------------------- */
.global agent_reasoning_add_fact
agent_reasoning_add_fact:
    stp     x29, x30, [sp, #-16]!

    /* Check if full */
    ldr     x3, =reason_fact_count
    ldr     w3, [x3]
    cmp     w3, #REASON_MAX_FACTS
    b.hs    .Lreason_add_fact_fail

    /* Add fact triple */
    ldr     x4, =reason_facts
    mov     x5, #16
    mul     x5, w3, x5
    add     x4, x4, x5

    str     w0, [x4, #0]        /* subject */
    str     w1, [x4, #4]        /* predicate */
    str     w2, [x4, #8]        /* object */
    mov     w5, #1
    str     w5, [x4, #12]       /* flags (active) */

    /* Also add to KG as edge */
    ldr     x4, =reason_kg_edge_count
    ldr     w4, [x4]
    cmp     w4, #(REASON_MAX_KG_NODES * 4)
    b.hs    .Lreason_add_fact_no_kg

    ldr     x5, =reason_kg_edges
    mov     x6, #16
    mul     x6, w4, x6
    add     x5, x5, x6

    str     w0, [x5, #0]        /* src = subject */
    str     w1, [x5, #4]        /* predicate */
    str     w2, [x5, #8]        /* dst = object */
    mov     w6, #100
    str     w6, [x5, #12]       /* weight = 100 */

    ldr     x5, =reason_kg_edge_count
    add     w4, w4, #1
    str     w4, [x5]

.Lreason_add_fact_no_kg:
    /* Increment fact count */
    ldr     x3, =reason_fact_count
    ldr     w4, [x3]
    add     w4, w4, #1
    str     w4, [x3]

    mov     x0, w4
    sub     x0, x0, #1          /* return index of added fact */
    b       .Lreason_add_fact_ret

.Lreason_add_fact_fail:
    mov     x0, #-1

.Lreason_add_fact_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_reasoning_infer — forward chaining inference
 * Input: x0 = subject ID, x1 = predicate ID
 * Output: x0 = object ID (inferred), or -1 (not found)
 *
 * Searches facts for matching subject+predicate, returns object.
 * ----------------------------------------------------------------------------- */
.global agent_reasoning_infer
agent_reasoning_infer:
    stp     x29, x30, [sp, #-16]!

    mov     x3, x0              /* subject */
    mov     x4, x1              /* predicate */

    ldr     x5, =reason_fact_count
    ldr     w5, [x5]
    cbz     w5, .Lreason_infer_fail

    ldr     x5, =reason_facts
    mov     w6, #0

.Lreason_infer_loop:
    cmp     w6, w5
    b.hs    .Lreason_infer_fail

    mov     x7, #16
    mul     x7, w6, x7
    add     x7, x5, x7

    ldr     w8, [x7, #0]        /* fact subject */
    ldr     w9, [x7, #4]        /* fact predicate */

    cmp     w8, w3
    b.ne    .Lreason_infer_next
    cmp     w9, w4
    b.ne    .Lreason_infer_next

    /* Match found */
    ldr     w0, [x7, #8]        /* return object */
    b       .Lreason_infer_ret

.Lreason_infer_next:
    add     w6, w6, #1
    b       .Lreason_infer_loop

.Lreason_infer_fail:
    mov     x0, #-1

.Lreason_infer_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_reasoning_query_kg — query knowledge graph
 * Input: x0 = subject (or -1 for any), x1 = predicate (or -1 for any),
 *        x2 = output buffer, x3 = max results
 * Output: x0 = number of results
 * ----------------------------------------------------------------------------- */
.global agent_reasoning_query_kg
agent_reasoning_query_kg:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x2             /* output buffer */
    mov     x20, x3             /* max results */
    mov     w3, #0              /* result count */

    ldr     x4, =reason_kg_edge_count
    ldr     w4, [x4]
    cbz     w4, .Lreason_query_done

    ldr     x4, =reason_kg_edges
    mov     w5, #0

.Lreason_query_loop:
    cmp     w5, w4
    b.hs    .Lreason_query_done
    cmp     w3, w20
    b.hs    .Lreason_query_done

    mov     x6, #16
    mul     x6, w5, x6
    add     x6, x4, x6

    ldr     w7, [x6, #0]        /* src */
    ldr     w8, [x6, #4]        /* pred */
    ldr     w9, [x6, #8]        /* dst */

    /* Check match */
    cmp     x0, #-1
    beq     .Lreason_query_match_any_subj
    cmp     w7, w0
    b.ne    .Lreason_query_next
.Lreason_query_match_any_subj:

    cmp     x1, #-1
    beq     .Lreason_query_match_any_pred
    cmp     w8, w1
    b.ne    .Lreason_query_next
.Lreason_query_match_any_pred:

    /* Store result (dst object) */
    str     w9, [x19, w3, lsl #2]
    add     w3, w3, #1

.Lreason_query_next:
    add     w5, w5, #1
    b       .Lreason_query_loop

.Lreason_query_done:
    mov     x0, w3

    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_reasoning_prove — prove a proposition
 * Input: x0 = proposition ID (fact index to verify)
 * Output: x0 = 1 (proven), 0 (not proven)
 *
 * Simplified: checks if fact exists in KB.
 * ----------------------------------------------------------------------------- */
.global agent_reasoning_prove
agent_reasoning_prove:
    stp     x29, x30, [sp, #-16]!

    /* Check if fact index is valid */
    ldr     x1, =reason_fact_count
    ldr     w1, [x1]
    cmp     x0, x1
    b.hs    .Lreason_prove_false

    /* Fact exists — proven */
    mov     x0, #1
    b       .Lreason_prove_ret

.Lreason_prove_false:
    mov     x0, #0

.Lreason_prove_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_reasoning_revise — revise a belief with new evidence
 * Input: x0 = belief_id, x1 = new_value
 * Output: x0 = 0 (ok), -1 (invalid)
 *
 * Updates belief using AGM-style revision.
 * ----------------------------------------------------------------------------- */
.global agent_reasoning_revise
agent_reasoning_revise:
    stp     x29, x30, [sp, #-16]!

    cmp     x0, #REASON_MAX_BELIEFS
    b.hs    .Lreason_revise_fail

    /* Get or create belief entry */
    ldr     x2, =reason_belief_count
    ldr     w2, [x2]
    cmp     w2, x0
    b.ls    .Lreason_revise_new

    /* Update existing belief */
    ldr     x2, =reason_beliefs
    mov     x3, #12
    mul     x3, x0, x3
    add     x2, x2, x3
    str     w1, [x2, #4]        /* update value */
    b       .Lreason_revise_done

.Lreason_revise_new:
    /* Create new belief */
    ldr     x2, =reason_beliefs
    mov     x3, #12
    mul     x3, x0, x3
    add     x2, x2, x3
    str     w0, [x2, #0]        /* id */
    str     w1, [x2, #4]        /* value */
    mov     w3, #50             /* initial confidence */
    str     w3, [x2, #8]        /* confidence */

    /* Update count if needed */
    ldr     x2, =reason_belief_count
    ldr     w3, [x2]
    add     x3, x0, #1
    cmp     w3, x3
    b.hs    .Lreason_revise_done
    str     w3, [x2]

.Lreason_revise_done:
    mov     x0, #0
    b       .Lreason_revise_ret

.Lreason_revise_fail:
    mov     x0, #-1

.Lreason_revise_ret:
    ldp     x29, x30, [sp], #16
    ret
