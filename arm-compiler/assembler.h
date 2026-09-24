#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <vector>
#include <cstdint>
#include "register_mapper.h"

class CodeAssembler {
public:
    CodeAssembler() = default;

    void emit32(uint32_t insn);
    void emitA32Nop();
    void emitA32BxLr();
    void emitA32Push(uint16_t reg_mask);
    void emitA32Pop(uint16_t reg_mask);
    void emitA32Mov(RegA32 dst, RegA32 src);
    void emitA32Add(RegA32 dst, RegA32 src1, RegA32 src2);

    void emitA64Nop();
    void emitA64Ret();
    void emitA64Add(RegA64 dst, RegA64 src1, RegA64 src2);
    void emitA64Mov(RegA64 dst, RegA64 src);

    const std::vector<uint8_t>& getBuffer() const { return buffer; }
    void clear() { buffer.clear(); }
    void reserve(size_t size) { buffer.reserve(size); }

private:
    std::vector<uint8_t> buffer;
};

#endif
