/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WARHEAD_LOCKED_QUEUE_H_
#define WARHEAD_LOCKED_QUEUE_H_

#include <deque>
#include <mutex>

template <class T>
class LockedQueue
{
    //! Storage type
    using StorageType = std::deque<T*>;

    //! Storage iterator
    using StorageIterator = typename std::deque<T*>::iterator;

    //! Lock access to the queue.
    std::mutex _lock;

    //! Storage backing the queue.
    StorageType _queue;

    //! Cancellation flag.
    volatile bool _canceled{};

public:
    //! Create a LockedQueue.
    LockedQueue() = default;

    //! Destroy a LockedQueue.
    ~LockedQueue()
    {
        Reset();
    }

    //! Adds an item to the queue.
    void Add(T* item)
    {
        std::lock_guard lock(_lock);
        _queue.emplace_back(item);
    }

    //! Adds items back to front of the queue
    template<class Container>
    void ReadContainer(Container& container)
    {
        std::lock_guard<std::mutex> lock(_lock);
        _queue.insert(_queue.begin(), std::begin(container), std::end(container));
    }

    //! Gets the next result in the queue, if any.
    bool GetNext(T*& result)
    {
        std::lock_guard<std::mutex> lock(_lock);

        if (_queue.empty())
            return false;

        result = _queue.front();
        _queue.pop_front();
        return true;
    }

    //! Cancels the queue.
    void Cancel()
    {
        std::lock_guard<std::mutex> lock(_lock);
        _canceled = true;
    }

    //! Checks if the queue is cancelled.
    bool Cancelled()
    {
        std::lock_guard<std::mutex> lock(_lock);
        return _canceled;
    }

    ///! Calls pop_front of the queue
    void PopFront()
    {
        std::lock_guard lock(_lock);
        _queue.pop_front();
    }

    ///! Checks if we're empty or not with locks held
    bool Empty()
    {
        std::lock_guard lock(_lock);
        return _queue.empty();
    }

    std::size_t GetSize()
    {
        std::lock_guard lock(_lock);
        return _queue.size();
    }

    StorageIterator begin()
    {
        std::lock_guard lock(_lock);
        return _queue.begin();
    }

    StorageIterator end()
    {
        std::lock_guard lock(_lock);
        return _queue.end();
    }

    void Reset()
    {
        T* element{ nullptr };
        while (GetNext(element))
            delete element;
    }
};

#endif
