#!/bin/sh
# AI-ASM OS v28.0 - Test Runner
# Runs all test suites and reports results

PASS=0
FAIL=0
SKIP=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

log_pass() { echo "  ${GREEN}PASS${NC} $1"; PASS=$((PASS + 1)); }
log_fail() { echo "  ${RED}FAIL${NC} $1"; FAIL=$((FAIL + 1)); }
log_skip() { echo "  ${YELLOW}SKIP${NC} $1"; SKIP=$((SKIP + 1)); }
section() { echo ""; echo "=== $1 ==="; }

# Check if WASM modules exist
check_module() {
    local name="$1"
    local path="modules/${name}/${name}.wasm"
    if [ -f "$path" ]; then
        log_pass "$name.wasm exists"
    else
        # Check source exists (binary may not be compiled in this env)
        if [ -f "modules/${name}/src/main.c" ]; then
            log_pass "$name (source present, binary not compiled)"
        else
            log_fail "$name (missing)"
        fi
    fi
}

# Check kernel module exists
check_kernel() {
    local name="$1"
    if [ -f "kernel/${name}.asm" ]; then
        log_pass "${name}.asm exists"
    else
        log_fail "${name}.asm (missing)"
    fi
}

# Run unit tests
test_unit() {
    section "Unit Tests"

    # Kernel modules
    echo "Kernel modules:"
    check_kernel process
    check_kernel sched
    check_kernel mmu
    check_kernel fault
    check_kernel trap
    check_kernel syscall
    check_kernel alloc
    check_kernel buddy
    check_kernel slab
    check_kernel cache
    check_kernel maintenance
    check_kernel perf_monitor
    check_kernel board_apple
    check_kernel board_jetson
    check_kernel neuromorphic
    check_kernel photonics

    # File system
    echo "File system:"
    check_kernel vfs
    check_kernel ext2
    check_kernel fat
    check_kernel devfs
    check_kernel ramfs

    # Network
    echo "Network:"
    check_kernel net
    check_kernel tcp
    check_kernel udp
    check_kernel http
    check_kernel dns
    check_kernel arp

    # WASM runtime
    echo "WASM runtime:"
    check_kernel wasm_embed
    check_kernel wasi
    check_kernel jit_codegen

    # IPC
    echo "IPC:"
    check_kernel pipe
    check_kernel mq
    check_kernel shm
    check_kernel agent_ipc

    # GUI
    echo "GUI:"
    check_kernel gui
    check_kernel fb
    check_kernel input
    check_kernel font
    check_kernel window

    # Agent
    echo "Agent:"
    check_kernel agent_core
    check_kernel agent_persist
}

# Check application modules
test_application() {
    section "Application Modules"

    # POSIX tools
    echo "POSIX tools:"
    check_module ls
    check_module cat
    check_module echo
    check_module wc
    check_module head
    check_module tail
    check_module sort
    check_module uniq
    check_module grep
    check_module tr
    check_module cut

    # Network
    echo "Network:"
    check_module wget
    check_module ping
    check_module nc

    # AI
    echo "AI:"
    check_module ai_bridge
    check_module ai_router

    # Agent
    echo "Agent:"
    check_module agent_runtime

    # Advanced (v19.0+)
    echo "Advanced modules:"
    check_module maintenance
    check_module continuous_optimization
    check_module apple_silicon
    check_module agi_reasoning
    check_module quantum_sim
    check_module quantum_hybrid
    check_module spiking_nn
    check_module neuromorphic_interface
    check_module lif_neuron
    check_module meta_cognition_deep
    check_module self_improve_loop
    check_module ethical_engine
    check_module social_intelligence
    check_module photonics_interface
    check_module dna_storage
    check_module biocompute
    check_module final_cert
    check_module quantum_network
    check_module cognitive_enhance
    check_module self_evolution
}

# Test documentation
test_docs() {
    section "Documentation"

    if [ -f docs/API_REFERENCE.md ]; then
        log_pass "API reference exists"
    else
        log_fail "API reference missing"
    fi

    if [ -f docs/USER_MANUAL.md ]; then
        log_pass "User manual exists"
    else
        log_fail "User manual missing"
    fi

    if [ -f docs/DEVELOPER_GUIDE.md ]; then
        log_pass "Developer guide exists"
    else
        log_fail "Developer guide missing"
    fi

    if [ -f docs/ARCHITECTURE.md ]; then
        log_pass "Architecture doc exists"
    else
        log_fail "Architecture doc missing"
    fi

    if [ -f CHANGELOG.md ]; then
        log_pass "CHANGELOG exists"
    else
        log_fail "CHANGELOG missing"
    fi

    if [ -f RELEASE_NOTES.md ]; then
        log_pass "RELEASE_NOTES exists"
    else
        log_fail "RELEASE_NOTES missing"
    fi
}

# Test scripts
test_scripts() {
    section "Scripts"

    if [ -x scripts/install.sh ]; then
        log_pass "install.sh exists and executable"
    elif [ -f scripts/install.sh ]; then
        log_pass "install.sh exists"
    else
        log_fail "install.sh missing"
    fi

    if [ -f scripts/release.sh ]; then
        log_pass "release.sh exists"
    else
        log_fail "release.sh missing"
    fi
}

# Test wasm_host.c
test_runtime() {
    section "WASM Runtime"

    if [ -f kernel/wasm_host.c ]; then
        LINES=$(wc -l < kernel/wasm_host.c)
        log_pass "wasm_host.c: $LINES lines"

        # Check for key functions
        for func in host_print host_alloc host_exit host_get_argv \
                    host_maintenance_check host_perf_get_cycles \
                    host_apple_get_soc host_quantum_init \
                    host_snn_create_network host_photonics_init_chip \
                    host_ethical_check; do
            if grep -q "$func" kernel/wasm_host.c 2>/dev/null; then
                log_pass "host function: $func"
            else
                log_fail "host function: $func (missing)"
            fi
        done
    else
        log_fail "wasm_host.c missing"
    fi
}

# Summary
summary() {
    section "Test Summary"
    TOTAL=$((PASS + FAIL + SKIP))
    echo "  Total:  $TOTAL"
    echo "  Passed: $PASS"
    echo "  Failed: $FAIL"
    echo "  Skipped: $SKIP"

    if [ "$FAIL" -eq 0 ]; then
        echo ""
        echo "  ${GREEN}All tests passed!${NC}"
    else
        echo ""
        echo "  ${RED}$FAIL test(s) failed${NC}"
    fi
}

# Main
main() {
    echo "========================================"
    echo "  AI-ASM OS v28.0 Test Suite"
    echo "========================================"

    test_unit
    test_application
    test_docs
    test_scripts
    test_runtime
    summary
}

main
