#pragma once

#include "core/Config.hpp"
#include "database/DatabaseManager.hpp"
#include "database/repositories/RoomRepository.hpp"
#include "database/repositories/GuestRepository.hpp"
#include "database/repositories/BookingRepository.hpp"
#include <memory>
#include <string>

// Forward declarations
struct GLFWwindow;

namespace HotelManagement {

class Application {
public:
    Application();
    ~Application();

    bool initialize();
    void run();
    void shutdown();

private:
    // Window management
    GLFWwindow* window = nullptr;
    int windowWidth = 1920;
    int windowHeight = 1080;
    std::string windowTitle = "Hotel Management System";

    // Core systems
    Config config;
    std::unique_ptr<DatabaseManager> dbManager;

    // Repositories
    std::unique_ptr<RoomRepository> roomRepo;
    std::unique_ptr<GuestRepository> guestRepo;
    std::unique_ptr<BookingRepository> bookingRepo;

    // Application state
    bool running = false;
    int currentView = 0; // 0=Dashboard, 1=Rooms, 2=Guests, 3=Bookings

    // Initialization helpers
    bool initWindow();
    bool initImGui();
    bool initDatabase();
    bool initRepositories();

    // Main loop
    void processEvents();
    void update();
    void render();
    void renderUI();

    // UI rendering
    void renderMenuBar();
    void renderSidebar();
    void renderMainContent();

    // View renderers
    void renderDashboard();
    void renderRoomsView();
    void renderGuestsView();
    void renderBookingsView();
};

} // namespace HotelManagement
