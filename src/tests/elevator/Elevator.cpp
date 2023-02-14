/*
* This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU Affero General Public License as published by the
* Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "catch2/catch.hpp"
#include "Elevator.h"

TEST_CASE("Get next floor with default passengers")
{
    SECTION("Step 1")
    {
        sElevator->ResetAllPassengers();
        sElevator->AddPassenger(4, 2);
        sElevator->AddPassenger(5, 1);
        REQUIRE(sElevator->GetNextFloor() == 4);
    }

    SECTION("Step 2")
    {
        sElevator->ResetAllPassengers();
        sElevator->AddPassenger(3, 9);
        sElevator->AddPassenger(5, 1);
        REQUIRE(sElevator->GetNextFloor() == 3);
    }

    SECTION("Step 3")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(1, MovementType::Up);
        sElevator->AddPassenger(8, 2);
        sElevator->AddPassenger(6, 2);

        REQUIRE(sElevator->GetNextFloor() == 6);
    }

    SECTION("Step 4")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(1, MovementType::Down);
        sElevator->AddPassenger(8, 2);
        sElevator->AddPassenger(6, 2);

        REQUIRE(sElevator->GetNextFloor() == 6);
    }

    SECTION("Step 5")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(9, MovementType::Down);
        sElevator->AddPassenger(6, 2);
        sElevator->AddPassenger(8, 2);
        sElevator->AddPassenger(7, 3);

        REQUIRE(sElevator->GetNextFloor() == 8);
    }

    SECTION("Step 6")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(9, MovementType::Up);

        sElevator->AddPassenger(6, 2);
        sElevator->AddPassenger(8, 2);
        sElevator->AddPassenger(1, 5);

        REQUIRE(sElevator->GetNextFloor() == 8);
    }

    SECTION("Step 7")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(5, MovementType::Up);
        sElevator->AddPassenger(3, 1);
        sElevator->AddPassenger(7, 9);

        REQUIRE(sElevator->GetNextFloor() == 7);
    }

    SECTION("Step 8")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(5, MovementType::Down);
        sElevator->AddPassenger(3, 1);
        sElevator->AddPassenger(7, 9);

        REQUIRE(sElevator->GetNextFloor() == 3);
    }
}

TEST_CASE("Get next floor with elevator passengers")
{
    SECTION("Step 1")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(1, MovementType::Up);
        sElevator->AddPassengerToElevator(9);
        sElevator->AddPassengerToElevator(5);
        REQUIRE(sElevator->GetNextFloor() == 5);
    }

    SECTION("Step 2")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(9, MovementType::Down);
        sElevator->AddPassengerToElevator(5);
        sElevator->AddPassengerToElevator(7);
        REQUIRE(sElevator->GetNextFloor() == 7);
    }

    SECTION("Step 3")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(9, MovementType::Up);
        sElevator->AddPassengerToElevator(5);
        sElevator->AddPassengerToElevator(7);
        REQUIRE(sElevator->GetNextFloor() == 7);
    }

    SECTION("Step 4")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(1, MovementType::Up);
        sElevator->AddPassengerToElevator(5);
        sElevator->AddPassengerToElevator(3);
        REQUIRE(sElevator->GetNextFloor() == 3);
    }

    SECTION("Step 5")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(1, MovementType::Down);
        sElevator->AddPassengerToElevator(5);
        sElevator->AddPassengerToElevator(3);
        REQUIRE(sElevator->GetNextFloor() == 3);
    }

    SECTION("Step 6")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(5, MovementType::Up);
        sElevator->AddPassengerToElevator(3);
        sElevator->AddPassengerToElevator(7);
        REQUIRE(sElevator->GetNextFloor() == 7);
    }

    SECTION("Step 7")
    {
        sElevator->ResetAllPassengers();
        sElevator->SetCurrentFloor(5, MovementType::Down);
        sElevator->AddPassengerToElevator(3);
        sElevator->AddPassengerToElevator(7);
        REQUIRE(sElevator->GetNextFloor() == 3);
    }
}
