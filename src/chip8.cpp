#include "chip8.h"
#include <fstream>

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int FONTSET_SIZE = 80;
uint8_t fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
	// function pointer table setup
	// unique opcodes
	table[0x0] = &Chip8::Table0;
	table[0x1] = &Chip8::OP_1nnn;
	table[0x2] = &Chip8::OP_2nnn;
	table[0x3] = &Chip8::OP_3xkk;
	table[0x4] = &Chip8::OP_4xkk;
	table[0x5] = &Chip8::OP_5xy0;
	table[0x6] = &Chip8::OP_6xkk;
	table[0x7] = &Chip8::OP_7xkk;
	table[0x8] = &Chip8::Table8;
	table[0x9] = &Chip8::OP_9xy0;
	table[0xA] = &Chip8::OP_Annn;
	table[0xB] = &Chip8::OP_Bnnn;
	table[0xC] = &Chip8::OP_Cxkk;
	table[0xD] = &Chip8::OP_Dxyn;
	table[0xE] = &Chip8::TableE;
	table[0xF] = &Chip8::TableF;

	// initializing entries up to 0xE
	for (size_t i = 0; i <= 0xE; i++)
	{
		table0[i] = &Chip8::OP_NULL;
		table8[i] = &Chip8::OP_NULL;
		tableE[i] = &Chip8::OP_NULL;
	}

	// table0 opcodes
	table0[0x0] = &Chip8::OP_00E0;
	table0[0xE] = &Chip8::OP_00EE;

	// table8 opcodes
	table8[0x0] = &Chip8::OP_8xy0;
	table8[0x1] = &Chip8::OP_8xy1;
	table8[0x2] = &Chip8::OP_8xy2;
	table8[0x3] = &Chip8::OP_8xy3;
	table8[0x4] = &Chip8::OP_8xy4;
	table8[0x5] = &Chip8::OP_8xy5;
	table8[0x6] = &Chip8::OP_8xy6;
	table8[0x7] = &Chip8::OP_8xy7;
	table8[0xE] = &Chip8::OP_8xyE;

	// tableE opcodes
	tableE[0x1] = &Chip8::OP_ExA1;
	tableE[0xE] = &Chip8::OP_Ex9E;

	// initializing tableF up to 0x65
	for (size_t i = 0; i <= 0x65; i++)
	{
		tableF[i] = &Chip8::OP_NULL;
	}

	// tableF opcodes
	tableF[0x07] = &Chip8::OP_Fx07;
	tableF[0x0A] = &Chip8::OP_Fx0A;
	tableF[0x15] = &Chip8::OP_Fx15;
	tableF[0x18] = &Chip8::OP_Fx18;
	tableF[0x1E] = &Chip8::OP_Fx1E;
	tableF[0x29] = &Chip8::OP_Fx29;
	tableF[0x33] = &Chip8::OP_Fx33;
	tableF[0x55] = &Chip8::OP_Fx55;
	tableF[0x65] = &Chip8::OP_Fx65;

	pc = START_ADDRESS; // start at 0x200, since 0x000 to 0x1FF is reserved
	for (unsigned int i = 0; i < FONTSET_SIZE; i++)
	{ // load fonts into memory
		memory[FONTSET_START_ADDRESS + i] = fontset[i];
	}
}

void Chip8::Table0()
{
	((*this).*(table0[opcode & 0x000Fu]))();
}

void Chip8::Table8()
{
	((*this).*(table8[opcode & 0x000Fu]))();
}

void Chip8::TableE()
{
	((*this).*(tableE[opcode & 0x000Fu]))();
}

void Chip8::TableF()
{
	((*this).*(tableF[opcode & 0x00FFu]))();
}

void Chip8::OP_NULL() {}

void Chip8::LoadRom(char const *filename)
{
	// creates 'file' object
	// two flags mean: open file as stream of binary, and move file pointer to the end
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		// Get size of file and allocate a buffer to hold the contents
		// file.tellg() returns the current position of the file pointer (which is the end, due to std::ios::ate)
		// since the pointer is at the end, it gives the size of the file in bytes
		std::streampos size = file.tellg();
		char *buffer = new char[size];

		// Go back to the beginning of the file and fill the buffer
		file.seekg(0, std::ios::beg); // moves file pointer to offset of 0 from beginning of file
		file.read(buffer, size);
		file.close();

		// Load the ROM contents into the Chip8's memory, starting at 0x200
		for (long i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

		// Free the buffer
		delete[] buffer;
	}
}

// instruction functions
//  Clear The Display
// sets all pixels in video buffer to 0
void Chip8::OP_00E0()
{
	memset(video, 0, sizeof(video));
}

// Jump to Location nnn
// uses & operator to get the last 3 nibbles of the opcode, sets pc to that value
void Chip8::OP_1nnn()
{
	pc = opcode & 0x0FFFu;
}

// Set Register Vx == kk
// isolates 2nd nibble of opcode and shifts it to the right by 8 bits to get register #
// sets register to last 2 nibbles of opcode
void Chip8::OP_6xkk()
{
	uint8_t register_num = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FFu;
	registers[register_num] = value;
}

// Add Vx; Vx = Vx + kk
void Chip8::OP_7xkk()
{
	uint8_t register_num = (opcode & 0x0F00u) >> 8u;
	uint8_t value = opcode & 0x00FFu;
	registers[register_num] += value;
}

// Set Index Register I; Set I = nnn
void Chip8::OP_Annn()
{
	index = opcode & 0x0FFFu;
}

// Display/Draw ; Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
void Chip8::OP_Dxyn()
{
	uint8_t x = registers[(opcode & 0x0F00u >> 8u)];
	uint8_t y = registers[(opcode & 0x00F0u >> 8u)];
}

// Return from a subroutine. Overwrites preemptive pc += 2

void Chip8::OP_00EE()
{
}

// 2nnn - CALL addr; Call subroutine at nnn.

void Chip8::OP_2nnn()
{
}

// 3xkk - SE Vx, byte; Skip next instruction if Vx = kk.

void Chip8::OP_3xkk()
{
}

// 4xkk - SNE Vx, byte Skip next instruction if Vx != kk.

void Chip8::OP_4xkk()
{
}

// 5xy0 - SE Vx, Vy; Skip next instruction if Vx = Vy.
void Chip8::OP_5xy0()
{
}

// 6xkk - LD Vx, byte ; Set Vx = kk.
void Chip8::OP_6xkk()
{
}
// 8xy0 - LD Vx, Vy ; Set Vx = Vy.
void Chip8::OP_8xy0()
{
}

// BITWISE
// 8xy1 - OR Vx, Vy; Set Vx = Vx OR Vy.
void Chip8::OP_8xy1()
{
}

// 8xy2 - AND Vx, Vy Set Vx = Vx AND Vy.
void Chip8::OP_8xy2()
{
}

// 8xy3 - XOR Vx, Vy Set Vx = Vx XOR Vy.
void Chip8::OP_8xy3()
{
}

// 8xy4 - ADD Vx, Vy; Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.

void Chip8::OP_8xy4()
{
}

// 8xy5 - SUB Vx, Vy ; Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.

void Chip8::OP_8xy5()
{
}

// 8xy7 - SUBN Vx, Vy ; Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.

void Chip8::OP_8xy7()
{
}