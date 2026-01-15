-- Hotel Management System Database Schema
-- PostgreSQL 12+

-- Drop existing tables if they exist (for clean setup)
DROP TABLE IF EXISTS audit_log CASCADE;
DROP TABLE IF EXISTS booking_services CASCADE;
DROP TABLE IF EXISTS services CASCADE;
DROP TABLE IF EXISTS invoices CASCADE;
DROP TABLE IF EXISTS payments CASCADE;
DROP TABLE IF EXISTS bookings CASCADE;
DROP TABLE IF EXISTS guests CASCADE;
DROP TABLE IF EXISTS rooms CASCADE;
DROP TABLE IF EXISTS room_types CASCADE;

-- Drop views if they exist
DROP VIEW IF EXISTS available_rooms_view;
DROP VIEW IF EXISTS current_occupancy_view;
DROP VIEW IF EXISTS booking_revenue_view;

-- Drop functions if they exist
DROP FUNCTION IF EXISTS update_updated_at_column() CASCADE;

-- ==========================================
-- TABLE DEFINITIONS
-- ==========================================

-- 1. Room Types Table
CREATE TABLE room_types (
    id SERIAL PRIMARY KEY,
    type_name VARCHAR(50) NOT NULL UNIQUE,
    base_price DECIMAL(10, 2) NOT NULL CHECK (base_price >= 0),
    max_occupancy INT NOT NULL CHECK (max_occupancy > 0),
    description TEXT,
    amenities JSONB DEFAULT '{}',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

COMMENT ON TABLE room_types IS 'Defines different types of rooms (Standard, Deluxe, Suite, etc.)';
COMMENT ON COLUMN room_types.amenities IS 'JSON object containing amenities like WiFi, TV, Mini-bar, etc.';

-- 2. Rooms Table
CREATE TABLE rooms (
    id SERIAL PRIMARY KEY,
    room_number VARCHAR(10) NOT NULL UNIQUE,
    room_type_id INT NOT NULL REFERENCES room_types(id) ON DELETE RESTRICT,
    floor_number INT NOT NULL CHECK (floor_number > 0),
    status VARCHAR(20) NOT NULL DEFAULT 'available'
        CHECK (status IN ('available', 'occupied', 'maintenance', 'reserved')),
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_rooms_status ON rooms(status);
CREATE INDEX idx_rooms_type ON rooms(room_type_id);
CREATE INDEX idx_rooms_floor ON rooms(floor_number);

COMMENT ON TABLE rooms IS 'Individual hotel rooms with their current status';
COMMENT ON COLUMN rooms.status IS 'Current room status: available, occupied, maintenance, or reserved';

-- 3. Guests Table
CREATE TABLE guests (
    id SERIAL PRIMARY KEY,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    email VARCHAR(255) UNIQUE,
    phone VARCHAR(20) NOT NULL,
    address TEXT,
    id_type VARCHAR(50) NOT NULL CHECK (id_type IN ('passport', 'drivers_license', 'national_id')),
    id_number VARCHAR(100) NOT NULL,
    date_of_birth DATE,
    nationality VARCHAR(100),
    preferences JSONB DEFAULT '{}',
    vip_status BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_guests_email ON guests(email);
CREATE INDEX idx_guests_phone ON guests(phone);
CREATE INDEX idx_guests_name ON guests(last_name, first_name);
CREATE INDEX idx_guests_vip ON guests(vip_status);

COMMENT ON TABLE guests IS 'Guest information and profiles';
COMMENT ON COLUMN guests.preferences IS 'JSON object for guest preferences (smoking, floor, pillows, etc.)';

-- 4. Bookings/Reservations Table
CREATE TABLE bookings (
    id SERIAL PRIMARY KEY,
    guest_id INT NOT NULL REFERENCES guests(id) ON DELETE RESTRICT,
    room_id INT NOT NULL REFERENCES rooms(id) ON DELETE RESTRICT,
    check_in_date DATE NOT NULL,
    check_out_date DATE NOT NULL,
    actual_check_in TIMESTAMP,
    actual_check_out TIMESTAMP,
    num_adults INT NOT NULL DEFAULT 1 CHECK (num_adults > 0),
    num_children INT DEFAULT 0 CHECK (num_children >= 0),
    status VARCHAR(20) NOT NULL DEFAULT 'pending'
        CHECK (status IN ('pending', 'confirmed', 'checked_in', 'checked_out', 'cancelled')),
    special_requests TEXT,
    total_amount DECIMAL(10, 2),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT valid_dates CHECK (check_out_date > check_in_date),
    CONSTRAINT valid_actual_dates CHECK (actual_check_out IS NULL OR actual_check_out > actual_check_in)
);

CREATE INDEX idx_bookings_guest ON bookings(guest_id);
CREATE INDEX idx_bookings_room ON bookings(room_id);
CREATE INDEX idx_bookings_dates ON bookings(check_in_date, check_out_date);
CREATE INDEX idx_bookings_status ON bookings(status);
CREATE INDEX idx_bookings_check_in ON bookings(check_in_date);
CREATE INDEX idx_bookings_check_out ON bookings(check_out_date);

COMMENT ON TABLE bookings IS 'Hotel reservations and bookings';
COMMENT ON COLUMN bookings.status IS 'Booking lifecycle: pending → confirmed → checked_in → checked_out (or cancelled)';

-- 5. Payments Table
CREATE TABLE payments (
    id SERIAL PRIMARY KEY,
    booking_id INT NOT NULL REFERENCES bookings(id) ON DELETE RESTRICT,
    amount DECIMAL(10, 2) NOT NULL CHECK (amount > 0),
    payment_method VARCHAR(50) NOT NULL
        CHECK (payment_method IN ('cash', 'credit_card', 'debit_card', 'bank_transfer', 'mobile_payment')),
    payment_status VARCHAR(20) NOT NULL DEFAULT 'pending'
        CHECK (payment_status IN ('pending', 'completed', 'failed', 'refunded')),
    transaction_id VARCHAR(255) UNIQUE,
    payment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_payments_booking ON payments(booking_id);
CREATE INDEX idx_payments_status ON payments(payment_status);
CREATE INDEX idx_payments_date ON payments(payment_date);

COMMENT ON TABLE payments IS 'Payment transactions for bookings';

-- 6. Invoices Table
CREATE TABLE invoices (
    id SERIAL PRIMARY KEY,
    booking_id INT NOT NULL REFERENCES bookings(id) ON DELETE RESTRICT,
    invoice_number VARCHAR(50) NOT NULL UNIQUE,
    issue_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    due_date DATE,
    subtotal DECIMAL(10, 2) NOT NULL CHECK (subtotal >= 0),
    tax_amount DECIMAL(10, 2) DEFAULT 0 CHECK (tax_amount >= 0),
    discount_amount DECIMAL(10, 2) DEFAULT 0 CHECK (discount_amount >= 0),
    total_amount DECIMAL(10, 2) NOT NULL CHECK (total_amount >= 0),
    status VARCHAR(20) NOT NULL DEFAULT 'unpaid'
        CHECK (status IN ('unpaid', 'partially_paid', 'paid', 'cancelled')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_invoices_booking ON invoices(booking_id);
CREATE INDEX idx_invoices_status ON invoices(status);
CREATE INDEX idx_invoices_number ON invoices(invoice_number);

COMMENT ON TABLE invoices IS 'Generated invoices for bookings';

-- 7. Additional Services Table (room service, laundry, etc.)
CREATE TABLE services (
    id SERIAL PRIMARY KEY,
    service_name VARCHAR(100) NOT NULL,
    description TEXT,
    price DECIMAL(10, 2) NOT NULL CHECK (price >= 0),
    category VARCHAR(50),
    active BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_services_category ON services(category);
CREATE INDEX idx_services_active ON services(active);

COMMENT ON TABLE services IS 'Available hotel services (room service, laundry, spa, etc.)';

-- 8. Booking Services Junction Table
CREATE TABLE booking_services (
    id SERIAL PRIMARY KEY,
    booking_id INT NOT NULL REFERENCES bookings(id) ON DELETE CASCADE,
    service_id INT NOT NULL REFERENCES services(id) ON DELETE RESTRICT,
    quantity INT NOT NULL DEFAULT 1 CHECK (quantity > 0),
    unit_price DECIMAL(10, 2) NOT NULL CHECK (unit_price >= 0),
    total_price DECIMAL(10, 2) NOT NULL CHECK (total_price >= 0),
    service_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    notes TEXT
);

CREATE INDEX idx_booking_services_booking ON booking_services(booking_id);
CREATE INDEX idx_booking_services_service ON booking_services(service_id);

COMMENT ON TABLE booking_services IS 'Services consumed by guests during their stay';

-- 9. Audit Log Table (for tracking changes)
CREATE TABLE audit_log (
    id SERIAL PRIMARY KEY,
    table_name VARCHAR(50) NOT NULL,
    record_id INT NOT NULL,
    action VARCHAR(20) NOT NULL CHECK (action IN ('INSERT', 'UPDATE', 'DELETE')),
    old_data JSONB,
    new_data JSONB,
    changed_by VARCHAR(100),
    changed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_audit_table_record ON audit_log(table_name, record_id);
CREATE INDEX idx_audit_changed_at ON audit_log(changed_at);

COMMENT ON TABLE audit_log IS 'Audit trail for data changes';

-- ==========================================
-- VIEWS
-- ==========================================

-- Available rooms view
CREATE VIEW available_rooms_view AS
SELECT
    r.id,
    r.room_number,
    r.floor_number,
    rt.type_name,
    rt.base_price,
    rt.max_occupancy,
    rt.amenities,
    rt.description
FROM rooms r
JOIN room_types rt ON r.room_type_id = rt.id
WHERE r.status = 'available'
ORDER BY r.floor_number, r.room_number;

COMMENT ON VIEW available_rooms_view IS 'Currently available rooms with their details';

-- Current occupancy view
CREATE VIEW current_occupancy_view AS
SELECT
    r.room_number,
    r.floor_number,
    rt.type_name,
    g.first_name || ' ' || g.last_name AS guest_name,
    g.phone AS guest_phone,
    b.check_in_date,
    b.check_out_date,
    b.num_adults,
    b.num_children,
    b.id AS booking_id
FROM bookings b
JOIN rooms r ON b.room_id = r.id
JOIN room_types rt ON r.room_type_id = rt.id
JOIN guests g ON b.guest_id = g.id
WHERE b.status = 'checked_in'
ORDER BY r.room_number;

COMMENT ON VIEW current_occupancy_view IS 'Currently occupied rooms with guest information';

-- Booking revenue summary view
CREATE VIEW booking_revenue_view AS
SELECT
    b.id AS booking_id,
    b.check_in_date,
    b.check_out_date,
    g.first_name || ' ' || g.last_name AS guest_name,
    r.room_number,
    rt.type_name AS room_type,
    b.total_amount,
    COALESCE(SUM(p.amount), 0) AS total_paid,
    b.total_amount - COALESCE(SUM(p.amount), 0) AS balance,
    b.status AS booking_status
FROM bookings b
JOIN guests g ON b.guest_id = g.id
JOIN rooms r ON b.room_id = r.id
JOIN room_types rt ON r.room_type_id = rt.id
LEFT JOIN payments p ON b.id = p.booking_id AND p.payment_status = 'completed'
GROUP BY b.id, g.first_name, g.last_name, r.room_number, rt.type_name
ORDER BY b.check_in_date DESC;

COMMENT ON VIEW booking_revenue_view IS 'Booking revenue with payment status';

-- ==========================================
-- FUNCTIONS AND TRIGGERS
-- ==========================================

-- Function to update updated_at timestamp
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = CURRENT_TIMESTAMP;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

-- Apply updated_at trigger to relevant tables
CREATE TRIGGER update_room_types_updated_at
    BEFORE UPDATE ON room_types
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_rooms_updated_at
    BEFORE UPDATE ON rooms
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_guests_updated_at
    BEFORE UPDATE ON guests
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_bookings_updated_at
    BEFORE UPDATE ON bookings
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

-- ==========================================
-- GRANT PERMISSIONS (adjust user as needed)
-- ==========================================

-- Example: GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO hotel_user;
-- Example: GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO hotel_user;

-- ==========================================
-- COMPLETION MESSAGE
-- ==========================================

SELECT 'Hotel Management System database schema created successfully!' AS message;
