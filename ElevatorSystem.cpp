#include <bits/stdc++.h>
using namespace std;

/*
Elevator System - Low Level Design Summary

Objective:
Design a basic elevator control system for a building with multiple floors and elevators.
The system should allow requesting elevators from outside (on each floor) and inside the elevator (to go to a floor).
It should simulate elevator movement and print current status.

Core Classes:
1. Direction
   - Enum representing elevator direction: UP, DOWN, IDLE.

2. Elevator
   - Represents an individual elevator.
   - Tracks current floor, direction, and set of destination floors.
   - Can receive floor requests and simulate movement.

3. ElevatorSystem
   - Manages a list of elevators.
   - Handles external and internal requests.
   - Decides which elevator to assign for a new request based on availability and proximity.

Workflow:
- External requests (e.g., from a floor wanting to go UP or DOWN) are handled by the ElevatorSystem.
- Internal requests (e.g., inside the elevator pressing a floor button) go directly to the Elevator.
- Elevators simulate moving one step at a time per "stepSystem()" call, and stop at floors in their destination queue.

Example:
- System is initialized with 2 elevators.
- External requests are made from various floors.
- Internal requests are added after entering an elevator.
- The system is stepped forward in a loop to simulate elevator movement and status.

Extensibility:
- Add support for prioritized requests, overload handling, or door open/close simulation.
- Add a UI layer or REST API for real-world interaction.
- Handle maintenance modes or emergency scenarios.

This design uses object-oriented principles to keep logic modular and testable.
*/

enum Direction { UP, DOWN, IDLE };

class Elevator {
    int id;
    int currentFloor;
    Direction direction;
    set<int> destinations;

public:
    Elevator(int id) : id(id), currentFloor(0), direction(IDLE) {}

    int getId() const {
        return id;
    }

    int getCurrentFloor() const {
        return currentFloor;
    }

    Direction getDirection() const {
        return direction;
    }

    void addDestination(int floor) {
        destinations.insert(floor);
        updateDirection();
    }

    void step() {
        if (destinations.empty()) {
            direction = IDLE;
            return;
        }

        if (direction == UP) {
            currentFloor++;
        } else if (direction == DOWN) {
            currentFloor--;
        }

        // If reached a destination, remove it
        if (destinations.count(currentFloor)) {
            destinations.erase(currentFloor);
        }

        updateDirection();
    }

    void updateDirection() {
        if (destinations.empty()) {
            direction = IDLE;
        } else if (*destinations.begin() > currentFloor) {
            direction = UP;
        } else if (*destinations.rbegin() < currentFloor) {
            direction = DOWN;
        } else if (*destinations.begin() == currentFloor) {
            // Edge case: destination == currentFloor
            destinations.erase(currentFloor);
            updateDirection();
        }
    }

    void status() {
        cout << "Elevator " << id << " at floor " << currentFloor
             << " direction: " << (direction == UP ? "UP" : direction == DOWN ? "DOWN" : "IDLE") << endl;
    }

    bool isIdle() const {
        return direction == IDLE;
    }

    int distanceFrom(int floor) const {
        return abs(currentFloor - floor);
    }
};

class ElevatorSystem {
    vector<Elevator> elevators;

public:
    ElevatorSystem(int numElevators) {
        for (int i = 0; i < numElevators; i++) {
            elevators.emplace_back(i);
        }
    }

    void requestElevator(int floor, Direction dir) {
        Elevator* bestElevator = nullptr;
        int minDistance = INT_MAX;

        for (auto& elevator : elevators) {
            if (elevator.isIdle()) {
                int dist = elevator.distanceFrom(floor);
                if (dist < minDistance) {
                    minDistance = dist;
                    bestElevator = &elevator;
                }
            }
        }

        if (!bestElevator) {
            // No idle elevator, pick the closest one
            for (auto& elevator : elevators) {
                int dist = elevator.distanceFrom(floor);
                if (dist < minDistance) {
                    minDistance = dist;
                    bestElevator = &elevator;
                }
            }
        }

        if (bestElevator) {
            bestElevator->addDestination(floor);
            cout << "Assigned Elevator " << bestElevator->getId() << " to floor " << floor << endl;
        }
    }

    void requestFloor(int elevatorId, int floor) {
        if (elevatorId >= 0 && elevatorId < elevators.size()) {
            elevators[elevatorId].addDestination(floor);
        }
    }

    void stepSystem() {
        for (auto& elevator : elevators) {
            elevator.step();
        }
    }

    void systemStatus() {
        cout << "=== Elevator System Status ===" << endl;
        for (auto& elevator : elevators) {
            elevator.status();
        }
        cout << endl;
    }
};

int main() {
    ElevatorSystem system(2); // 2 elevators

    system.requestElevator(3, UP);  // External request at floor 3
    system.requestElevator(5, DOWN); // External request at floor 5
    system.requestElevator(0, UP); // External request at ground floor

    for (int t = 0; t < 10; t++) {
        cout << "Time step " << t << endl;
        system.stepSystem();
        system.systemStatus();
        if (t == 2) {
            system.requestFloor(0, 7); // Internal request in elevator 0
        }
    }

    return 0;
}
