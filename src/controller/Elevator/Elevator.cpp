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

#include "Elevator.h"
#include "Log.h"
#include <vector>
#include <random>

constexpr auto FLOOR_COUNT_MAX = 9;
constexpr auto FLOOR_COUNT_MIN = 1;

WH_CTRL_API std::atomic<bool> Elevator::_cancel{ false };
WH_CTRL_API uint8 Elevator::_exitCode = SHUTDOWN_EXIT_CODE;

/*static*/ Elevator* Elevator::instance()
{
    static Elevator instance;
    return &instance;
}

void Elevator::Start()
{
    std::srand(std::time(nullptr)); //use current time as seed for random generator

    AddRandomPassengers(5);
}

void Elevator::ResetAllPassengers()
{
    if (!_elevatorQueue.Empty())
        _elevatorQueue.Reset();

    if (!_floorQueue.Empty())
        _floorQueue.Reset();
}

void Elevator::AddPassengerToElevator(uint8 floorNeed)
{
    _elevatorQueue.Add(new ElevatorPassenger(floorNeed));
}

void Elevator::AddPassenger(uint8 currentFloor, uint8 floorNeed)
{
    _floorQueue.Add(new FloorPassenger(currentFloor, floorNeed));
}

void Elevator::Update()
{
    // Pop passengers from elevator
    ProcessExitPassengers();

    // Emplace passenger in elevator
    ProcessPopulatePassengers();

    // if all queues empty - no passenger. Skip next steps and stop elevator
    if (_elevatorQueue.Empty() && _floorQueue.Empty())
    {
        LOG_WARN("elevator", "Not found any passengers. Stay elevator in floor: {}", _currentFloor);
        LOG_INFO("elevator", "");

//        AddRandomPassengers(2);
//        AddRandomElevatorPassengers(2);
        return;
    }

    LOG_INFO("elevator", "Elevator info: Movement: {}. Current floor: {}", _movementType == MovementType::Up ? "Up" : "Down", _currentFloor);
    LOG_INFO("elevator", "");

    // Try to get next floor for elevator
    auto nextFloor = GetNextFloor();

    // Change movement type if need
    if (_movementType == MovementType::Up && nextFloor < _currentFloor)
        _movementType = MovementType::Down;
    else if (_movementType == MovementType::Down && nextFloor > _currentFloor)
        _movementType = MovementType::Up;

    // Set new current floor
    _currentFloor = nextFloor;
}

void Elevator::ProcessExitPassengers()
{
    if (_elevatorQueue.Empty())
        return;

    std::vector<ElevatorPassenger*> requeue;
    ElevatorPassenger* passenger{ nullptr };
    std::size_t exitCount{};

    while (_elevatorQueue.GetNext(passenger))
    {
        // Check current floor and remove passenger if need
        if (passenger->FloorNeed == _currentFloor)
        {
            LOG_DEBUG("elevator", "Passenger exit in floor: {}", _currentFloor);
            delete passenger;
            exitCount++;
            continue;
        }

        requeue.emplace_back(passenger);
    }

    // Re add passengers in queue if need
    if (!requeue.empty())
        _elevatorQueue.ReadContainer(requeue);

    if (exitCount)
        LOG_DEBUG("elevator", "Exit count: {}", exitCount);
}

void Elevator::ProcessPopulatePassengers()
{
    if (_floorQueue.Empty())
        return;

    std::vector<FloorPassenger*> requeue;
    FloorPassenger* passenger{ nullptr };
    std::size_t enterCount{};

    while (_floorQueue.GetNext(passenger))
    {
        // Check current floor and remove passenger if need
        if (passenger->CurrentFloor == _currentFloor)
        {
            LOG_DEBUG("elevator", "Add new elevator passenger. Floor need: {}", passenger->FloorNeed);

            AddPassengerToElevator(passenger->FloorNeed);
            enterCount++;
            delete passenger;
            continue;
        }

        requeue.emplace_back(passenger);
    }

    // Re add passengers in queue if need
    if (!requeue.empty())
        _floorQueue.ReadContainer(requeue);

    if (enterCount)
        LOG_DEBUG("elevator", "Enter count: {}", enterCount);
}

uint8 Elevator::GetNextFloor()
{
    uint8 nextFloorUp{ FLOOR_COUNT_MAX };
    uint8 nextFloorDown{ FLOOR_COUNT_MIN };

    for (auto passenger : _elevatorQueue)
    {
        if (nextFloorUp > passenger->FloorNeed && _currentFloor < passenger->FloorNeed)
            nextFloorUp = passenger->FloorNeed;
        else if (nextFloorDown < passenger->FloorNeed && _currentFloor > passenger->FloorNeed)
            nextFloorDown = passenger->FloorNeed;
    }

    for (auto passenger : _floorQueue)
    {
        if (nextFloorUp > passenger->CurrentFloor && _currentFloor < passenger->CurrentFloor)
            nextFloorUp = passenger->CurrentFloor;
        else if (nextFloorDown < passenger->CurrentFloor && _currentFloor > passenger->CurrentFloor)
            nextFloorDown = passenger->CurrentFloor;
    }

    // Check max floor
    if (_movementType == MovementType::Up && _currentFloor == FLOOR_COUNT_MAX)
        return nextFloorDown;

    // Check min floor
    if (_movementType == MovementType::Down && _currentFloor == FLOOR_COUNT_MIN)
        return nextFloorUp;

    if (_movementType == MovementType::Up && nextFloorUp > _currentFloor)
        return nextFloorUp;

    return nextFloorDown;
}

uint8 Elevator::GetRandomNumber()
{
    // Random engine
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<> distribution(1, 9);

    return distribution(generator);
}

void Elevator::AddRandomPassengers(uint8 count /*= 5*/)
{
    for (uint8 i{}; i < count; i++)
        AddPassenger(GetRandomNumber(), GetRandomNumber());
}

void Elevator::AddRandomElevatorPassengers(uint8 count /*= 5*/)
{
    for (uint8 i{}; i < count; i++)
        AddPassengerToElevator(GetRandomNumber());
}
