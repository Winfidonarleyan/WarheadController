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

// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "FileUtil.h"
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void Warhead::File::CorrectDirPath(std::string& path, bool makeAbsolute /*= false*/)
{
    if (path.empty())
        path = fs::absolute(fs::current_path()).generic_string();
    else
        std::replace(std::begin(path), std::end(path), '\\', '/');

    if (path.at(path.length() - 1) != '/')
        path.push_back('/');

    if (makeAbsolute)
        path = fs::absolute(path).generic_string();
}

bool Warhead::File::CreateDirIfNeed(std::string_view path)
{
    if (path.empty())
        return true;

    fs::path dirPath{ path };

    if (fs::exists(dirPath) && fs::is_directory(path))
        return true;

    try
    {
        return fs::create_directory(dirPath);
    }
    catch (...)
    {
        return false;
    }
}

std::size_t Warhead::File::FindWord(std::string_view path, std::string_view findWord)
{
    std::ifstream in(path.data(), std::ios::in | std::ios::binary);
    if (in.fail())
        return 0;

    std::size_t lineNumber{};
    std::string line;
    bool isFound{};

    while (in.good())
    {
        lineNumber++;
        std::getline(in, line);

        if (!in.good() && !in.eof())
            break;

        if (line.empty())
            continue;

        std::size_t found = line.find(findWord);
        if (found != std::string::npos)
        {
            isFound = true;
            break;
        }
    }

    in.close();
    return isFound ? lineNumber : 0;
}

bool Warhead::File::CopyFile(std::string_view pathFrom, std::string_view pathTo)
{
    try
    {
        return fs::copy_file(pathFrom, pathTo, fs::copy_options::update_existing);
    }
    catch (...) { }

    return false;
}
