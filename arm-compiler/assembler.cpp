#include "assembler.h"

void CodeAssembler::emit32(uint32_t insn) {
    const auto* p = reinterpret_cast<const uint8_t*>(&insn);
    buffer.insert(buffer.end(), p, p + 4);
}

void CodeAssembler::emitA32Nop() {
    emit32(0xE1A00000);
}

void CodeAssembler::emitA32BxLr() {
    emit32(0xE12FFF1E);
}

void CodeAssembler::emitA32Push(uint16_t reg_mask) {
    uint32_t insn = 0xE92D0000 | reg_mask;
    emit32(insn);
}

void CodeAssembler::emitA32Pop(uint16_t reg_mask) {
    uint32_t insn = 0xE8BD0000 | reg_mask;
    emit32(insn);
}

void CodeAssembler::emitA32Mov(RegA32 dst, RegA32 src) {
    uint32_t d = static_cast<uint32_t>(dst);
    uint32_t s = static_cast<uint32_t>(src);
    uint32_t insn = 0xE1A00000 | (d << 12) | s;
    emit32(insn);
}

void CodeAssembler::emitA32Add(RegA32 dst, RegA32 src1, RegA32 src2) {
    uint32_t d = static_cast<uint32_t>(dst);
    uint32_t s1 = static_cast<uint32_t>(src1);
    uint32_t s2 = static_cast<uint32_t>(src2);
    uint32_t insn = 0xE0800000 | (s1 << 16) | (d << 12) | s2;
    emit32(insn);
}

void CodeAssembler::emitA64Nop() {
    emit32(0xD503201F);
}

void CodeAssembler::emitA64Ret() {
    emit32(0xD65F03C0);
}

void CodeAssembler::emitA64Add(RegA64 dst, RegA64 src1, RegA64 src2) {
    uint32_t d = static_cast<uint32_t>(dst);
    uint32_t s1 = static_cast<uint32_t>(src1);
    uint32_t s2 = static_cast<uint32_t>(src2);
    uint32_t insn = 0x8B000000 | (s2 << 16) | (s1 << 5) | d;
    emit32(insn);
}

void CodeAssembler::emitA64Mov(RegA64 dst, RegA64 src) {
    emitA64Add(dst, src, RegA64::X0);
}
