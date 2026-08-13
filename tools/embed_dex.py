#!/usr/bin/env python3
import sys
from pathlib import Path

if len(sys.argv) != 3:
    raise SystemExit("usage: embed_dex.py <classes.dex> <output.h>")

src = Path(sys.argv[1])
dst = Path(sys.argv[2])

data = src.read_bytes()

lines = []
for i in range(0, len(data), 12):
    chunk = data[i:i + 12]
    lines.append("    " + ", ".join(f"0x{b:02x}" for b in chunk) + ",")

header = """#pragma once
#include <stddef.h>
#include <stdint.h>

static const uint8_t kKiraMenuDex[] = {
%s
};

static const size_t kKiraMenuDexLen = sizeof(kKiraMenuDex);
""" % "\n".join(lines)

dst.write_text(header, encoding="utf-8")
print(f"Embedded {len(data)} DEX bytes into {dst}")
