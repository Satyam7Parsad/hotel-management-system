# Hotel Management System

A modern, feature-rich hotel management system built with C++20, ImGui, and PostgreSQL.

## Features

- **Room Management**: Track rooms, room types, availability, and status
- **Guest Management**: Guest profiles, contact information, booking history
- **Booking System**: Reservations, check-in/check-out, availability checking
- **Billing & Payments**: Invoice generation, payment processing, pricing

## Tech Stack

- **Language**: C++20
- **UI Framework**: Dear ImGui with glass morphism design
- **Database**: PostgreSQL with libpqxx
- **Build System**: CMake 3.15+
- **Graphics**: OpenGL 3.3+ with GLFW

## Project Structure

```
hotel-management-system/
├── include/              # Header files
│   ├── core/            # Core application (Application, Config)
│   ├── database/        # Database layer
│   │   ├── models/     # Data models (Room, Guest, Booking, etc.)
│   │   └── repositories/ # Data access layer (CRUD operations)
│   ├── ui/              # User interface
│   │   ├── components/ # Reusable UI components
│   │   └── views/      # Main application views
│   └── utils/           # Utilities (Logger, DateUtils, Validators)
├── src/                 # Implementation files (mirrors include/)
├── external/            # Third-party libraries
│   ├── imgui/          # ImGui library
│   └── implot/         # ImPlot for charts
├── assets/              # Assets (fonts, shaders)
├── config/              # Configuration files
├── scripts/             # Database setup scripts
└── CMakeLists.txt       # Build configuration
```

## Prerequisites

### macOS
```bash
brew install postgresql libpqxx glfw cmake
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install postgresql libpqxx-dev libglfw3-dev cmake build-essential
```

### Windows
- Install PostgreSQL from https://www.postgresql.org/download/windows/
- Install libpqxx (use vcpkg or build from source)
- Install GLFW (https://www.glfw.org/)
- Install CMake (https://cmake.org/)

## Database Setup

1. **Start PostgreSQL**:
   ```bash
   # macOS
   brew services start postgresql

   # Linux
   sudo systemctl start postgresql
   ```

2. **Create database and user**:
   ```bash
   createdb hotel_management_db
   psql postgres -c "CREATE USER hotel_user WITH PASSWORD 'your_password';"
   psql postgres -c "GRANT ALL PRIVILEGES ON DATABASE hotel_management_db TO hotel_user;"
   ```

3. **Run database schema**:
   ```bash
   psql -U hotel_user -d hotel_management_db -f scripts/setup_db.sql
   ```

4. **Load sample data** (optional):
   ```bash
   psql -U hotel_user -d hotel_management_db -f scripts/seed_data.sql
   ```

## Configuration

1. **Copy the example config**:
   ```bash
   cp config/database.ini.example config/database.ini
   ```

2. **Edit `config/database.ini`** with your database credentials:
   ```ini
   [database]
   host=localhost
   port=5432
   dbname=hotel_management_db
   user=hotel_user
   password=your_password_here
   ```

## Building

1. **Create build directory**:
   ```bash
   mkdir build && cd build
   ```

2. **Configure with CMake**:
   ```bash
   cmake ..
   ```

3. **Build**:
   ```bash
   cmake --build .
   ```

   Or for faster parallel builds:
   ```bash
   cmake --build . -j$(nproc)  # Linux
   cmake --build . -j$(sysctl -n hw.ncpu)  # macOS
   ```

4. **Run**:
   ```bash
   ./bin/HotelManagementSystem
   ```

## Development

### Debug Build
```bash
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Release Build
```bash
mkdir build-release && cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Architecture

### Core Infrastructure (Phase 2 - ✅ Complete)

- **Logger**: Thread-safe file logging with levels (DEBUG, INFO, WARNING, ERROR)
- **Config**: INI file parser for configuration management
- **DateUtils**: Date/time utilities (parsing, formatting, validation, calculations)
- **Validators**: Input validation (email, phone, prices, names, credit cards)
- **DatabaseManager**: PostgreSQL connection management with libpqxx
- **Models**: Data structures for Room, Guest, Booking, Payment, Invoice, Service

### Data Models

- **RoomType**: Room categories (Standard, Deluxe, Suite, etc.)
- **Room**: Individual hotel rooms with status tracking
- **Guest**: Guest profiles and preferences
- **Booking**: Reservations with check-in/out tracking
- **Payment**: Payment transactions with multiple methods
- **Invoice**: Invoice generation and tracking
- **Service**: Additional services (room service, laundry, etc.)

### Database Schema

- 9 core tables with foreign keys and constraints
- Views for common queries (available rooms, current occupancy, revenue)
- Triggers for automatic timestamp updates
- Indexes for performance
- JSONB columns for flexible data (amenities, preferences)

## Next Steps

The following components are ready to be implemented:

### Phase 3: Repository Layer
- RoomRepository
- GuestRepository
- BookingRepository
- PaymentRepository
- InvoiceRepository

### Phase 4: UI Foundation
- Application bootstrap
- UIManager
- Theme system (glass morphism)
- Reusable components (Table, Form, Modal, DatePicker)

### Phase 5: Main Views
- Dashboard View
- Rooms View
- Guests View
- Bookings View
- Check-In/Out View
- Billing View
- Reports View

## Current Status

✅ **Completed**:
- Project structure setup
- Build system (CMake)
- Database schema (9 tables, views, triggers)
- Seed data (50 rooms, 20 guests, sample bookings)
- Core infrastructure (Logger, Config, DateUtils, Validators)
- Database connection layer (DatabaseManager)
- All data models (Room, Guest, Booking, Payment, Invoice, Service)
- ImGui and ImPlot integration

🚧 **In Progress**:
- Repository layer implementation

📋 **Planned**:
- UI components and views
- Business logic
- Testing and optimization

## Database Schema Overview

### Tables
- `room_types`: Room categories with pricing
- `rooms`: Individual rooms with status
- `guests`: Guest information and preferences
- `bookings`: Reservations and check-ins
- `payments`: Payment transactions
- `invoices`: Generated invoices
- `services`: Available services
- `booking_services`: Services consumed by guests
- `audit_log`: Change tracking

### Sample Data
- 50 rooms across 5 floors
- 6 room types (Single, Double, Deluxe, Family, Executive, Presidential)
- 20 sample guests
- 20+ bookings (past, current, future)
- 12 available services
- Complete payment and invoice records

## License

MIT License

## Contributing

Contributions are welcome! Please follow the existing code style and architecture patterns.

## Support

For issues or questions, please open an issue on GitHub.
