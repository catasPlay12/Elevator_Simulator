#include "elevator.h"
#include <iostream>
#include <string>
#include <sstream>

void printHelp() {
    std::cout << "\n  Comenzi disponibile:\n";
    std::cout << "  [Enter]       - avansează un pas\n";
    std::cout << "  r <etaj(e)>   - adaugă cerere (ex: r 3  sau  r 2 4 5)\n";
    std::cout << "  run <n>       - rulează n pași automat (ex: run 5)\n";
    std::cout << "  help          - afișează ajutor\n";
    std::cout << "  q             - ieși\n\n";
}

int main() {
    std::cout << "╔══════════════════════════════╗\n";
    std::cout << "║   Simulator Lift — 6 etaje   ║\n";
    std::cout << "║   Parter (0) → Etaj 5        ║\n";
    std::cout << "╚══════════════════════════════╝\n";
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
            // un singur pas
            lift.step();
            stepCount++;
            lift.printBuilding();

        } else if (line.rfind("run", 0) == 0) {
            int n = 1;
            std::istringstream iss(line.substr(3));
            iss >> n;
            if (n <= 0) n = 1;
            std::cout << "  Rulez " << n << " pași...\n";
            for (int i = 0; i < n; i++) {
                lift.step();
                stepCount++;
            }
            lift.printBuilding();

        } else if (line[0] == 'r') {
            // "r 3" sau "r 2 4 5"
            std::istringstream iss(line.substr(1));
            int floor;
            bool any = false;
            while (iss >> floor) {
                lift.addRequest(floor);
                any = true;
            }
            if (!any) {
                std::cout << "  Sintaxă: r <etaj>  (ex: r 3  sau  r 1 3 5)\n";
            }
            lift.printBuilding();

        } else {
            std::cout << "  Comandă necunoscută. Scrie 'help' pentru ajutor.\n";
        }

        if (lift.isIdle() && !lift.hasRequests()) {
            std::cout << "  ✓ Lift în așteptare.\n";
        }
    }

    return 0;
}
