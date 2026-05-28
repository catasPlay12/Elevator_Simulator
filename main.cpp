#include "elevator.h"
#include <iostream>
#include <string>
#include <sstream>

void printHelp() {
    std::cout << "\n  Available Commands:\n";
    std::cout << "  [Enter]       - step up\n";
    std::cout << "  r <etaj(e)>   - add request (ex: r 3  or  r 2 4 5)\n";
    std::cout << "  run <n>       - run n steps automatically (ex: run 5)\n";
    std::cout << "  help          - display help\n";
    std::cout << "  q             - quit\n\n";
}

int main() {
    std::cout << "╔══════════════════════════════════╗\n";
    std::cout << "║   Elevator Simulator — 6 floors  ║\n";
    std::cout << "║   Ground Floor (0) → Floor 5     ║\n";
    std::cout << "╚══════════════════════════════════╝\n";
    printHelp();

    Elevator lift(6, 0);
    lift.printBuilding();

    std::string line;
    int stepCount = 0;

    while (true) {
        std::cout << "\n[pas " << stepCount << "] > ";
        if (!std::getline(std::cin, line)) break;

        // trim leading spaces
        size_t start = line.find_first_not_of(' ');
        if (start != std::string::npos) line = line.substr(start);

        if (line == "q" || line == "quit") {
            std::cout << "La revedere!\n";
            break;

        } else if (line == "help") {
            printHelp();

        } else if (line.empty()) {
            // single step
            lift.step();
            stepCount++;
            lift.printBuilding();

        } else if (line.rfind("run", 0) == 0) {
            int n = 1;
            std::istringstream iss(line.substr(3));
            iss >> n;
            if (n <= 0) n = 1;
            std::cout << "  Running " << n << " steps...\n";
            for (int i = 0; i < n; i++) {
                lift.step();
                stepCount++;
            }
            lift.printBuilding();

        } else if (line[0] == 'r') {
            // "r 3" or "r 2 4 5"
            std::istringstream iss(line.substr(1));
            int floor;
            bool any = false;
            while (iss >> floor) {
                lift.addRequest(floor);
                any = true;
            }
            if (!any) {
                std::cout << "  Sintax: r <floor>  (ex: r 3  or  r 1 3 5)\n";
            }
            lift.printBuilding();

        } else {
            std::cout << "  Unknown command. Type 'help' for help.\n";
        }

        if (lift.isIdle() && !lift.hasRequests()) {
            std::cout << "  ✓ Elevator on hold.\n";
        }
    }

    return 0;
}
