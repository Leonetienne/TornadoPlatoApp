#include "BenchmarkScene.h"
#include <iostream>

BenchmarkScene::BenchmarkScene(const std::string& sceneName):
    Scene(sceneName) {};

void BenchmarkScene::Start() {
    running = true;
    std::cout << "Now running benchmark scene \"" << GetSceneName() << "\"... Press [SPACE] to skip..." << std::endl;
    Setup();
}

void BenchmarkScene::Stop() {
    running = false;
    std::cout << "Finished running benchmark scene \"" << GetSceneName() << '"' << std::endl;
}

bool BenchmarkScene::GetIsRunning() const {
    return running;
}

