/*  =========================================================================
    fty_common_messagebus_mqtt_example - description

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

/*
@header
    fty_common_messagebus_mqtt_example -
@discuss
@end
*/

#include <FtyCommonMessageBusDto.hpp>
#include <FtyCommonMqttTestDef.hpp>
#include <fty/messagebus/MsgBusException.hpp>
#include <fty/messagebus/MsgBusFactory.hpp>
#include <fty/messagebus/mqtt/MsgBusMqtt.hpp>
#include <fty/messagebus/utils/MsgBusHelper.hpp>

#include <chrono>
#include <csignal>
#include <fty_log.h>
#include <iostream>
#include <thread>

namespace
{
  using namespace fty::messagebus;
  using namespace fty::messagebus::mqttv5;
  using namespace fty::messagebus::mqttv5::test;
  using Message = fty::messagebus::mqttv5::MqttMessage;
  using MessageBus = fty::messagebus::IMessageBus<Message>;

  static bool _continue = true;

  static void signalHandler(int signal)
  {
    std::cout << "Signal " << signal << " received\n";
    _continue = false;
  }
} // namespace

int main(int /*argc*/, char** argv)
{
  log_info("%s - starting...", argv[0]);

  // Install a signal handler
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  auto mqttClient = MessageBusFactory::createMqttMsgBus(DEFAULT_MQTT_END_POINT, "mqtt-discovery-client");
  mqttClient->connect();

  while (_continue)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  delete mqttClient;

  log_info("%s - end", argv[0]);
  return EXIT_SUCCESS;
}