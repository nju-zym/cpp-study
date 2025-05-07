#include <iostream>
using namespace std;

// Include our implementation
#include "vehicle.cpp"

int main() {
    // Test case 1 - From the problem statement
    Vehicle* C = new Car("My car");
    C->drive();  // Should output: My car drive on road

    Vehicle* B = new Boat("My boat");
    B->drive();  // Should output: My boat drive on river

    AmphibianCar* A = new AmphibianCar("My amphibian car");
    A->driveAsCar();   // Should output: My amphibian car drive on road as car
    A->driveAsBoat();  // Should output: My amphibian car drive on river as boat

    // Test case 2 - From the problem statement
    // Need to use the conversion operators explicitly
    Car* CarMode   = (Car*)*A;
    Boat* BoatMode = (Boat*)*A;

    CarMode->drive();   // Should output: My amphibian car drive on road as car
    BoatMode->drive();  // Should output: My amphibian car drive on river as boat

    // Clean up
    delete C;
    delete B;
    delete A;

    return 0;
}
