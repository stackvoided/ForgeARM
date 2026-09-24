#ifndef REGISTER_MAPPER_H
#define REGISTER_MAPPER_H

#include <cstdint>
#include <unordered_map>

enum class RegA64 : uint8_t {
    X0 = 0, X1, X2, X3, X4, X5, X6, X7,
    X8, X9, X10, X11, X12, X13, X14, X15,
    X16, X17, X18, X19, X20, X21, X22, X23,
    X24, X25, X26, X27, X28, X29, X30, SP, INVALID
};

enum class RegA32 : uint8_t {
    R0 = 0, R1, R2, R3, R4, R5, R6, R7,
    R8, R9, R10, R11, R12, SP = 13, LR = 14, PC = 15, INVALID
};

class RegisterMapper {
public:
    RegisterMapper();
    
    RegA32 mapToA32(RegA64 reg);
    RegA64 mapToA64(RegA32 reg);
    
    bool requiresSpill(RegA64 reg) const;
    int getSpillOffset(RegA64 reg) const;

private:
    std::unordered_map<RegA64, RegA32> a64_to_a32;
    std::unordered_map<RegA32, RegA64> a32_to_a64;
};

#endif
