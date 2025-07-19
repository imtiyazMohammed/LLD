/*
Parking Lot System - Low Level Design Summary

Objective:
Design a parking lot system that supports multiple floors and vehicle types (BIKE, CAR, TRUCK).
The system should allow parking, unparking, and viewing current status.

Core Classes:
1. Vehicle
   - Represents a vehicle with registration number and type.
2. ParkingSpot
   - Represents an individual parking spot with a specific type (BIKE_SPOT, CAR_SPOT, TRUCK_SPOT).
   - Handles parking and unparking logic.
3. ParkingFloor
   - Contains a collection of ParkingSpot objects.
   - Finds available spots and handles vehicle removal from spots.
4. ParkingLot
   - Manages multiple ParkingFloor instances.
   - Handles high-level operations: park, unpark, getStatus.

Workflow:
- When a vehicle is to be parked, the system searches each floor for the first available and compatible spot.
- If a matching spot is found, the vehicle is assigned to it.
- Unparking is done by searching for the vehicle registration number and freeing the corresponding spot.
- The current status shows all floors and the status of each spot (occupied or free).

Example:
- The main function creates a parking lot with 2 floors.
- Each floor contains 2 bike spots, 2 car spots, and 1 truck spot.
- Four vehicles are parked, status is printed, one vehicle is unparked, and status is printed again.

Extensibility:
- Add ticketing and billing systems.
- Include entry/exit timestamps and price calculations.
- Support additional vehicle and spot types.
- Implement REST API or multithreaded access if needed.

This design uses composition and enums to maintain clarity and modularity in an object-oriented manner.
*/

#include <bits/stdc++.h>
using namespace std;

// Vehicle and Spot types
enum VehicleType { BIKE, CAR, TRUCK };
enum SpotType { BIKE_SPOT, CAR_SPOT, TRUCK_SPOT };

// Base Vehicle class
class Vehicle {
public:
    string regNumber;
    VehicleType type;
    
    Vehicle(string regNumber, VehicleType type) : regNumber(regNumber), type(type) {}
};

// Spot class
class ParkingSpot {
    int spotId;
    SpotType type;
    bool isOccupied;
    Vehicle* parkedVehicle;

public:
    ParkingSpot(int id, SpotType type) : spotId(id), type(type), isOccupied(false), parkedVehicle(nullptr) {}

    bool canFitVehicle(VehicleType vehicleType) {
        return !isOccupied &&
               ((vehicleType == BIKE && type == BIKE_SPOT) ||
                (vehicleType == CAR && type == CAR_SPOT) ||
                (vehicleType == TRUCK && type == TRUCK_SPOT));
    }

    bool parkVehicle(Vehicle* vehicle) {
        if (canFitVehicle(vehicle->type)) {
            isOccupied = true;
            parkedVehicle = vehicle;
            return true;
        }
        return false;
    }

    void unparkVehicle() {
        isOccupied = false;
        parkedVehicle = nullptr;
    }

    bool occupied() {
        return isOccupied;
    }

    int getId() {
        return spotId;
    }

    SpotType getType() {
        return type;
    }

    Vehicle* getVehicle() {
        return parkedVehicle;
    }
};

// Floor class
class ParkingFloor {
    int floorId;
    vector<ParkingSpot> spots;

public:
    ParkingFloor(int id, vector<pair<int, SpotType>> spotInfo) : floorId(id) {
        for (auto& info : spotInfo) {
            spots.emplace_back(info.first, info.second);
        }
    }

    ParkingSpot* findAvailableSpot(VehicleType type) {
        for (auto& spot : spots) {
            if (spot.canFitVehicle(type)) {
                return &spot;
            }
        }
        return nullptr;
    }

    bool unparkVehicle(string regNumber) {
        for (auto& spot : spots) {
            if (spot.occupied() && spot.getVehicle()->regNumber == regNumber) {
                spot.unparkVehicle();
                return true;
            }
        }
        return false;
    }

    void printStatus() {
        cout << "  Floor " << floorId << " status:\n";
        for (auto& spot : spots) {
            cout << "    Spot " << spot.getId() << " (" 
                 << (spot.getType() == BIKE_SPOT ? "Bike" : spot.getType() == CAR_SPOT ? "Car" : "Truck") 
                 << "): ";
            if (spot.occupied())
                cout << "Occupied by " << spot.getVehicle()->regNumber << endl;
            else
                cout << "Free" << endl;
        }
    }
};

// Parking Lot class
class ParkingLot {
    vector<ParkingFloor> floors;

public:
    ParkingLot(int numFloors, vector<pair<int, SpotType>> spotConfigPerFloor) {
        for (int i = 0; i < numFloors; i++) {
            floors.emplace_back(i, spotConfigPerFloor);
        }
    }

    bool park(Vehicle* vehicle) {
        for (auto& floor : floors) {
            ParkingSpot* spot = floor.findAvailableSpot(vehicle->type);
            if (spot && spot->parkVehicle(vehicle)) {
                cout << "Vehicle " << vehicle->regNumber << " parked at spot " << spot->getId() << " on floor " << &floor - &floors[0] << endl;
                return true;
            }
        }
        cout << "No available spot for " << vehicle->regNumber << endl;
        return false;
    }

    void unpark(string regNumber) {
        for (auto& floor : floors) {
            if (floor.unparkVehicle(regNumber)) {
                cout << "Vehicle " << regNumber << " unparked.\n";
                return;
            }
        }
        cout << "Vehicle " << regNumber << " not found.\n";
    }

    void getStatus() {
        cout << "==== Parking Lot Status ====" << endl;
        for (auto& floor : floors) {
            floor.printStatus();
        }
    }
};

int main() {
    // Each floor has: 2 bike spots, 2 car spots, 1 truck spot
    vector<pair<int, SpotType>> config = {
        {1, BIKE_SPOT}, {2, BIKE_SPOT},
        {3, CAR_SPOT}, {4, CAR_SPOT},
        {5, TRUCK_SPOT}
    };

    ParkingLot lot(2, config);

    Vehicle* v1 = new Vehicle("KA01AB1234", BIKE);
    Vehicle* v2 = new Vehicle("KA02CD5678", CAR);
    Vehicle* v3 = new Vehicle("KA03EF9876", TRUCK);
    Vehicle* v4 = new Vehicle("KA04GH1122", CAR);

    lot.park(v1);
    lot.park(v2);
    lot.park(v3);
    lot.park(v4);  // Should go to next available floor

    lot.getStatus();

    lot.unpark("KA02CD5678");
    lot.getStatus();

    delete v1;
    delete v2;
    delete v3;
    delete v4;

    return 0;
}
