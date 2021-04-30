#include "Precompile.h"
#include "Engine.hpp"
#include "os.h"

using namespace pb;

void pb::Engine::Init() {
    InitWindow(InitWindowDesc{});
}

void pb::Engine::Term() {
}
