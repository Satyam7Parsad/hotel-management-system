-- Hotel Management System - Seed Data
-- Sample data for testing and development

-- ==========================================
-- ROOM TYPES
-- ==========================================

INSERT INTO room_types (type_name, base_price, max_occupancy, description, amenities) VALUES
('Standard Single', 89.99, 1, 'Comfortable single room with all basic amenities',
 '{"wifi": true, "tv": true, "air_conditioning": true, "desk": true}'),
('Standard Double', 129.99, 2, 'Spacious double room perfect for couples',
 '{"wifi": true, "tv": true, "air_conditioning": true, "mini_fridge": true, "desk": true}'),
('Deluxe Double', 179.99, 2, 'Deluxe room with premium amenities and city view',
 '{"wifi": true, "tv": true, "air_conditioning": true, "mini_fridge": true, "coffee_maker": true, "balcony": true, "desk": true}'),
('Family Suite', 249.99, 4, 'Large suite with separate living area, perfect for families',
 '{"wifi": true, "tv": true, "air_conditioning": true, "mini_fridge": true, "coffee_maker": true, "microwave": true, "sofa_bed": true, "balcony": true, "desk": true}'),
('Executive Suite', 349.99, 2, 'Luxurious suite with premium furnishings and panoramic views',
 '{"wifi": true, "tv": true, "air_conditioning": true, "mini_bar": true, "coffee_maker": true, "jacuzzi": true, "king_bed": true, "balcony": true, "desk": true, "safe": true}'),
('Presidential Suite', 599.99, 4, 'Ultimate luxury with private lounge and exclusive services',
 '{"wifi": true, "multiple_tvs": true, "air_conditioning": true, "full_bar": true, "espresso_machine": true, "jacuzzi": true, "steam_shower": true, "king_bed": true, "separate_living_room": true, "dining_area": true, "balcony": true, "desk": true, "safe": true, "butler_service": true}');

-- ==========================================
-- ROOMS (50 rooms across 5 floors)
-- ==========================================

-- Floor 1 - Standard rooms (10 rooms: 101-110)
INSERT INTO rooms (room_number, room_type_id, floor_number, status) VALUES
('101', 1, 1, 'available'),
('102', 1, 1, 'available'),
('103', 2, 1, 'available'),
('104', 2, 1, 'occupied'),
('105', 2, 1, 'available'),
('106', 2, 1, 'available'),
('107', 2, 1, 'reserved'),
('108', 2, 1, 'available'),
('109', 2, 1, 'occupied'),
('110', 2, 1, 'available');

-- Floor 2 - Standard and Deluxe mix (10 rooms: 201-210)
INSERT INTO rooms (room_number, room_type_id, floor_number, status) VALUES
('201', 2, 2, 'available'),
('202', 2, 2, 'available'),
('203', 3, 2, 'occupied'),
('204', 3, 2, 'available'),
('205', 3, 2, 'available'),
('206', 3, 2, 'available'),
('207', 3, 2, 'reserved'),
('208', 3, 2, 'available'),
('209', 3, 2, 'available'),
('210', 3, 2, 'maintenance');

-- Floor 3 - Deluxe and Family Suites (10 rooms: 301-310)
INSERT INTO rooms (room_number, room_type_id, floor_number, status) VALUES
('301', 3, 3, 'available'),
('302', 3, 3, 'occupied'),
('303', 3, 3, 'available'),
('304', 4, 3, 'available'),
('305', 4, 3, 'occupied'),
('306', 4, 3, 'available'),
('307', 4, 3, 'available'),
('308', 4, 3, 'reserved'),
('309', 4, 3, 'available'),
('310', 4, 3, 'available');

-- Floor 4 - Executive Suites (10 rooms: 401-410)
INSERT INTO rooms (room_number, room_type_id, floor_number, status) VALUES
('401', 5, 4, 'available'),
('402', 5, 4, 'occupied'),
('403', 5, 4, 'available'),
('404', 5, 4, 'available'),
('405', 5, 4, 'available'),
('406', 5, 4, 'reserved'),
('407', 5, 4, 'available'),
('408', 5, 4, 'available'),
('409', 5, 4, 'occupied'),
('410', 5, 4, 'available');

-- Floor 5 - Premium Suites (10 rooms: 501-510)
INSERT INTO rooms (room_number, room_type_id, floor_number, status) VALUES
('501', 5, 5, 'available'),
('502', 5, 5, 'available'),
('503', 5, 5, 'occupied'),
('504', 5, 5, 'available'),
('505', 6, 5, 'available'),
('506', 6, 5, 'reserved'),
('507', 5, 5, 'available'),
('508', 5, 5, 'available'),
('509', 5, 5, 'available'),
('510', 6, 5, 'available');

-- ==========================================
-- GUESTS (20 sample guests)
-- ==========================================

INSERT INTO guests (first_name, last_name, email, phone, address, id_type, id_number, date_of_birth, nationality, vip_status, preferences) VALUES
('John', 'Smith', 'john.smith@email.com', '+1-555-0101', '123 Main St, New York, NY 10001', 'passport', 'P123456789', '1985-03-15', 'USA', false, '{"smoking": false, "floor_preference": "high", "pillow_type": "firm"}'),
('Emma', 'Johnson', 'emma.j@email.com', '+1-555-0102', '456 Oak Ave, Boston, MA 02101', 'drivers_license', 'DL987654321', '1990-07-22', 'USA', true, '{"smoking": false, "floor_preference": "high", "pillow_type": "soft", "dietary": "vegetarian"}'),
('Michael', 'Williams', 'mwilliams@email.com', '+1-555-0103', '789 Pine Rd, Chicago, IL 60601', 'passport', 'P234567890', '1978-11-05', 'USA', false, '{"smoking": false}'),
('Sophia', 'Brown', 'sophia.brown@email.com', '+44-20-5550104', '10 Baker St, London, UK', 'passport', 'UK876543210', '1988-04-18', 'UK', true, '{"smoking": false, "floor_preference": "high", "newspaper": "Financial Times"}'),
('James', 'Davis', 'james.davis@email.com', '+1-555-0105', '321 Elm St, Miami, FL 33101', 'drivers_license', 'DL456789123', '1995-09-30', 'USA', false, '{"smoking": false, "pillow_type": "firm"}'),
('Olivia', 'Martinez', 'olivia.m@email.com', '+1-555-0106', '654 Maple Dr, Los Angeles, CA 90001', 'passport', 'P345678901', '1982-12-12', 'USA', false, '{"smoking": false, "floor_preference": "low"}'),
('William', 'Garcia', 'will.garcia@email.com', '+1-555-0107', '987 Cedar Ln, Houston, TX 77001', 'drivers_license', 'DL789456123', '1975-06-25', 'USA', true, '{"smoking": false, "newspaper": "Wall Street Journal", "pillow_type": "memory_foam"}'),
('Ava', 'Rodriguez', 'ava.rod@email.com', '+34-91-5550108', 'Calle Mayor 5, Madrid, Spain', 'passport', 'ES654321987', '1992-02-14', 'Spain', false, '{"smoking": false, "language_preference": "Spanish"}'),
('Robert', 'Wilson', 'robert.w@email.com', '+1-555-0109', '147 Birch Ave, Seattle, WA 98101', 'passport', 'P456789012', '1980-08-08', 'USA', false, '{"smoking": false}'),
('Isabella', 'Anderson', 'isabella.a@email.com', '+1-555-0110', '258 Walnut St, Denver, CO 80201', 'drivers_license', 'DL321654987', '1987-01-20', 'USA', false, '{"smoking": false, "pillow_type": "soft"}'),
('David', 'Taylor', 'david.taylor@email.com', '+61-2-5550111', '15 Sydney Rd, Sydney, Australia', 'passport', 'AU123789456', '1983-05-16', 'Australia', true, '{"smoking": false, "floor_preference": "high"}'),
('Mia', 'Thomas', 'mia.thomas@email.com', '+1-555-0112', '369 Spruce Ct, Portland, OR 97201', 'passport', 'P567890123', '1991-10-03', 'USA', false, '{"smoking": false}'),
('Daniel', 'Moore', 'daniel.moore@email.com', '+1-555-0113', '741 Ash Blvd, Phoenix, AZ 85001', 'drivers_license', 'DL654987321', '1979-07-27', 'USA', false, '{"smoking": false, "late_checkout": "preferred"}'),
('Charlotte', 'Jackson', 'charlotte.j@email.com', '+33-1-5550114', '20 Rue de Paris, Paris, France', 'passport', 'FR987321654', '1986-03-09', 'France', true, '{"smoking": false, "language_preference": "French", "pillow_type": "soft"}'),
('Joseph', 'White', 'joseph.white@email.com', '+1-555-0115', '852 Fir Way, San Diego, CA 92101', 'passport', 'P678901234', '1993-11-21', 'USA', false, '{"smoking": false}'),
('Amelia', 'Harris', 'amelia.h@email.com', '+49-30-5550116', 'Berliner Str 30, Berlin, Germany', 'passport', 'DE321987654', '1984-06-14', 'Germany', false, '{"smoking": false, "language_preference": "German"}'),
('Christopher', 'Martin', 'chris.martin@email.com', '+1-555-0117', '963 Poplar Ave, Atlanta, GA 30301', 'drivers_license', 'DL987123654', '1977-09-05', 'USA', false, '{"smoking": false}'),
('Harper', 'Thompson', 'harper.t@email.com', '+1-555-0118', '159 Willow Dr, Nashville, TN 37201', 'passport', 'P789012345', '1989-12-18', 'USA', true, '{"smoking": false, "floor_preference": "high", "pillow_type": "firm"}'),
('Matthew', 'Lee', 'matt.lee@email.com', '+1-555-0119', '753 Cypress St, Dallas, TX 75201', 'drivers_license', 'DL159753486', '1981-04-11', 'USA', false, '{"smoking": false}'),
('Evelyn', 'Walker', 'evelyn.w@email.com', '+1-555-0120', '357 Redwood Ln, San Francisco, CA 94101', 'passport', 'P890123456', '1994-08-26', 'USA', false, '{"smoking": false, "dietary": "gluten_free"}');

-- ==========================================
-- BOOKINGS (mix of past, current, and future)
-- ==========================================

-- Current bookings (checked-in)
INSERT INTO bookings (guest_id, room_id, check_in_date, check_out_date, actual_check_in, status, num_adults, num_children, total_amount) VALUES
(1, 4, '2026-01-13', '2026-01-17', '2026-01-13 15:30:00', 'checked_in', 2, 0, 519.96),
(2, 9, '2026-01-14', '2026-01-18', '2026-01-14 14:15:00', 'checked_in', 2, 0, 519.96),
(3, 13, '2026-01-12', '2026-01-16', '2026-01-12 16:00:00', 'checked_in', 2, 0, 719.96),
(4, 22, '2026-01-11', '2026-01-20', '2026-01-11 15:00:00', 'checked_in', 2, 1, 2699.91),
(5, 25, '2026-01-13', '2026-01-19', '2026-01-13 14:30:00', 'checked_in', 4, 2, 1499.94),
(6, 32, '2026-01-14', '2026-01-17', '2026-01-14 16:15:00', 'checked_in', 2, 0, 1049.97),
(7, 39, '2026-01-10', '2026-01-18', '2026-01-10 15:45:00', 'checked_in', 2, 0, 2799.92),
(8, 43, '2026-01-15', '2026-01-19', '2026-01-15 14:00:00', 'checked_in', 2, 0, 1399.96);

-- Reserved bookings (future)
INSERT INTO bookings (guest_id, room_id, check_in_date, check_out_date, status, num_adults, num_children, total_amount) VALUES
(9, 7, '2026-01-17', '2026-01-20', 'confirmed', 2, 0, 389.97),
(10, 17, '2026-01-18', '2026-01-22', 'confirmed', 2, 0, 719.96),
(11, 28, '2026-01-19', '2026-01-23', 'confirmed', 2, 1, 999.96),
(12, 36, '2026-01-20', '2026-01-25', 'confirmed', 2, 0, 1749.95),
(13, 46, '2026-01-22', '2026-01-27', 'confirmed', 2, 0, 2999.95);

-- Past bookings (checked-out)
INSERT INTO bookings (guest_id, room_id, check_in_date, check_out_date, actual_check_in, actual_check_out, status, num_adults, num_children, total_amount) VALUES
(14, 15, '2026-01-05', '2026-01-09', '2026-01-05 15:00:00', '2026-01-09 11:00:00', 'checked_out', 2, 0, 719.96),
(15, 20, '2026-01-01', '2026-01-07', '2026-01-01 16:00:00', '2026-01-07 10:30:00', 'checked_out', 4, 2, 1499.94),
(16, 30, '2025-12-28', '2026-01-03', '2025-12-28 15:30:00', '2026-01-03 11:15:00', 'checked_out', 2, 0, 2099.94),
(17, 35, '2026-01-02', '2026-01-06', '2026-01-02 14:45:00', '2026-01-06 10:00:00', 'checked_out', 2, 0, 1399.96),
(18, 40, '2025-12-30', '2026-01-05', '2025-12-30 15:00:00', '2026-01-05 11:30:00', 'checked_out', 2, 0, 2099.94);

-- Pending bookings (awaiting confirmation)
INSERT INTO bookings (guest_id, room_id, check_in_date, check_out_date, status, num_adults, num_children, total_amount) VALUES
(19, 24, '2026-01-25', '2026-01-28', 'pending', 2, 0, 539.97),
(20, 34, '2026-01-26', '2026-01-30', 'pending', 2, 0, 1399.96);

-- Cancelled booking
INSERT INTO bookings (guest_id, room_id, check_in_date, check_out_date, status, num_adults, num_children, total_amount) VALUES
(16, 18, '2026-01-16', '2026-01-19', 'cancelled', 2, 0, 539.97);

-- ==========================================
-- SERVICES
-- ==========================================

INSERT INTO services (service_name, description, price, category, active) VALUES
('Room Service - Breakfast', 'Continental breakfast delivered to room', 25.00, 'Food & Beverage', true),
('Room Service - Lunch', 'Lunch menu delivered to room', 35.00, 'Food & Beverage', true),
('Room Service - Dinner', 'Dinner menu delivered to room', 45.00, 'Food & Beverage', true),
('Laundry Service', 'Per item laundry and pressing', 15.00, 'Housekeeping', true),
('Dry Cleaning', 'Per item dry cleaning', 20.00, 'Housekeeping', true),
('Airport Transfer', 'Private car service to/from airport', 75.00, 'Transportation', true),
('Spa Massage - 60min', 'Relaxing 60-minute massage', 120.00, 'Spa & Wellness', true),
('Spa Massage - 90min', 'Extended 90-minute massage', 180.00, 'Spa & Wellness', true),
('Mini Bar Refill', 'Full mini bar restock', 50.00, 'Food & Beverage', true),
('Late Checkout', 'Checkout extension until 6 PM', 75.00, 'Room Service', true),
('Extra Bed/Cot', 'Additional bed in room (per night)', 40.00, 'Room Service', true),
('Pet Accommodation', 'Pet-friendly room service (per night)', 30.00, 'Room Service', true);

-- ==========================================
-- BOOKING SERVICES (services used by guests)
-- ==========================================

-- Services for current bookings
INSERT INTO booking_services (booking_id, service_id, quantity, unit_price, total_price, service_date, notes) VALUES
(1, 1, 2, 25.00, 50.00, '2026-01-14 08:30:00', 'Room 104'),
(1, 4, 5, 15.00, 75.00, '2026-01-14 10:00:00', 'Shirts and pants'),
(2, 1, 3, 25.00, 75.00, '2026-01-15 08:00:00', 'Room 109'),
(3, 7, 1, 120.00, 120.00, '2026-01-13 14:00:00', '60min relaxation massage'),
(4, 1, 5, 25.00, 125.00, '2026-01-12 08:30:00', 'Family breakfast'),
(4, 6, 2, 75.00, 150.00, '2026-01-11 10:00:00', 'Airport pickup for 2'),
(5, 1, 4, 25.00, 100.00, '2026-01-14 09:00:00', 'Breakfast for 4'),
(5, 11, 1, 40.00, 40.00, '2026-01-13 15:00:00', 'Extra cot for child');

-- Services for past bookings
INSERT INTO booking_services (booking_id, service_id, quantity, unit_price, total_price, service_date, notes) VALUES
(13, 1, 4, 25.00, 100.00, '2026-01-06 08:30:00', 'Daily breakfast'),
(13, 8, 1, 180.00, 180.00, '2026-01-07 15:00:00', '90min couples massage'),
(14, 1, 6, 25.00, 150.00, '2026-01-02 09:00:00', 'Daily breakfast for family'),
(14, 12, 6, 30.00, 180.00, '2026-01-01 15:00:00', 'Small dog accommodation');

-- ==========================================
-- PAYMENTS
-- ==========================================

-- Payments for completed bookings
INSERT INTO payments (booking_id, amount, payment_method, payment_status, transaction_id, payment_date) VALUES
(13, 719.96, 'credit_card', 'completed', 'TXN-2026-001-CC', '2026-01-09 11:15:00'),
(14, 1499.94, 'credit_card', 'completed', 'TXN-2026-002-CC', '2026-01-07 10:45:00'),
(15, 2099.94, 'debit_card', 'completed', 'TXN-2026-003-DC', '2026-01-03 11:30:00'),
(16, 1399.96, 'credit_card', 'completed', 'TXN-2026-004-CC', '2026-01-06 10:15:00'),
(17, 2099.94, 'bank_transfer', 'completed', 'TXN-2026-005-BT', '2026-01-05 14:00:00');

-- Partial payments for current bookings
INSERT INTO payments (booking_id, amount, payment_method, payment_status, transaction_id, payment_date) VALUES
(1, 260.00, 'credit_card', 'completed', 'TXN-2026-006-CC', '2026-01-13 15:45:00'),
(2, 260.00, 'credit_card', 'completed', 'TXN-2026-007-CC', '2026-01-14 14:30:00'),
(4, 1350.00, 'credit_card', 'completed', 'TXN-2026-008-CC', '2026-01-11 15:15:00'),
(7, 1400.00, 'debit_card', 'completed', 'TXN-2026-009-DC', '2026-01-10 16:00:00');

-- Deposit payments for confirmed bookings
INSERT INTO payments (booking_id, amount, payment_method, payment_status, transaction_id, payment_date) VALUES
(9, 195.00, 'credit_card', 'completed', 'TXN-2026-010-CC', '2026-01-10 09:00:00'),
(10, 360.00, 'credit_card', 'completed', 'TXN-2026-011-CC', '2026-01-11 10:30:00'),
(11, 500.00, 'debit_card', 'completed', 'TXN-2026-012-DC', '2026-01-12 11:15:00'),
(12, 875.00, 'credit_card', 'completed', 'TXN-2026-013-CC', '2026-01-13 13:00:00'),
(13, 1500.00, 'bank_transfer', 'completed', 'TXN-2026-014-BT', '2026-01-14 09:30:00');

-- ==========================================
-- INVOICES
-- ==========================================

-- Invoices for completed bookings
INSERT INTO invoices (booking_id, invoice_number, issue_date, due_date, subtotal, tax_amount, discount_amount, total_amount, status) VALUES
(13, 'INV-2026-0001', '2026-01-09 11:00:00', '2026-01-09', 719.96, 71.996, 0, 791.956, 'paid'),
(14, 'INV-2026-0002', '2026-01-07 10:30:00', '2026-01-07', 1499.94, 149.994, 0, 1649.934, 'paid'),
(15, 'INV-2026-0003', '2026-01-03 11:15:00', '2026-01-03', 2099.94, 209.994, 100.00, 2209.934, 'paid'),
(16, 'INV-2026-0004', '2026-01-06 10:00:00', '2026-01-06', 1399.96, 139.996, 0, 1539.956, 'paid'),
(17, 'INV-2026-0005', '2026-01-05 11:30:00', '2026-01-05', 2099.94, 209.994, 0, 2309.934, 'paid');

-- Invoices for current bookings (partially paid)
INSERT INTO invoices (booking_id, invoice_number, issue_date, due_date, subtotal, tax_amount, discount_amount, total_amount, status) VALUES
(1, 'INV-2026-0006', '2026-01-13 15:30:00', '2026-01-17', 519.96, 51.996, 0, 571.956, 'partially_paid'),
(2, 'INV-2026-0007', '2026-01-14 14:15:00', '2026-01-18', 519.96, 51.996, 0, 571.956, 'partially_paid'),
(3, 'INV-2026-0008', '2026-01-12 16:00:00', '2026-01-16', 719.96, 71.996, 0, 791.956, 'unpaid'),
(4, 'INV-2026-0009', '2026-01-11 15:00:00', '2026-01-20', 2699.91, 269.991, 0, 2969.901, 'partially_paid');

-- ==========================================
-- COMPLETION MESSAGE
-- ==========================================

SELECT 'Sample data inserted successfully!' AS message;
SELECT 'Summary:' AS info;
SELECT COUNT(*) AS room_types FROM room_types;
SELECT COUNT(*) AS rooms FROM rooms;
SELECT COUNT(*) AS guests FROM guests;
SELECT COUNT(*) AS bookings FROM bookings;
SELECT COUNT(*) AS services FROM services;
SELECT COUNT(*) AS payments FROM payments;
SELECT COUNT(*) AS invoices FROM invoices;
