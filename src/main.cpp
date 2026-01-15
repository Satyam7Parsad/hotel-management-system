#include "core/Application.hpp"
#include "utils/Logger.hpp"
#include <iostream>
#include <exception>

using namespace HotelManagement;

int main(int argc, char** argv) {
    try {
        // Initialize logger
        Logger::init("hotel_system.log");
        Logger::setLevel(LogLevel::INFO);

        Logger::info("========================================");
        Logger::info("Hotel Management System Starting...");
        Logger::info("========================================");

        // Create and run application
        Application app;

        if (!app.initialize()) {
            Logger::error("Failed to initialize application");
            std::cerr << "ERROR: Failed to initialize application. Check hotel_system.log for details." << std::endl;
            return 1;
        }

        app.run();
        app.shutdown();

        Logger::info("========================================");
        Logger::info("Hotel Management System Exited Successfully");
        Logger::info("========================================");

        Logger::shutdown();

        return 0;

    } catch (const std::exception& e) {
        Logger::error("Fatal error: ", e.what());
        std::cerr << "FATAL ERROR: " << e.what() << std::endl;
        Logger::shutdown();
        return 1;
    } catch (...) {
        Logger::error("Unknown fatal error occurred");
        std::cerr << "FATAL ERROR: Unknown exception" << std::endl;
        Logger::shutdown();
        return 1;
    }
}
