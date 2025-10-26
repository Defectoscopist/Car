
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

enum class EngineType
{
    Gasoline,
    Diesel,
    Electricity
};

constexpr string_view ToString(EngineType type)
{
    switch (type)
    {
    case EngineType::Gasoline: return "Gasoline";
    case EngineType::Diesel: return "Diesel";
    case EngineType::Electricity: return "Electricity";
    default: return "Unknown";
    }
}

class ScoppedTimer
{
    chrono::steady_clock::time_point start;

public:

    ScoppedTimer() { start = chrono::steady_clock::now(); }
    ~ScoppedTimer() {
        auto end = chrono::steady_clock::now();
        cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
    }
};

class Engine
{

public:

    int HorsePower;
    EngineType TypeOfEngine;

public:

    Engine() : HorsePower(100), TypeOfEngine(EngineType::Gasoline) {
        cout << "Created engine:";
        ShowSpecs();
    }

    Engine(int horsePower, EngineType typeOfEngine) : HorsePower(horsePower), TypeOfEngine(typeOfEngine) {
        cout << "Created engine:";
        ShowSpecs();
    }

    ~Engine() {
        cout << "Deleted engine:";
        ShowSpecs();
    }

    void ShowSpecs() const
    {
        cout << "Type - " << ToString(TypeOfEngine)
            << "\t\t Horse Power - " << HorsePower << '\n';
    }

};

struct Car
{
    string Name;
    float MaxSpeed;
    unique_ptr<Engine> CarEngine;

    Car() : Name("Unknown"), MaxSpeed(0.f), CarEngine(nullptr) { cout << "Car created\n"; };
    Car(string name, float speed, unique_ptr<Engine> engine = nullptr) : Name(name), MaxSpeed(speed), CarEngine(move(engine)) { cout << Name << " created\n"; };

    ~Car() { cout << "Car " << Name << " deleted\n"; };

    void ShowCar()
    {
        cout << Name << "\t max speed: " << MaxSpeed << '\t';
        if (CarEngine != nullptr)
        {
            CarEngine->ShowSpecs();
        }
        else
        {
            cout << "no engine\n";
        }
        
    }
};

class Garage
{

public:
    vector<unique_ptr<Car>> Cars;

    void AddCar(unique_ptr<Car> car)
    {
        Cars.emplace_back(car);
    }

    void ShowCars()
    {
        cout << "Cars in Garage:\n";
        for (int i = 0; i < Cars.size(); i++)
        {
            cout << i << '\t' << Cars[i]->Name << "\tspeed: " << Cars[i]->MaxSpeed << '\n';
        }
    }

    Car* GetFastestCar() const
    {
        if (Cars.empty()) { return nullptr; }
        Car* fastest_car = Cars[0].get();
        for (size_t i = 1; i < Cars.size(); i++)
        {
            if (Cars[i]->MaxSpeed > fastest_car->MaxSpeed)
            {
                fastest_car = Cars[i].get();
            }
        }
    }

};


int main()
{
    {
        ScoppedTimer Timer;
        Car* MyCar = new Car("Kia Rio", 180);
        cout << MyCar->Name << " has max speed " << MyCar->MaxSpeed << '\n';
        delete MyCar;

        {
            Engine Engine1(220, EngineType::Gasoline);
            unique_ptr<Engine> Engine2 = make_unique<Engine>(330, EngineType::Electricity);
        }
    }
}
