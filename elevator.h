#pragma once
#include <vector>
#include <queue>
#include <string>

enum class Direction { UP, DOWN, IDLE };
enum class DoorState { OPEN, CLOSED };

struct Request {
    int floor;
    Direction direction; // direction from which the call was made
};

class Elevator {
public:
    // Constructor & Destructor
    Elevator(int totalFloors, int startFloor = 0);
    ~Elevator() = default;

    // Core actions
    void addRequest(int floor);
    void step();            // advance simulation by one time unit
    void openDoors();
    void closeDoors();

    // State queries
    int  getCurrentFloor()  const;
    Direction getDirection() const;
    DoorState getDoorState() const;
    bool isIdle()           const;
    bool hasRequests()      const;

    // Display
    void printStatus()      const;
    void printBuilding()    const;

private:
    // Internal helpers
    void    moveTowardsTarget();
    int     chooseNextTarget() const;
    void    serveCurrentFloor();
    bool    isRequestedFloor(int floor) const;
    std::string directionToString() const;
    std::string doorStateToString() const;

    // State
    int       currentFloor_;
    int       totalFloors_;
    Direction direction_;
    DoorState doorState_;

    std::vector<bool> floorRequests_; // which floors are requested
    int  doorsOpenTimer_;             // how many steps doors stay open
    static const int DOORS_OPEN_DURATION = 2;
};
