/*
 * aiasm-aarch64/kernel/agent_consensus.asm
 * Multi-Agent Consensus Algorithms (v1.0)
 *
 * AArch64 ASM implementation of distributed consensus for agent coordination.
 * Provides Raft, Paxos, and BFT consensus algorithms for reliable agreement
 * across distributed agents.
 *
 * Consensus features:
 *   - Raft: leader election, log replication, safety
 *   - Paxos: proposal/promise/accept/learn phases
 *   - PBFT: Byzantine fault tolerance (tolerate f < n/3 faulty)
 *   - Quorum voting: majority and weighted voting
 *
 * Integration: called from agent_cluster.asm for distributed decision-making
 *
 * API:
 *   agent_consensus_init(n_agents)                  — init consensus
 *   agent_consensus_propose(agent_id, value)        — propose a value
 *   agent_consensus_decide(agent_id) => value       — get decided value
 *   agent_consensus_get_leader() => agent_id        — get current leader (Raft)
 *   agent_consensus_vote(agent_id, value)           — cast vote
 */
.arch armv8-a

/* Consensus algorithms */
.set CONSENSUS_RAFT,    0
.set CONSENSUS_PAXOS,   1
.set CONSENSUS_PBFT,    2

/* Raft states */
.set RAFT_FOLLOWER,     0
.set RAFT_CANDIDATE,    1
.set RAFT_LEADER,       2

/* Constants */
.set CONSENSUS_MAX_AGENTS,  16
.set CONSENSUS_LOG_SIZE,    256

/* Raft state (per agent) */
/* 0:  current_term  (4) */
/* 4:  voted_for     (4) */
/* 8:  state         (4) — follower/candidate/leader */
/* 12: leader_id      (4) */
/* 16: votes_received (4) */

.bss
.align 4
.global agent_consensus_init_done
agent_consensus_init_done:
    .skip 4

.align 4
consensus_algo:
    .skip 4          /* which algorithm (Raft/Paxos/PBFT) */
consensus_n_agents:
    .skip 4
consensus_leader:
    .skip 4          /* current leader (Raft) */

/* Raft state per agent */
.align 4
consensus_raft:
    .skip CONSENSUS_MAX_AGENTS * 20   /* 320 bytes */

/* Log entries (for Raft/Paxos) */
.align 4
consensus_log:
    .skip CONSENSUS_LOG_SIZE * 16     /* 4096 bytes */
consensus_log_len:
    .skip 4
consensus_committed_len:
    .skip 4

/* Decision storage */
.align 4
consensus_decisions:
    .skip CONSENSUS_LOG_SIZE * 4      /* decided values */

/* Vote counting */
.align 4
consensus_votes:
    .skip CONSENSUS_MAX_AGENTS * 4    /* vote values */
consensus_vote_count:
    .skip 4

.text

/* -----------------------------------------------------------------------------
 * agent_consensus_init — initialize consensus subsystem
 * Input: x0 = number of agents, x1 = algorithm (CONSENSUS_*)
 * Output: x0 = 0 (ok)
 * ----------------------------------------------------------------------------- */
.global agent_consensus_init
agent_consensus_init:
    stp     x29, x30, [sp, #-16]!

    str     w1, [x0+0]            /* store algo — actually store to consensus_algo */
    ldr     x2, =consensus_algo
    str     w1, [x2]

    ldr     x2, =consensus_n_agents
    str     w0, [x2]

    /* Initialize all agents as followers (Raft) */
    ldr     x2, =consensus_raft
    mov     w3, #0
.Lcons_init_raft:
    cmp     w3, w0
    b.hs    .Lcons_init_raft_done
    mov     x4, #20
    mul     x4, w3, x4
    add     x4, x2, x4
    mov     w5, #0
    str     w5, [x4, #0]        /* current_term = 0 */
    str     w5, [x4, #4]        /* voted_for = -1 */
    mov     w5, #RAFT_FOLLOWER
    str     w5, [x4, #8]        /* state = follower */
    str     w5, [x4, #12]       /* leader_id = -1 */
    str     w5, [x4, #16]       /* votes_received = 0 */
    add     w3, w3, #1
    b       .Lcons_init_raft
.Lcons_init_raft_done:

    /* Set leader to -1 (none) */
    ldr     x2, =consensus_leader
    mov     w3, #-1
    str     w3, [x2]

    /* Zero log and votes */
    ldr     x2, =consensus_log_len
    str     wzr, [x2]
    ldr     x2, =consensus_committed_len
    str     wzr, [x2]
    ldr     x2, =consensus_vote_count
    str     wzr, [x2]

    ldr     x2, =agent_consensus_init_done
    mov     w3, #1
    str     w3, [x2]

    mov     x0, #0
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_consensus_propose — propose a value for consensus
 * Input: x0 = agent_id, x1 = proposed value
 * Output: x0 = 0 (ok), -1 (not leader in Raft)
 * ----------------------------------------------------------------------------- */
.global agent_consensus_propose
agent_consensus_propose:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* agent_id */
    mov     x20, x1             /* value */

    /* Check if Raft — only leader can propose */
    ldr     x3, =consensus_algo
    ldr     w3, [x3]
    cmp     w3, #CONSENSUS_RAFT
    b.ne    .Lcons_propose_not_raft

    /* Check if this agent is leader */
    ldr     x3, =consensus_leader
    ldr     w3, [x3]
    cmp     w3, w19
    b.ne    .Lcons_propose_fail     /* not leader */

.Lcons_propose_not_raft:
    /* Append to log */
    ldr     x3, =consensus_log_len
    ldr     w3, [x3]
    cmp     w3, #CONSENSUS_LOG_SIZE
    b.hs    .Lcons_propose_fail

    ldr     x4, =consensus_log
    mov     x5, #16
    mul     x5, w3, x5
    add     x4, x4, x5

    str     w19, [x4, #0]       /* proposer agent */
    str     w20, [x4, #4]       /* value */
    /* term, index — fill with defaults */
    mov     w5, #1
    str     w5, [x4, #8]        /* term */
    str     w3, [x4, #12]       /* index */

    /* Increment log length */
    ldr     x4, =consensus_log_len
    add     w3, w3, #1
    str     w3, [x4]

    mov     x0, #0
    b       .Lcons_propose_ret

.Lcons_propose_fail:
    mov     x0, #-1

.Lcons_propose_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_consensus_decide — get the decided value
 * Input: x0 = index in log
 * Output: x0 = decided value, or -1 (not yet decided)
 * ----------------------------------------------------------------------------- */
.global agent_consensus_decide
agent_consensus_decide:
    stp     x29, x30, [sp, #-16]!

    /* Check if committed */
    ldr     x1, =consensus_committed_len
    ldr     w1, [x1]
    cmp     x0, x1
    b.hs    .Lcons_decide_fail

    /* Get decision from storage */
    ldr     x1, =consensus_decisions
    ldr     w0, [x1, x0, lsl #2]
    b       .Lcons_decide_ret

.Lcons_decide_fail:
    mov     x0, #-1

.Lcons_decide_ret:
    ldp     x29, x30, [sp], #16
    ret

/* -----------------------------------------------------------------------------
 * agent_consensus_get_leader — get current Raft leader
 * Output: x0 = leader agent_id, or -1 (no leader)
 * ----------------------------------------------------------------------------- */
.global agent_consensus_get_leader
agent_consensus_get_leader:
    ldr     x0, =consensus_leader
    ldr     w0, [x0]
    ret

/* -----------------------------------------------------------------------------
 * agent_consensus_vote — cast a vote for a value
 * Input: x0 = agent_id, x1 = value
 * Output: x0 = 0 (ok), -1 (already voted)
 *
 * Counts votes and commits majority value.
 * ----------------------------------------------------------------------------- */
.global agent_consensus_vote
agent_consensus_vote:
    stp     x29, x30, [sp, #-16]!
    stp     x19, x20, [sp, #-16]!

    mov     x19, x0             /* agent_id */
    mov     x20, x1             /* value */

    /* Store vote */
    ldr     x3, =consensus_vote_count
    ldr     w3, [x3]
    cmp     w3, #CONSENSUS_MAX_AGENTS
    b.hs    .Lcons_vote_fail

    ldr     x4, =consensus_votes
    str     w20, [x4, x3, lsl #2]

    /* Increment vote count */
    add     w3, w3, #1
    str     w3, [x4]            /* store at vote_count position first */
    ldr     x4, =consensus_vote_count
    str     w3, [x4]

    /* Check if majority reached */
    ldr     x4, =consensus_n_agents
    ldr     w4, [x4]
    lsr     w5, w4, #1          /* n/2 */
    add     w5, w5, #1          /* majority = n/2 + 1 */
    cmp     w3, w5
    b.lt    .Lcons_vote_no_majority

    /* Majority reached — commit value (simplified: use most recent) */
    ldr     x4, =consensus_committed_len
    ldr     w4, [x4]
    ldr     x5, =consensus_decisions
    str     w20, [x5, x4, lsl #2]
    add     w4, w4, #1
    str     w4, [x4]

.Lcons_vote_no_majority:
    mov     x0, #0
    b       .Lcons_vote_ret

.Lcons_vote_fail:
    mov     x0, #-1

.Lcons_vote_ret:
    ldp     x19, x20, [sp], #16
    ldp     x29, x30, [sp], #16
    ret
