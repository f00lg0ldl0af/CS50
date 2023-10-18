-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find crime scene description
SELECT description
  FROM crime_scene_reports
 WHERE month = 7
   AND day = 28
   AND street = 'Humphrey Street';
/* Theft of the CS50 duck took place at 10:15am at the
Humphrey Street bakery. Interviews were conducted today
with three witnesses who were present at the time – each
of their interview transcripts mentions the bakery.
Littering took place at 16:36. No known witnesses.*/

-- Find crime details from relevant interviews
SELECT name, day, month, year, transcript
  FROM interviews
 WHERE transcript LIKE '%bakery';
/*
Interviewee: Ruth
Sometime within ten minutes of the theft,
I saw the thief get into a car in the bakery parking lot
and drive away. If you have security footage from the bakery
parking lot, you might want to look for cars that left the
parking lot in that time frame.

TODO: GET SECURITY FOOTAGE (10.15 - 10.30AM)

Interviewee: Eugene
I don't know the thief's name, but it was someone I recognized.
Earlier this morning, before I arrived at Emma's bakery,
I was walking by the ATM on Leggett Street and saw the thief
there withdrawing some money.

TODO: GET BANK ACCOUNT

Interviewee: Raymond
As the thief was leaving the bakery, they called someone who
talked to them for less than a minute. In the call, I heard the
thief say that they were planning to take the earliest flight
out of Fiftyville tomorrow. The thief then asked the person
on the other end of the phone to purchase the flight ticket.

TODO: GET CALL RECIPIENT NUMBER, FLIGHT DETAILS
*/


-- Find security footage of car leaving
SELECT hour, minute, license_plate FROM bakery_security_logs
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND activity = 'exit';

/*
+------+--------+---------------+
| hour | minute | license_plate |
+------+--------+---------------+
| 10   | 16     | 5P2BI95       |
| 10   | 18     | 94KL13X       |
| 10   | 18     | 6P58WS2       |
| 10   | 19     | 4328GD8       |
| 10   | 20     | G412CB7       |
| 10   | 21     | L93JTIZ       |
| 10   | 23     | 322W7JE       |
| 10   | 23     | 0NTHK55       |
| 10   | 35     | 1106N58       |
+------+--------+---------------+
*/
-- Match license plate to person (suspect list - 9)
SELECT bsl.license_plate, people.name, people.phone_number, people.passport_number
  FROM bakery_security_logs AS bsl
       INNER JOIN people
       ON bsl.license_plate = people.license_plate
 WHERE year = 2021
   AND month = 7
   AND day = 28
   AND hour = 10
   AND activity = 'exit';
/*
+---------------+---------+----------------+-----------------+
| license_plate |  name   |  phone_number  | passport_number |
+---------------+---------+----------------+-----------------+
| 5P2BI95       | Vanessa | (725) 555-4692 | 2963008352      |
| 94KL13X       | Bruce   | (367) 555-5533 | 5773159633      |
| 6P58WS2       | Barry   | (301) 555-4174 | 7526138472      |
| 4328GD8       | Luca    | (389) 555-5198 | 8496433585      |
| G412CB7       | Sofia   | (130) 555-0289 | 1695452385      |
| L93JTIZ       | Iman    | (829) 555-5269 | 7049073643      |
| 322W7JE       | Diana   | (770) 555-1861 | 3592750733      |
| 0NTHK55       | Kelsey  | (499) 555-9472 | 8294398571      |
| 1106N58       | Taylor  | (286) 555-6063 | 1988161715      |
+---------------+---------+----------------+-----------------+
*/
-- Find ATM (Leggett Street) transaction (suspect list - 5)
SELECT people.name, people.phone_number, people.passport_number,
       atx.account_number, atx.amount,
       bka.person_id

  FROM atm_transactions AS atx
       INNER JOIN bank_accounts AS bka
       ON atx.account_number = bka.account_number

       INNER JOIN people
       ON bka.person_id = people.id

       INNER JOIN bakery_security_logs AS bsl
       ON bsl.license_plate = people.license_plate

 WHERE bsl.year = 2021
   AND bsl.month = 7
   AND bsl.day = 28
   AND bsl.hour = 10
   AND bsl.activity = 'exit'
   AND atm_location = 'Leggett Street'
   AND transaction_type = 'withdraw';
/*
+--------+----------------+-----------------+----------------+--------+-----------+
|  name  |  phone_number  | passport_number | account_number | amount | person_id |
+--------+----------------+-----------------+----------------+--------+-----------+
| Bruce  | (367) 555-5533 | 5773159633      | 49610011       | 10     | 686048    |
| Bruce  | (367) 555-5533 | 5773159633      | 49610011       | 50     | 686048    |
| Diana  | (770) 555-1861 | 3592750733      | 26013199       | 35     | 514354    |
| Iman   | (829) 555-5269 | 7049073643      | 25506511       | 20     | 396669    |
| Luca   | (389) 555-5198 | 8496433585      | 28500762       | 48     | 467400    |
| Taylor | (286) 555-6063 | 1988161715      | 76054385       | 60     | 449774    |
+--------+----------------+-----------------+----------------+--------+-----------+
*/

-- Find passenger due for earliest flights 29 Jul 2021 (suspect list - 4)
SELECT f.id, f.origin_airport_id, f.destination_airport_id,
       f.hour, f.minute,
       people.name, people.phone_number, people.passport_number,
       psg.flight_id, psg.passport_number
  FROM passengers AS psg
       INNER JOIN flights AS f
       ON psg.flight_id = f.id

       INNER JOIN people
       ON psg.passport_number = people.passport_number

  WHERE psg.passport_number IN (5773159633, 3592750733, 7049073643, 8496433585, 1988161715)
  AND f.year = 2021
  AND f.month = 7
  AND f.day = 29;
/*
+----+-------------------+------------------------+------+--------+--------+----------------+-----------------+-----------+-----------------+
| id | origin_airport_id | destination_airport_id | hour | minute |  name  |  phone_number  | passport_number | flight_id | passport_number |
+----+-------------------+------------------------+------+--------+--------+----------------+-----------------+-----------+-----------------+
| 18 | 8                 | 6                      | 16   | 0      | Diana  | (770) 555-1861 | 3592750733      | 18        | 3592750733      |
| 36 | 8                 | 4                      | 8    | 20     | Bruce  | (367) 555-5533 | 5773159633      | 36        | 5773159633      |
| 36 | 8                 | 4                      | 8    | 20     | Taylor | (286) 555-6063 | 1988161715      | 36        | 1988161715      |
| 36 | 8                 | 4                      | 8    | 20     | Luca   | (389) 555-5198 | 8496433585      | 36        | 8496433585      |
+----+-------------------+------------------------+------+--------+--------+----------------+-----------------+-----------+-----------------+
*/

-- Find who called on 28 July 2021. Suspects - Taylor / Bruce
SELECT people.name, pc.caller, pc.receiver, pc.duration
  FROM phone_calls AS pc
       INNER JOIN people
       ON pc.caller = people.phone_number

 WHERE pc.year = 2021
   AND pc.month = 7
   AND pc.day = 28
   AND people.name IN ('Bruce', 'Luca', 'Taylor');

/*
+--------+----------------+----------------+----------+
|  name  |     caller     |    receiver    | duration |
+--------+----------------+----------------+----------+
| Bruce  | (367) 555-5533 | (375) 555-8161 | 45       |
| Bruce  | (367) 555-5533 | (344) 555-9601 | 120      |
| Bruce  | (367) 555-5533 | (022) 555-4052 | 241      |
| Taylor | (286) 555-6063 | (676) 555-6554 | 43       |
| Taylor | (286) 555-6063 | (310) 555-8568 | 235      |
| Bruce  | (367) 555-5533 | (704) 555-5790 | 75       |
+--------+----------------+----------------+----------+
*/
-- Find call recipient (accomplice) from calls < 100s.
SELECT name, phone_number
  FROM people
WHERE phone_number IN ('(375) 555-8161', '(704) 555-5790', '(676) 555-6554');
+-------+----------------+
| name  |  phone_number  |
+-------+----------------+
| James | (676) 555-6554 |
| Carl  | (704) 555-5790 |
| Robin | (375) 555-8161 |
+-------+----------------+

-- Find city thief escaped to
SELECT * FROM airports
 WHERE id IN (4, 8);
 /*
 +----+--------------+-----------------------------+---------------+
| id | abbreviation |          full_name          |     city      |
+----+--------------+-----------------------------+---------------+
| 4  | LGA          | LaGuardia Airport           | New York City |
| 8  | CSF          | Fiftyville Regional Airport | Fiftyville    |
+----+--------------+-----------------------------+---------------+
 */

