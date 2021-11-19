#include <iostream>
using namespace std;

class ICar
{
public:

    virtual void EngineStart() = 0;

    virtual void EngineStop() = 0;

    virtual void Refuel(double liters) = 0;

    virtual void RunningIdle() = 0;

protected:

    bool engineIsRunning = false;
};

class IEngine
{
public:

    virtual void Consume(double liters) = 0;

    virtual void Start() = 0;

    virtual void Stop() = 0;

protected:

    bool isRunning = false;
};

class IFuelTank
{
public:

    virtual void Consume(double liters) = 0;

    virtual void Refuel(double liters) = 0;

protected:
    
    double fillLevel = 0.0; 

    bool isOnReserve = false;

    bool isComplete = false;
};

class IFuelTankDisplay
{
protected:

    double fillLevel = 0.0;

    bool isOnReserve = false;

    bool isComplete = false;
};
class Engine : public IEngine
{
public:
    Engine() {}
    ~Engine() {}
    void Consume(double liters){}

    void Start()
    {
        isRunning = true;
    }

    void Stop()
    {
        isRunning = false;
    }
    

};

class FuelTank : public IFuelTank
{
protected:
    const double MAXIMUM_FILL_LEVEL = 60.0;
    const double RESERVE_FILL_LEVEL = 5.0;
public:
    FuelTank(double liters)
    {
        setFillLevel(liters);
    }
    ~FuelTank(){}
    void Consume(double liters) {
        if (liters > 0) {
            fillLevel -= liters;
        }
        if (fillLevel < RESERVE_FILL_LEVEL) {
            isOnReserve = true;
        }
        if (fillLevel < MAXIMUM_FILL_LEVEL) {
            isComplete = false;
        }
        if (fillLevel < 0.0) {
            fillLevel = 0.0;
        }
    }
    void Refuel(double liters) {
        if (liters > 0) {
            fillLevel += liters;
        }
        if (fillLevel >= MAXIMUM_FILL_LEVEL) {
            fillLevel = MAXIMUM_FILL_LEVEL;
            isComplete = true;
        }
        if (fillLevel > RESERVE_FILL_LEVEL) {
            isOnReserve = false;
        }
        if (fillLevel < RESERVE_FILL_LEVEL) {
            isOnReserve = true;
        }
    }
    void setFillLevel(double liters) {
        if (liters >= 60) {
            fillLevel = 60.0;
            isComplete = true;
        }
        else {
            if (liters < 0) fillLevel = 0;
            else fillLevel = liters;
        }
        if (liters < 5) isOnReserve = true;
    }
    double getFilllevel()
    {
        return fillLevel;
    }
    bool getIsOnReserve() 
    { 
        return isOnReserve; 
    }
    bool getIsComplete() 
    { 
        return isComplete; 
    }
};

class FuelTankDisplay : public IFuelTankDisplay
{

public:
    FuelTank* Tdisplay;
    FuelTankDisplay(FuelTank* Tdisplay) 
    {
        this->Tdisplay = Tdisplay;
     
    }
    ~FuelTankDisplay() {}
    double getFillLevel() 
    { 
        return Tdisplay->getFilllevel();
    }
    bool getIsOnReserve()
    {
        return Tdisplay->getIsOnReserve();
    }
    bool getIsComplete() 
    { 
        return Tdisplay->getIsComplete();
    }
};

class Car : public ICar
{
protected:
    const double FUEL_CONSUMPTION = 0.0003;
public:

    Engine* engine;
    FuelTank* fuelTank;
    FuelTankDisplay* fuelTankDisplay;

    Car() {
        engine = new Engine;
        fuelTank = new FuelTank(20);
        fuelTankDisplay = new FuelTankDisplay(fuelTank); 
    }

    Car(double liters) {
        engine = new Engine;
        fuelTank = new FuelTank(liters);
        fuelTankDisplay = new FuelTankDisplay(fuelTank);
    }

    ~Car()
    {
        delete engine;
        delete fuelTank;
        delete fuelTankDisplay;
    }

    void EngineStart() {
        if (fuelTank->getFilllevel() > 0) {
            engine->Start();
            engineIsRunning = true;
        }
    }

    void EngineStop() {
        engine->Stop();
        engineIsRunning = false;
    }

    void Refuel(double liters) { fuelTank->Refuel(liters); }

    void RunningIdle() {
        if (getEngineIsRunning())
            fuelTank->Consume(FUEL_CONSUMPTION);
        if (fuelTank->getFilllevel() == 0)
            EngineStop();
    }

    bool getEngineIsRunning() 
    { 
        return engineIsRunning; 
    }

};





void main()
{
    setlocale(LC_ALL, " ");

    Car car;
    if (!car.getEngineIsRunning()) {
        car.EngineStart();
        cout << "Engine is running..." << endl;
    }
    if (car.getEngineIsRunning()) {
        car.EngineStop();
        cout << "Engine Stop." << endl;
    }

    car.fuelTank->setFillLevel(1);
    car.EngineStart();
    for (int i = 0; i < 3000; i++)
        car.RunningIdle();

    cout << "Fuel tank = " << car.fuelTankDisplay->getFillLevel()
        << " liters." << endl;

    car.fuelTank->setFillLevel(60);
    if (car.fuelTankDisplay->getIsComplete())
        cout << "Fuel tank is complete." << endl;

    car.fuelTank->setFillLevel(4);
    if (car.fuelTankDisplay->getIsOnReserve())
        cout << "Fuel tank is on reserve." << endl;

    car.fuelTank->setFillLevel(10);
    car.Refuel(40);
    if (car.fuelTankDisplay->getFillLevel() == 45)
    cout << "Fuel tank = 45 liters." << endl;
    
}