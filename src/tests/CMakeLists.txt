#
# This file is part of the WarheadCore Project. See AUTHORS file for Copyright information
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU Affero General Public License as published by the
# Free Software Foundation; either version 3 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#

CollectSourceFiles(
  ${CMAKE_CURRENT_SOURCE_DIR}
  TEST_SOURCES)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(tests ${TEST_SOURCES})

target_link_libraries(tests
  PRIVATE
    warhead-core-interface
    controller
    Catch2::Catch2)

CollectIncludeDirectories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  TEST_INCLUDES)

target_include_directories(tests
  PUBLIC
    ${TEST_INCLUDES}
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR})

catch_discover_tests(tests)

set_target_properties(tests
  PROPERTIES
    FOLDER
      "tests")
