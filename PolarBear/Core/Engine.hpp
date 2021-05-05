#pragma once

namespace pb {

    class Engine {
    public:
        Engine() {}
        ~Engine() {}

        void Init();
        void Run();
        void Term();
    };

}