#include <iostream>
using namespace std;

// Include our implementation
#include "vehicle.cpp"

int main() {
    // Sample 1
    Vehicle* C = new Car("My car");
    C->drive();  // 输出Vehicle name + " drive on road"

    Vehicle* B = new Boat("My boat");
    B->drive();  // 输出Vehicle name + " drive on river"

    AmphibianCar* A = new AmphibianCar("My amphibian car");
    A->driveAsCar();   // 输出Vehicle name + " drive on road as car"
    A->driveAsBoat();  // 输出Vehicle name + " drive on river as boat"

    // Sample 2
    Car* CarMode   = A;
    Boat* BoatMode = A;

    CarMode->drive();   // 将Car中的drive()方法重写为driveAsCar()
    BoatMode->drive();  // 将Boat中的drive()方法重写为driveAsBoat()

    // Clean up
    delete C;
    delete B;
    delete A;

    return 0;
}
