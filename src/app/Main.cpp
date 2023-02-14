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

#include "Config.h"
#include "Timer.h"
#include "Errors.h"
#include "Elevator.h"
#include "Log.h"
#include <csignal>
#include <thread>

void TerminateHandler(int sigval);
void ElevatorUpdateLoop();

/// Launch the server
int main()
{
    signal(SIGTERM, &TerminateHandler);
    signal(SIGINT, &TerminateHandler);
    signal(SIGABRT, &Warhead::AbortHandler);

    // Use only console logger
    sLog->UsingDefaultLogs();

    // Configure elevator
    sElevator->Start();

    // Start main loop
    ElevatorUpdateLoop();

    LOG_INFO("elevator", "Halting process...");

    // 0 - normal shutdown
    // 1 - shutdown at error
    return 0;
}

void ElevatorUpdateLoop()
{
    auto realCurrTime = 0ms;
    auto realPrevTime = GetTimeMS();

    while (!Elevator::IsStopped())
    {
        realCurrTime = GetTimeMS();

        auto diff = GetMSTimeDiff(realPrevTime, realCurrTime);
        if (diff == 0ms)
        {
            std::this_thread::sleep_for(1s);
            continue;
        }

        sElevator->Update();
        realPrevTime = realCurrTime;
    }

    LOG_INFO("elevator", "Stop update loop");
}

void TerminateHandler(int sigval)
{
    LOG_WARN("elevator", "Caught signal: {}. Stop process", sigval);
    sElevator->StopNow(SHUTDOWN_EXIT_CODE);
}
