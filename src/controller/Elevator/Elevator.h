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

#ifndef WARHEAD_ELEVATOR_H_
#define WARHEAD_ELEVATOR_H_

#include "Define.h"
#include "LockedQueue.h"
#include <atomic>
#include <functional>

// Exit code for main function
enum ShutdownExitCode : uint8
{
    SHUTDOWN_EXIT_CODE,
    ERROR_EXIT_CODE
};

// Elevator command
enum class MovementType : uint8
{
    Up,
    Down
};

// Object for passengers in elevator
struct ElevatorPassenger
{
    explicit ElevatorPassenger(uint8 floorNeed) :
        FloorNeed(floorNeed) { }

    uint8 FloorNeed{};
};

// Object for passengers on floors
struct FloorPassenger
{
    explicit FloorPassenger(uint8 currentFloor, uint8 floorNeed) :
        CurrentFloor(currentFloor), FloorNeed(floorNeed) { }

    uint8 CurrentFloor{};
    uint8 FloorNeed{};
};

class WH_CTRL_API Elevator
{
public:
    Elevator() = default;
    ~Elevator() = default;

    // Singleton
    static Elevator* instance();

    // Default start. Add random passengers on floors
    void Start();

    // Reset all queues
    void ResetAllPassengers();

    // Add passenger in _elevatorQueue
    void AddPassengerToElevator(uint8 floorNeed);

    // Add passenger in _floorQueue
    void AddPassenger(uint8 currentFloor, uint8 floorNeed);

    // Update elevator. Change current floor, movement, execute all queues
    void Update();

    // Get cancel token
    [[nodiscard]] static bool IsStopped() { return _cancel; }

    // Get exit code for main function
    static uint8 GetExitCode() { return _exitCode; }

    // Stop all works and set new exit code
    static void StopNow(uint8 exitcode) { _cancel = true; _exitCode = exitcode; }

    // Get next floor for elevator
    uint8 GetNextFloor();

    // Set current floor and movement type for elevator
    inline void SetCurrentFloor(uint8 floor, MovementType movementType) { _currentFloor = floor; _movementType = movementType; }

private:
    // Add random count passengers in _elevatorQueue
    void AddRandomPassengers(uint8 count = 5);

    // Add random count passengers in _floorQueue
    void AddRandomElevatorPassengers(uint8 count = 5);

    // Pop passengers from elevator (execute _elevatorQueue)
    void ProcessExitPassengers();

    // Emplace passenger in elevator (execute _floorQueue)
    void ProcessPopulatePassengers();

    // Get random number between 1 and 9 (min and max floors)
    static uint8 GetRandomNumber();

    // Current elevator floor
    uint8 _currentFloor{ 1 };

    // Current elevator command movement
    MovementType _movementType{};

    // Queue for passengers in elevator
    LockedQueue<ElevatorPassenger> _elevatorQueue;

    // Queue for passengers in floors
    LockedQueue<FloorPassenger> _floorQueue;

    // Cancel main loop operation
    static std::atomic<bool> _cancel;

    // Exit code for main function
    static uint8 _exitCode;
};

#define sElevator Elevator::instance()

#endif
