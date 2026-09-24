#include "translator.h"

CodeTranslator::CodeTranslator(TranslationMode mode) : mode(mode) {}

std::vector<uint8_t> CodeTranslator::translateSection(const uint8_t* code, size_t size) {
    assembler.clear();
    assembler.reserve(size);

    for (size_t offset = 0; offset + 4 <= size; offset += 4) {
        uint32_t insn = *reinterpret_cast<const uint32_t*>(code + offset);

        if (mode == TranslationMode::ARM64_TO_ARM32) {
            processArm64Instruction(insn);
        } else {
            processArm32Instruction(insn);
        }
    }

    return assembler.getBuffer();
}

void CodeTranslator::processArm64Instruction(uint32_t insn) {
    if (insn == 0xD503201F) {
        assembler.emitA32Nop();
        return;
    }

    if (insn == 0xD65F03C0) {
        assembler.emitA32BxLr();
        return;
    }

    if ((insn & 0xFFE00000) == 0x8B000000) {
        uint8_t rm = (insn >> 16) & 0x1F;
        uint8_t rn = (insn >> 5) & 0x1F;
        uint8_t rd = insn & 0x1F;

        RegA32 r32_d = reg_mapper.mapToA32(static_cast<RegA64>(rd));
        RegA32 r32_n = reg_mapper.mapToA32(static_cast<RegA64>(rn));
        RegA32 r32_m = reg_mapper.mapToA32(static_cast<RegA64>(rm));

        if (r32_d != RegA32::INVALID && r32_n != RegA32::INVALID && r32_m != RegA32::INVALID) {
            assembler.emitA32Add(r32_d, r32_n, r32_m);
            return;
        }
    }

    assembler.emitA32Nop();
}

void CodeTranslator::processArm32Instruction(uint32_t insn) {
    if (insn == 0xE1A00000) {
        assembler.emitA64Nop();
        return;
    }

    if (insn == 0xE12FFF1E) {
        assembler.emitA64Ret();
        return;
    }

    if ((insn & 0x0FE00000) == 0x00800000) {
        uint8_t rn = (insn >> 16) & 0x0F;
        uint8_t rd = (insn >> 12) & 0x0F;
        uint8_t rm = insn & 0x0F;

        RegA64 r64_d = reg_mapper.mapToA64(static_cast<RegA32>(rd));
        RegA64 r64_n = reg_mapper.mapToA64(static_cast<RegA32>(rn));
        RegA64 r64_m = reg_mapper.mapToA64(static_cast<RegA32>(rm));

        if (r64_d != RegA64::INVALID && r64_n != RegA64::INVALID && r64_m != RegA64::INVALID) {
            assembler.emitA64Add(r64_d, r64_n, r64_m);
            return;
        }
    }

    assembler.emitA64Nop();
}
