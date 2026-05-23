#!/usr/bin/env python3
"""
Merge duplicate WASM sections to produce a WASM 1.0 compatible binary.

Usage: python3 wasm_merge.py input.wasm output.wasm
"""
import sys

def read_leb128(data, pos):
    result = 0
    shift = 0
    while True:
        b = data[pos]; pos += 1
        result |= (b & 0x7f) << shift
        if b & 0x80 == 0:
            break
        shift += 7
    return result, pos

def write_leb128(val):
    out = bytearray()
    while val > 0x7f:
        out.append((val & 0x7f) | 0x80)
        val >>= 7
    out.append(val)
    return bytes(out)

def merge_func_sections(parts):
    """Merge multiple func sections. Each has: count (LEB) + type indices."""
    total_count = 0
    type_indices = bytearray()
    for part in parts:
        count, pos = read_leb128(part, 0)
        total_count += count
        type_indices.extend(part[pos:])
    result = bytearray()
    result.extend(write_leb128(total_count))
    result.extend(type_indices)
    return bytes(result)

def merge_global_sections(parts):
    """Merge multiple global sections. Each has: count (LEB) + global declarations."""
    total_count = 0
    declarations = bytearray()
    for part in parts:
        count, pos = read_leb128(part, 0)
        total_count += count
        declarations.extend(part[pos:])
    result = bytearray()
    result.extend(write_leb128(total_count))
    result.extend(declarations)
    return bytes(result)

def merge_import_sections(parts):
    """Merge multiple import sections. Each has: count (LEB) + imports."""
    total_count = 0
    imports = bytearray()
    for part in parts:
        count, pos = read_leb128(part, 0)
        total_count += count
        # Parse each import to find its size
        for _ in range(count):
            start = pos
            # module name
            mod_len, pos = read_leb128(part, pos)
            pos += mod_len
            # field name
            field_len, pos = read_leb128(part, pos)
            pos += field_len
            # import kind
            kind = part[pos]; pos += 1
            # import type
            if kind == 0:  # function
                pos += 1  # type index (1 byte LEB)
            elif kind == 1:  # table
                pos += 2  # elem type + limits
                # skip limits
                flags = part[pos]; pos += 1
                pos += 1  # initial
                if flags & 0x1:
                    pos += 1  # max
            elif kind == 2:  # memory
                flags = part[pos]; pos += 1
                pos += 1  # initial
                if flags & 0x1:
                    pos += 1  # max
            elif kind == 3:  # global
                pos += 2  # valtype + mut
            imports.extend(part[start:pos])
    result = bytearray()
    result.extend(write_leb128(total_count))
    result.extend(imports)
    return bytes(result)

def merge_code_sections(parts):
    """Merge multiple code sections. Each has: count (LEB) + function bodies."""
    total_count = 0
    bodies = bytearray()
    for part in parts:
        count, pos = read_leb128(part, 0)
        total_count += count
        for _ in range(count):
            body_size, pos = read_leb128(part, pos)
            bodies.extend(write_leb128(body_size))
            bodies.extend(part[pos:pos + body_size])
            pos += body_size
    result = bytearray()
    result.extend(write_leb128(total_count))
    result.extend(bodies)
    return bytes(result)

def merge_data_sections(parts):
    """Merge multiple data sections. Each has: count (LEB) + data segments."""
    total_count = 0
    segments = bytearray()
    for part in parts:
        count, pos = read_leb128(part, 0)
        total_count += count
        for _ in range(count):
            seg_start = pos
            mode = part[pos]; pos += 1
            if mode == 0:
                # active: offset expr + size + data
                while part[pos] != 0x0b:
                    pos += 1
                pos += 1  # 0x0b
                data_size, pos = read_leb128(part, pos)
                pos += data_size
            elif mode == 1:
                # passive: size + data
                data_size, pos = read_leb128(part, pos)
                pos += data_size
            elif mode == 2:
                # active with memory index
                pos += 1  # memory index
                while part[pos] != 0x0b:
                    pos += 1
                pos += 1
                data_size, pos = read_leb128(part, pos)
                pos += data_size
            segments.extend(part[seg_start:pos])
    result = bytearray()
    result.extend(write_leb128(total_count))
    result.extend(segments)
    return bytes(result)

def merge_memory_sections(parts):
    """Merge multiple memory sections. Each has: count (LEB) + memories."""
    total_count = 0
    memories = bytearray()
    for part in parts:
        count, pos = read_leb128(part, 0)
        total_count += count
        memories.extend(part[pos:])
    result = bytearray()
    result.extend(write_leb128(total_count))
    result.extend(memories)
    return bytes(result)

def merge_sections(data):
    """Parse WASM binary, merge duplicate standard sections."""
    out = bytearray(data[0:8])  # magic + version

    # Parse all sections
    sections_by_id = {}
    section_order = []

    pos = 8
    while pos < len(data):
        section_id = data[pos]; pos += 1
        section_len, pos = read_leb128(data, pos)
        section_data = data[pos:pos + section_len]
        pos += section_len

        if section_id not in sections_by_id:
            sections_by_id[section_id] = []
            section_order.append(section_id)
        sections_by_id[section_id].append(section_data)

    MERGERS = {
        2: merge_import_sections,
        3: merge_func_sections,
        5: merge_memory_sections,
        6: merge_global_sections,
        10: merge_code_sections,
        11: merge_data_sections,
    }

    # Write merged sections in order, skip custom and unknown sections
    STANDARD_SECTIONS = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}

    for sid in section_order:
        if sid not in STANDARD_SECTIONS:  # skip custom and unknown sections
            continue

        parts = sections_by_id[sid]

        if len(parts) == 1:
            out.append(sid)
            out.extend(write_leb128(len(parts[0])))
            out.extend(parts[0])
        elif sid in MERGERS:
            merged = MERGERS[sid](parts)
            out.append(sid)
            out.extend(write_leb128(len(merged)))
            out.extend(merged)
        else:
            # For other sections (type, table, export, etc.), use first one
            out.append(sid)
            out.extend(write_leb128(len(parts[0])))
            out.extend(parts[0])

    return bytes(out)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} input.wasm output.wasm")
        sys.exit(1)

    with open(sys.argv[1], 'rb') as f:
        data = f.read()

    merged = merge_sections(data)
    with open(sys.argv[2], 'wb') as f:
        f.write(merged)

    print(f"Merged: {len(data)} -> {len(merged)} bytes")

    # Verify
    pos = 8
    while pos < len(merged):
        section_id = merged[pos]; pos += 1
        section_len, pos = read_leb128(merged, pos)
        section_names = {1:'type', 2:'import', 3:'func', 4:'table', 5:'memory', 6:'global', 7:'export', 8:'start', 9:'element', 10:'code', 11:'data', 12:'data_count'}
        name = section_names.get(section_id, f'unknown({section_id})')
        print(f'  section {name} (id={section_id}), len={section_len}')
        pos += section_len
