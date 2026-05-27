#include "elevator.h"
#include <iostream>
#include <algorithm>
#include <climits>

// ─────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────
Elevator::Elevator(int totalFloors, int startFloor)
    : totalFloors_(totalFloors),
      currentFloor_(startFloor),
      direction_(Direction::IDLE),
      doorState_(DoorState::CLOSED),
      floorRequests_(totalFloors, false),
      doorsOpenTimer_(0)
{}

// ─────────────────────────────────────────────
// Public — add a request
// ─────────────────────────────────────────────
void Elevator::addRequest(int floor) {
    if (floor < 0 || floor >= totalFloors_) {
        std::cout << "[WARN] Floor " << floor << " out of range.\n";
        return;
    }
    floorRequests_[floor] = true;
    std::cout << "[REQ ] Floor " << floor << " added.\n";
}

// ─────────────────────────────────────────────
// Public — advance simulation by one step
// ─────────────────────────────────────────────
void Elevator::step() {
    // If doors are open, count down timer
    if (doorState_ == DoorState::OPEN) {
        doorsOpenTimer_--;
        if (doorsOpenTimer_ <= 0) {
            closeDoors();
        }
        return; // don't move while doors are open
    }

    // If this floor is requested, serve it
    if (isRequestedFloor(currentFloor_)) {
        serveCurrentFloor();
        return;
    }

    // Move towards next target
    if (hasRequests()) {
        moveTowardsTarget();
    } else {
        direction_ = Direction::IDLE;
    }
}

// ─────────────────────────────────────────────
// Public — door controls
// ─────────────────────────────────────────────
void Elevator::openDoors() {
    doorState_ = DoorState::OPEN;
    doorsOpenTimer_ = DOORS_OPEN_DURATION;
    std::cout << "[DOOR] Doors OPEN at floor " << currentFloor_ << "\n";
}

void Elevator::closeDoors() {
    doorState_ = DoorState::CLOSED;
    std::cout << "[DOOR] Doors CLOSED at floor " << currentFloor_ << "\n";
}

// ─────────────────────────────────────────────
// Public — state queries
// ─────────────────────────────────────────────
int Elevator::getCurrentFloor() const { return currentFloor_; }
Direction Elevator::getDirection() const { return direction_; }
DoorState Elevator::getDoorState() const { return doorState_; }
bool Elevator::isIdle() const { return direction_ == Direction::IDLE; }

bool Elevator::hasRequests() const {
    for (bool r : floorRequests_) if (r) return true;
    return false;
}

// ─────────────────────────────────────────────
// Public — display
// ─────────────────────────────────────────────
void Elevator::printStatus() const {
    std::cout << "Floor: " << currentFloor_
              << " | Dir: "  << directionToString()
              << " | Door: " << doorStateToString()
              << "\n";
}

void Elevator::printBuilding() const {
    for (int f = totalFloors_ - 1; f >= 0; f--) {
        std::cout << "F" << f << " |";
        if (f == currentFloor_) {
            std::cout << (doorState_ == DoorState::OPEN ? "[=]" : "[|]");
        } else {
            std::cout << "   ";
        }
        std::cout << "| ";
        if (floorRequests_[f]) std::cout << "*";
        std::cout << "\n";
    }
    std::cout << "   +---------+\n";
}

// ─────────────────────────────────────────────
// Private — helpers
// ─────────────────────────────────────────────
void Elevator::serveCurrentFloor() {
    floorRequests_[currentFloor_] = false;
    openDoors();
}

void Elevator::moveTowardsTarget() {
    int target = chooseNextTarget();
    if (target > currentFloor_) {
        direction_ = Direction::UP;
        currentFloor_++;
        std::cout << "[MOVE] Moving UP  → floor " << currentFloor_ << "\n";
    } else if (target < currentFloor_) {
        direction_ = Direction::DOWN;
        currentFloor_--;
        std::cout << "[MOVE] Moving DOWN → floor " << currentFloor_ << "\n";
    }
}

// SCAN algorithm: keep going in current direction; flip when no more requests that way
int Elevator::chooseNextTarget() const {
    // Look for requests in current direction first
    if (direction_ == Direction::UP || direction_ == Direction::IDLE) {
        for (int f = currentFloor_ + 1; f < totalFloors_; f++) {
            if (floorRequests_[f]) return f;
        }
    }
    if (direction_ == Direction::DOWN || direction_ == Direction::IDLE) {
        for (int f = currentFloor_ - 1; f >= 0; f--) {
            if (floorRequests_[f]) return f;
        }
    }
    // Flip direction
    for (int f = currentFloor_ - 1; f >= 0; f--)
        if (floorRequests_[f]) return f;
    for (int f = currentFloor_ + 1; f < totalFloors_; f++)
        if (floorRequests_[f]) return f;

    return currentFloor_; // fallback
}

bool Elevator::isRequestedFloor(int floor) const {
    return floorRequests_[floor];
}

std::string Elevator::directionToString() const {
    switch (direction_) {
        case Direction::UP:   return "UP  ";
        case Direction::DOWN: return "DOWN";
        case Direction::IDLE: return "IDLE";
    }
    return "?";
}

std::string Elevator::doorStateToString() const {
    return doorState_ == DoorState::OPEN ? "OPEN  " : "CLOSED";
}
