#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <vector>
#include <cstdint>
#include "register_mapper.h"
#include "assembler.h"

enum class TranslationMode {
    ARM64_TO_ARM32,
    ARM32_TO_ARM64
};

class CodeTranslator {
public:
    explicit CodeTranslator(TranslationMode mode);
    ~CodeTranslator() = default;

    std::vector<uint8_t> translateSection(const uint8_t* code, size_t size);

private:
    TranslationMode mode;
    RegisterMapper reg_mapper;
    CodeAssembler assembler;

    void processArm64Instruction(uint32_t insn);
    void processArm32Instruction(uint32_t insn);
};

#endif
