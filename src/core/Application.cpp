#include "core/Application.hpp"
#include "utils/Logger.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace HotelManagement {

Application::Application() {}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
    Logger::info("Initializing Hotel Management System...");

    // Load configuration
    if (!config.load("config/database.ini")) {
        Logger::warning("Could not load config file, using defaults");
    }

    windowWidth = config.getWindowWidth();
    windowHeight = config.getWindowHeight();
    windowTitle = config.getWindowTitle();

    if (!initWindow()) {
        Logger::error("Failed to initialize window");
        return false;
    }

    if (!initImGui()) {
        Logger::error("Failed to initialize ImGui");
        return false;
    }

    if (!initDatabase()) {
        Logger::error("Failed to initialize database");
        return false;
    }

    if (!initRepositories()) {
        Logger::error("Failed to initialize repositories");
        return false;
    }

    running = true;
    Logger::info("Application initialized successfully");
    return true;
}

bool Application::initWindow() {
    if (!glfwInit()) {
        Logger::error("Failed to initialize GLFW");
        return false;
    }

    // OpenGL 3.3 + GLSL 330
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        Logger::error("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    Logger::info("Window created: ", windowWidth, "x", windowHeight);
    return true;
}

bool Application::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup style
    ImGui::StyleColorsDark();

    // Glass morphism colors
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 8.0f;
    style.TabRounding = 8.0f;
    style.Alpha = 0.95f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.15f, 0.9f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.3f, 0.8f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.3f, 0.3f, 0.4f, 0.9f);
    colors[ImGuiCol_Button] = ImVec4(0.4f, 0.2f, 0.8f, 0.8f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.5f, 0.3f, 0.9f, 0.9f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.6f, 0.4f, 1.0f, 1.0f);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Logger::info("ImGui initialized");
    return true;
}

bool Application::initDatabase() {
    try {
        std::string connStr = config.buildConnectionString();
        dbManager = std::make_unique<DatabaseManager>(connStr);

        if (!dbManager->connect()) {
            Logger::error("Failed to connect to database");
            return false;
        }

        Logger::info("Database connected successfully");
        return true;
    } catch (const std::exception& e) {
        Logger::error("Database initialization failed: ", e.what());
        return false;
    }
}

bool Application::initRepositories() {
    try {
        roomRepo = std::make_unique<RoomRepository>(*dbManager);
        guestRepo = std::make_unique<GuestRepository>(*dbManager);
        bookingRepo = std::make_unique<BookingRepository>(*dbManager);

        Logger::info("Repositories initialized");
        return true;
    } catch (const std::exception& e) {
        Logger::error("Repository initialization failed: ", e.what());
        return false;
    }
}

void Application::run() {
    Logger::info("Starting main loop...");

    while (running && !glfwWindowShouldClose(window)) {
        processEvents();
        update();
        render();
    }

    Logger::info("Main loop ended");
}

void Application::processEvents() {
    glfwPollEvents();
}

void Application::update() {
    // Update logic here
}

void Application::render() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render UI
    renderUI();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void Application::renderUI() {
    renderMenuBar();

    // Main content area
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight - 20));
    ImGui::Begin("MainWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    renderSidebar();
    ImGui::SameLine();
    renderMainContent();

    ImGui::End();
}

void Application::renderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
                running = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Dashboard")) currentView = 0;
            if (ImGui::MenuItem("Rooms")) currentView = 1;
            if (ImGui::MenuItem("Guests")) currentView = 2;
            if (ImGui::MenuItem("Bookings")) currentView = 3;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Application::renderSidebar() {
    ImGui::BeginChild("Sidebar", ImVec2(200, -1), true);

    ImGui::Text("Hotel Management");
    ImGui::Separator();

    if (ImGui::Selectable("Dashboard", currentView == 0)) currentView = 0;
    if (ImGui::Selectable("Rooms", currentView == 1)) currentView = 1;
    if (ImGui::Selectable("Guests", currentView == 2)) currentView = 2;
    if (ImGui::Selectable("Bookings", currentView == 3)) currentView = 3;

    ImGui::EndChild();
}

void Application::renderMainContent() {
    ImGui::BeginChild("MainContent", ImVec2(0, -1), true);

    switch (currentView) {
        case 0: renderDashboard(); break;
        case 1: renderRoomsView(); break;
        case 2: renderGuestsView(); break;
        case 3: renderBookingsView(); break;
    }

    ImGui::EndChild();
}

void Application::renderDashboard() {
    ImGui::Text("Dashboard");
    ImGui::Separator();

    // Get statistics
    int totalRooms = roomRepo->getTotalRooms();
    int occupiedRooms = roomRepo->getRoomsByStatus(RoomStatus::Occupied);
    int availableRooms = roomRepo->getRoomsByStatus(RoomStatus::Available);
    int totalGuests = guestRepo->getTotalGuests();
    int activeBookings = bookingRepo->getActiveBookingsCount();

    // Display metrics
    ImGui::Text("Total Rooms: %d", totalRooms);
    ImGui::Text("Occupied: %d", occupiedRooms);
    ImGui::Text("Available: %d", availableRooms);
    ImGui::Separator();
    ImGui::Text("Total Guests: %d", totalGuests);
    ImGui::Text("Active Bookings: %d", activeBookings);
}

void Application::renderRoomsView() {
    ImGui::Text("Rooms Management");
    ImGui::Separator();

    auto rooms = roomRepo->findAll();

    if (ImGui::BeginTable("RoomsTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Room #");
        ImGui::TableSetupColumn("Floor");
        ImGui::TableSetupColumn("Type ID");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (const auto& room : rooms) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%s", room.roomNumber.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%d", room.floorNumber);
            ImGui::TableNextColumn();
            ImGui::Text("%d", room.roomTypeId);
            ImGui::TableNextColumn();
            ImGui::Text("%s", room.statusToString().c_str());
            ImGui::TableNextColumn();
            if (ImGui::SmallButton("View")) {
                // View room details
            }
        }

        ImGui::EndTable();
    }
}

void Application::renderGuestsView() {
    ImGui::Text("Guests Management");
    ImGui::Separator();

    auto guests = guestRepo->findAll();

    if (ImGui::BeginTable("GuestsTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Email");
        ImGui::TableSetupColumn("Phone");
        ImGui::TableSetupColumn("VIP");
        ImGui::TableHeadersRow();

        for (const auto& guest : guests) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%d", guest.id);
            ImGui::TableNextColumn();
            ImGui::Text("%s", guest.getFullName().c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", guest.email.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", guest.phone.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", guest.vipStatus ? "Yes" : "No");
        }

        ImGui::EndTable();
    }
}

void Application::renderBookingsView() {
    ImGui::Text("Bookings Management");
    ImGui::Separator();

    auto bookings = bookingRepo->findAll();

    if (ImGui::BeginTable("BookingsTable", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Guest ID");
        ImGui::TableSetupColumn("Room ID");
        ImGui::TableSetupColumn("Check-in");
        ImGui::TableSetupColumn("Check-out");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();

        for (const auto& booking : bookings) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%d", booking.id);
            ImGui::TableNextColumn();
            ImGui::Text("%d", booking.guestId);
            ImGui::TableNextColumn();
            ImGui::Text("%d", booking.roomId);
            ImGui::TableNextColumn();
            ImGui::Text("%s", booking.checkInDate.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", booking.checkOutDate.c_str());
            ImGui::TableNextColumn();
            ImGui::Text("%s", booking.statusToString().c_str());
        }

        ImGui::EndTable();
    }
}

void Application::shutdown() {
    if (window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();

        Logger::info("Application shut down");
    }
}

} // namespace HotelManagement
