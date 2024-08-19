#include "chip8.h"
#include "sdldisplay.h"
#include <iostream>
#include <chrono>
#include <string>

int main(int argc, char **argv)
{
    // check if arguments are valid. arguments are video scale, cycle delay, and the ROM file
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " [Video Scale] [Cycle Delay] [ROM File]" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // initialize variables based on arguments. video scale, cycle delay, and the ROM file
    int videoScale = std::stoi(argv[1]); // stoi = "string to int"
    int cycleDelay = std::stoi(argv[2]);
    std::string romFile = argv[3];

    // initialize the SDL display
    SDLDisplay display("CHIP-8 Emulator", (videoScale * 64), (videoScale * 32), 64, 32);

    // initialize Chip8, load the ROM file using method in Chip8 class
    Chip8 chip8;
    chip8.LoadRom(romFile);

    // get the video pitch for use in the loop to update the screen.
    // pitch =  # of bytes in a row of pixel data, including padding between lines
    // size of one pixel in bytes * # of pixels in a row
    int videoPitch = sizeof(chip8.video[0]) * 64;

    // initialize a chrono high resolution clock to keep track of time.
    auto lastCycleTime = std::chrono::high_resolution_clock::now();

    // initialize a bool to keep track of whether the emulator is running
    bool quit = false;
    // while loop (while emulator is running)

    std::cout << "before while loop" << std::endl;
    while (!quit)
    {
        quit = display.ProcessInput(chip8.keypad);
        // get the current time
        const auto currTime = std::chrono::high_resolution_clock::now();
        // get the dt (delta time) by subtracting the last time from the current time
        float deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currTime - lastCycleTime).count();
        // if the dt is greater than the cycle delay
        if (deltaTime > cycleDelay)
        {
            // update the last time to the current time
            lastCycleTime = currTime;
            // call the cycle method in the Chip8 class
            chip8.Cycle();
            // update screen
            display.Update(chip8.video, videoPitch);
        }
        // end of loop
    }

    return 0;
}