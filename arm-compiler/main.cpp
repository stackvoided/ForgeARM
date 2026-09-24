#include <iostream>
#include <string>
#include "elf_parser.h"
#include "translator.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input_file.so> <output_file.so>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    ElfParser elf;
    if (!elf.parse(input_path)) {
        std::cerr << "Error: Failed to parse ELF file: " << input_path << "\n";
        return 1;
    }

    bool is_src_64 = elf.is64Bit();
    TranslationMode mode = is_src_64 ? TranslationMode::ARM64_TO_ARM32 : TranslationMode::ARM32_TO_ARM64;

    std::cout << "Input Architecture: " << (is_src_64 ? "AArch64 (ARM64)" : "ARM32") << "\n";
    std::cout << "Target Architecture: " << (is_src_64 ? "ARM32 (armeabi-v7a)" : "AArch64 (arm64-v8a)") << "\n";

    CodeTranslator translator(mode);

    for (auto& section : elf.getSections()) {
        if (section.name == ".text" || section.name == ".plt") {
            if (!section.data.empty()) {
                section.data = translator.translateSection(section.data.data(), section.data.size());
            }
        }
    }

    if (!elf.write(output_path, !is_src_64)) {
        std::cerr << "Error: Failed to write output file: " << output_path << "\n";
        return 1;
    }

    std::cout << "Translation complete. Output saved to: " << output_path << "\n";
    return 0;
}
