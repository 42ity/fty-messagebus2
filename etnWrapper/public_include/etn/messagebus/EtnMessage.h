/*  =========================================================================
    Copyright (C) 2014 - 2021 Eaton

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

#include <fty/messagebus/Message.h>

namespace etn::messagebus
{
  enum class AddressType
  {
    TOPIC,
    QUEUE,
    REQUEST_QUEUE,
    REPLY_QUEUE
  };

  enum class BusType
  {
    AMQP,
    MQTT
  };

  class EtnMessage
  {
  public:
    static fty::messagebus::Address buildAddress(const fty::messagebus::Address& address, const AddressType& addressType = AddressType::TOPIC);
    static BusType getBusType(const fty::messagebus::Address& address);
  };

} // namespace etn::messagebus
