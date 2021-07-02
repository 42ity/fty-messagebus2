/*  =========================================================================
    fty_common_messagebus_interface - class description

    Copyright (C) 2014 - 2020 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
*/

#pragma once

#include <chrono>
#include <ctime>

#include <string>
#include <uuid/uuid.h>

namespace fty::messagebus::utils
{
  inline const std::string generateUuid()
  {
    uuid_t id;
    uuid_generate(id);
    return (reinterpret_cast<const char*>(id));
  }

  inline const std::string getClientId(const std::string& prefix)
  {
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch());
    std::string clientId = prefix + "-" + std::to_string(ms.count());
    return clientId;
  }

} // namespace fty::messagebus::utils
