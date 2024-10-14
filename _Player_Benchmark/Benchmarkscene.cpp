#include "BenchmarkScene.h"
#include <iostream>

void BenchmarkScene::Start() {
    running = true;
    std::cout << "Now running benchmark scene \"" << GetName() << "\"... Press [SPACE] to skip..." << std::endl;
    Setup();
}

void BenchmarkScene::Stop() {
    running = false;
    std::cout << "Finished running benchmark scene \"" << GetName() << '"' << std::endl;
}

bool BenchmarkScene::GetIsRunning() const {
    return running;
}

