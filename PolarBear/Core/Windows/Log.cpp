#include <Precompile.h>
#include "../Interfaces/ILog.h"

#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

#include <fmt/core.h>
#include <fmt/format.h>

namespace pb {

    bool isSymInitialized = false;
    void printStack(void) {
        void* stack[100];
        unsigned short frames;
        SYMBOL_INFO* symbol;

        auto process = GetCurrentProcess();

        if (!isSymInitialized) {
            isSymInitialized = SymInitialize(process, NULL, TRUE);
        }

        frames = CaptureStackBackTrace(0, 100, stack, NULL);
        symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
        if (!symbol) {
            return;
        }

        symbol->MaxNameLen = 255;
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

        char buffer[256] = {};
        for (unsigned short i = 0; i < frames; i++) {
            IMAGEHLP_LINE64 line = {sizeof(IMAGEHLP_LINE64)};
            DWORD  dwDisplacement = 0;
            SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
            SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &dwDisplacement, &line);
            sprintf_s(buffer, "%s(%d): %s\n", line.FileName, line.LineNumber + dwDisplacement, symbol->Name);
            OutputDebugStringA(buffer);
            sprintf_s(buffer, 256, "%d: %s - 0x%llX\n", frames - i - 1, symbol->Name, symbol->Address);
            //OutputDebugStringA(buffer);
        }

        free(symbol);
    }

    void ILog::Log(LogLevel level, const char* fileName, int lineNumber, const char* message, ...) {
        const size_t BUFF_SIZE = 1024;
        char buffer[BUFF_SIZE] = {};

        va_list args;
        va_start(args, message);

        auto offset = sprintf_s(buffer, "%s(%d): ", fileName, lineNumber);
        vsnprintf_s(buffer + offset, BUFF_SIZE - offset, BUFF_SIZE - offset, message, args);

        OutputDebugStringA(buffer);
    }

}