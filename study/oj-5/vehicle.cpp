#include <iostream>
#include <string>

using namespace std;

class Vehicle {
   protected:
    string name;
    int iscar;
    int isboat;

   public:
    Vehicle(string name) : name(name) {
        iscar  = 0;
        isboat = 0;
    }
    void drive() {
        if (iscar == 1) {
            cout << name << " drive on road" << endl;
        }
        if (isboat == 1) {
            cout << name << " drive on river" << endl;
        }
    }
};

class Car : virtual public Vehicle {
   public:
    Car(string name) : Vehicle(name) {
        iscar = 1;
    }
    void drive() {
        driveAsCar();
    }
    virtual void driveAsCar() {
        cout << name << " drive on road" << endl;
    }
};

class Boat : virtual public Vehicle {
   public:
    Boat(string name) : Vehicle(name) {
        isboat = 1;
    }
    void drive() {
        driveAsBoat();
    }
    virtual void driveAsBoat() {
        cout << name << " drive on river" << endl;
    }
};

class AmphibianCar : public Car, public Boat {
   public:
    AmphibianCar(string name) : Car(name), Boat(name), Vehicle(name) {}
    void driveAsCar() override {
        cout << name << " drive on road as car" << endl;
    }
    void driveAsBoat() override {
        cout << name << " drive on river as boat" << endl;
    }
};