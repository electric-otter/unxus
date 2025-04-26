#ifndef UNXUSD_H
#define UNXUSD_H

#include <iostream>
#include <vector>
#include <string>

// Abstract base class for all drivers
class Driver {
public:
    virtual void initialize() = 0;      // Initialize the driver
    virtual void shutdown() = 0;        // Shut down the driver
    virtual void operate() = 0;         // Operate the device
    virtual ~Driver() {}                 // Virtual destructor
};

// File driver class
class FileDriver : public Driver {
public:
    void initialize() override;
    void shutdown() override;
    void operate() override;
};

// Network driver class
class NetworkDriver : public Driver {
public:
    void initialize() override;
    void shutdown() override;
    void operate() override;
};

// Graphics driver class
class GraphicsDriver : public Driver {
public:
    void initialize() override;
    void shutdown() override;
    void operate() override;
};

// DriverManager class to manage drivers
class DriverManager {
public:
    void loadDriver(Driver* driver);
    void operateAll();
    void shutdownAll();
    ~DriverManager();

private:
    std::vector<Driver*> drivers;  // A list of loaded drivers
};

#endif // UNXUSDRIVER_H
