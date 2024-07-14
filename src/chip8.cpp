#include <cstdint>

class Chip8 {
    public: //'{}' after each variable means initialized to 0
        uint8_t     registers[16]{};
        uint8_t     memory[4096]{}; 
        uint16_t    index;
        uint16_t    pc;
        uint16_t    stack[16]{};
        uint8_t     sp;
        uint8_t     delayTimer;
        uint8_t     soundTimer;
        uint8_t     keypad[16]{};
        uint32_t    display[64 * 32]{};
};