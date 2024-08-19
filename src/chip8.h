#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>
#include <stack>

class Chip8
{
public:
    Chip8();
    void Cycle();
    void LoadRom(std::string filename);
    uint32_t video[64 * 32]{};
    uint8_t keypad[16]{};

private:
    uint8_t registers[16]{};
    uint8_t memory[4096]{};
    uint16_t index{};
    uint16_t pc{};
    std::stack<uint16_t> stack{};
    uint8_t sp{};
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    uint16_t opcode;

    // function pointer tables (NEEDS IMPLEMENTATION)
    void Table0();
    void Table8();
    void TableE();
    void TableF();
    void OP_NULL();

    typedef void (Chip8::*Chip8Func)();
    Chip8Func table[0xF + 1];
    Chip8Func table0[0xE + 1];
    Chip8Func table8[0xE + 1];
    Chip8Func tableE[0xE + 1];
    Chip8Func tableF[0x65 + 1];

    // instruction functions (NEEDS IMPLEMENTATION)

    // table0 opcodes
    void OP_00E0();
    void OP_00EE();

    // table8 opcodes
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();

    // tableE opcodes
    void OP_Ex9E();
    void OP_ExA1();

    // tableF functions
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();

    // unique opcodes
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();
};

#endif