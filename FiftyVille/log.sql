-- Keep a log of any SQL queries you execute as you solve the mystery.

--airports              crime_scene_reports   people
--atm_transactions      flights               phone_calls
--bakery_security_logs  interviews
--bank_accounts         passengers

--Find what we have in the day of the crime
SELECT * FROM crime_scene_reports WHERE year = 2024 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

--Seach a little be more about the bakery mention
SELECT * FROM interviews  WHERE year = 2024 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';

--In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
SELECT * FROM flights  WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airports_id IN (SELECT id FROM airports WHERE city LIKE 'Fiftyville');
--| 36 | 8                 | 4                      | 2024 | 7     | 29  | 8    | 20     |se
--| 4  | LGA          | LaGuardia Airport                       | New York City |


SELECT pl.name FROM people pl
WHERE pl.passport_number IN (
    SELECT passport_number FROM passengers WHERE flight_id = 36
);


--+--------+
--|  name  |
--+--------+
--| Kenny  |
--| Sofia  |
--| Taylor |
--| Luca   |
--| Kelsey |
--| Edward |
--| Bruce  |
--| Doris  |
--+--------+

--I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
SELECT pl.name FROM people pl
WHERE pl.passport_number IN (
    SELECT passport_number FROM passengers WHERE flight_id = 36
)
AND
pl.id IN
(
    SELECT person_id FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE atm_location LIKE 'Leggett Street' AND day = 28 AND transaction_type LIKE 'withdraw'
    )
);
--+--------+
--|  name  |
--+--------+
--| Kenny  |
--| Taylor |
--| Luca   |
--| Bruce  |
--+--------+



--they called someone who talked to them for less than a minute.
SELECT pl.name FROM people pl
WHERE pl.passport_number IN (
    SELECT passport_number FROM passengers WHERE flight_id = 36
)
AND
pl.id IN
(
    SELECT person_id FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions
        WHERE atm_location LIKE 'Leggett Street' AND day = 28 AND transaction_type LIKE 'withdraw'
    )
)
AND
pl.phone_number IN
(
    SELECT pc.caller FROM phone_calls pc
    WHERE pc.year = 2024 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60
);

--+--------+
--|  name  |
--+--------+
--| Kenny  |
--| Taylor |
--| Bruce  |
--+--------+

--bakery parking lot, you might want to look for cars that left the parking lot in that time frame.


SELECT pl.name FROM people pl
WHERE pl.phone_number IN (
    SELECT pc.receiver FROM phone_calls pc
    WHERE pc.year = 2024 AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60 AND pc.caller IN
    (SELECT phone_number FROM people WHERE name LIKE 'Bruce')
);
