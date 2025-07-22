#include "Mutex.hpp"
#include "config.hpp"
#include "../shared/Config.hpp"

extern Coordinator gCoordinator;
using namespace std::chrono;

struct Action {
    milliseconds time;
    int action;
    int enemyCode;  // 1 for mosquito, 2 for plane
};

void levelOne(struct GameStruct *gameStruct, Coordinator& gCoordinator) {
    // Define a list of actions with their corresponding time intervals and enemy codes
    std::vector<Action> actions = {
        {milliseconds(1000), 0, 1},
        {milliseconds(1150), 1, 1},
        {milliseconds(1300), 2, 1},
        {milliseconds(1450), 3, 1},
        {milliseconds(2000), 4, 2},
        {milliseconds(2200), 5, 1},
        {milliseconds(2400), 6, 1},
        {milliseconds(2500), 7, 2},
        {milliseconds(3000), 8, 1},
        {milliseconds(3300), 9, 1},
        {milliseconds(3600), 10, 1},
        {milliseconds(3500), 11, 2},
        {milliseconds(3900), 12, 1},
        {milliseconds(4000), 13, 2},
        {milliseconds(4100), 14, 1},
    };

    auto startTime = high_resolution_clock::now(); // Get the start time

    auto initialStartTime = high_resolution_clock::now(); // Get the inistart time
    int currentIndex = 0;

    while (true) {
        auto currentTime = high_resolution_clock::now();
        auto elapsedTime = duration_cast<milliseconds>(currentTime - startTime);

        auto elapsedTotalTime = duration_cast<milliseconds>(currentTime - initialStartTime);

        // Check if it's time to perform the next action
        if (currentIndex < actions.size() && elapsedTime >= actions[currentIndex].time) {
            int action = actions[currentIndex].action;
            int enemyCode = actions[currentIndex].enemyCode;
            
            if (enemyCode == 1) {
                createMosquitoSafely(gameStruct);
            } else if (enemyCode == 2) {
                createPlaneSafely(gameStruct);
            }
            
            currentIndex++;
        }

        if (currentIndex == 14) {
            startTime = currentTime;
            currentIndex = 0;
        }

        {
            std::lock_guard<std::mutex> lock(levelOneMutex);
            if (!gameStruct->startLevelOne) {
                break; // Exit the loop
            }
        }

        if (elapsedTotalTime >= seconds(90)) {
            // Action to be performed at 1m 30
            std::lock_guard<std::mutex> lock(levelOneMutex);
            gameStruct->startLevelOne = false;
            std::cout << "Level One ends - Win!" << std::endl;
            break; // Exit the loop
        }
    }
}

void levelTwo(struct GameStruct *gameStruct, Coordinator& gCoordinator) {

    auto startTime = high_resolution_clock::now(); // Get the start time
    int actionpassed = 0;

    while (true) {
        auto currentTime = high_resolution_clock::now();
        auto elapsedTime = duration_cast<seconds>(currentTime - startTime);

        if (elapsedTime >= seconds(1) && actionpassed == 0) {
            actionpassed++;
            createAlienBossSafely(gameStruct);
        }

        if (gameStruct->enemiesAlienBossVector.empty() && elapsedTime >= seconds(5)) {
            // When boss dies
            std::lock_guard<std::mutex> lock(levelTwoMutex);
            gameStruct->startLevelTwo = false;
            std::cout << "Level Two ends - Win!" << std::endl;
            break;
        }
    }
}