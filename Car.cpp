
#include <iostream>
#include <cmath>
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

struct Vector2D
{
    float X;
    float Y;

    Vector2D() : X(0.f), Y(0.f) {};
    Vector2D(float x, float y) : X(x), Y(y) {};

    Vector2D operator - (Vector2D& otherVector)
    {
        X -= otherVector.X;
        Y -= otherVector.Y;
    }

    Vector2D operator + (Vector2D& otherVector)
    {
        X += otherVector.X;
        Y += otherVector.Y;
    }

    Vector2D operator * (int& value)
    {
        X *= value;
        Y *= value;
    }

    Vector2D operator / (int& value)
    {
        X /= value;
        Y /= value;
    }

    float DistanceTo(Vector2D& otherVector) const
    {
        return sqrt(pow(X - otherVector.X, 2) + pow(Y - otherVector.Y, 2));
    }
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

class Car
{

public:
    string Name;
    float MaxSpeed;
    unique_ptr<Engine> CarEngine;
    Vector2D Position = Vector2D();

public:

    Car() : Name("Unknown"), MaxSpeed(0.f), CarEngine(nullptr) { cout << "Car created\n"; };
    Car(string name, float speed, unique_ptr<Engine> engine = nullptr) : Name(name), MaxSpeed(speed), CarEngine(move(engine)) { cout << Name << " created\n"; };

    ~Car() { cout << "Car " << Name << " deleted\n"; };

    void ShowCar() noexcept
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

    void Move(Vector2D DeltaMove) noexcept
    {
        Position + DeltaMove;
    }

    void PrintPosition() const
    {
        cout << Name << " is at " << Position.X << ", " << Position.Y << " point\n";
    }
};

class Garage
{

public:
    vector<unique_ptr<Car>> Cars;

    Garage() {};

    void AddCar(unique_ptr<Car> car)
    {
        Cars.emplace_back(move(car));
    }
    void AddCar()
    {
        unique_ptr<Car> car = make_unique<Car>();
        Cars.emplace_back(move(car));
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
        return fastest_car;
    }


    Car* GetClosestCar(Vector2D position) const
    {
        if (Cars.empty())
        {
            return nullptr;
        }

        Car* closest_car = Cars[0].get();
        float nearestDistance = closest_car->Position.DistanceTo(position);
        for (size_t i = 1; i < Cars.size(); i++)
        {
            if (Cars[i]->Position.DistanceTo(position) < nearestDistance)
            {
                nearestDistance = Cars[i]->Position.DistanceTo(position);
            }
        }
        return closest_car;
    }

    Car* GetClosestCar(Vector2D position, float& nearestDistance) const
    {
        if (Cars.empty())
        {
            return nullptr;
        }

        Car* closest_car = Cars[0].get();
        nearestDistance = closest_car->Position.DistanceTo(position);
        for (size_t i = 1; i < Cars.size(); i++)
        {
            if (Cars[i]->Position.DistanceTo(position) < nearestDistance)
            {
                nearestDistance = Cars[i]->Position.DistanceTo(position);
            }
        }
        return closest_car;
    }

};


int main()
{

    Car* MyCar = new Car("Kia Rio", 180);
    cout << MyCar->Name << " has max speed " << MyCar->MaxSpeed << '\n';
    delete MyCar;
    Engine Engine1(220, EngineType::Gasoline);
    unique_ptr<Engine> Engine2 = make_unique<Engine>(330, EngineType::Electricity);
    unique_ptr<Garage> Garage1 = make_unique<Garage>();

    ScoppedTimer* Timer = new ScoppedTimer();
    for (int i = 0; i < 1000; i++)
    {
        Garage1->AddCar();
    }
    delete Timer;
}
