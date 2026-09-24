<div align="center">

# ⚡ ARM Binary Translator

**A lightweight, zero-dependency C++17 Proof-of-Concept static binary translator and AOT re-assembler for Android Shared Libraries (`.so`).**

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=cplusplus)](https://en.cppreference.com/w/cpp/17)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Android-green.svg?style=for-the-badge&logo=linux)](https://www.android.com/)
[![License](https://img.shields.io/badge/License-MIT-orange.svg?style=for-the-badge)](LICENSE)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg?style=for-the-badge)](Makefile)
</div>

---

## 📌 Overview

**ARM Binary Translator** is a C++17 tool designed to demonstrate static binary transpilation between **AArch64 (`arm64-v8a`)** and **ARMv7 (`armeabi-v7a`)** architectures for Android `.so` shared objects. 

The project performs low-level ELF format parsing, instruction stream decoding, register reallocation (spill handling), and ISA re-encoding without relying on heavy external dependencies.

---

## ✨ Key Features

- 🛠 **Custom ELF32/ELF64 Engine**: Parses, reconstructs, and writes valid ELF structures (`Shdr`, `Ehdr`, `.text`, `.plt`) across 32-bit and 64-bit binaries.
- 🔄 **Bidirectional Translation**: Supports both `AArch64 -> ARMv7` downscaling and `ARMv7 -> AArch64` upscaling.
- 🗃 **Register Mapping & Allocation Logic**: Emulates AArch64's 31 64-bit register file ($X0\text{--}X30$) over ARMv7's 16 32-bit registers ($R0\text{--}R15$).
- 🚀 **High Performance & Zero Dependencies**: Built strictly using modern C++17 standard libraries for maximum compilation speed and portability.
- 📦 **Clean Modular Design**: Single-responsibility architectural design with separated parsing, assembly, translation, and mapping modules.

---

## 🏗 Architecture & Project Structure

```text
arm-compiler/
├── 📄 elf_header.h          # Low-level C-struct definitions for ELF32/ELF64 headers
├── 📄 elf_parser.h          # Interface for ELF binary parsing & re-writing
├── 📄 elf_parser.cpp        # Custom ELF file parser implementation
├── 📄 register_mapper.h    # Register mapping interface (AArch64 <-> ARM32)
├── 📄 register_mapper.cpp  # State management and spill-offset calculations
├── 📄 assembler.h          # Low-level A32 and A64 instruction encoder
├── 📄 assembler.cpp        # Byte-buffer code emitter and opcode construction
├── 📄 translator.h         # Core ISA translation context and interface
├── 📄 translator.cpp       # Pattern-matching translation loops
├── 📄 main.cpp             # CLI Entry point & pipeline controller
└── 🛠 Makefile             # Optimized build configuration script
