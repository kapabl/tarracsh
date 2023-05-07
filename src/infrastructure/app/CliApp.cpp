#include "CliApp.h"




#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
#include <windows.h>
// #include <processenv.h>
// #include <consoleapi.h>
#endif



using namespace kapa::infrastructure::app::cli;

#ifdef _WIN32
void CliApp::prepareConsoleForUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
}

void CliApp::prepareConsoleForVT100() {
    const auto stdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    unsigned long ulMode;
    GetConsoleMode(stdOutHandle, &ulMode);
    SetConsoleMode(stdOutHandle, ulMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    // SetConsoleMode(stdOutHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
}


#endif