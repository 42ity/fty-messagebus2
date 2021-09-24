/*  =========================================================================
    MsgBusAmqp.cpp - class description

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

#include "MsgBusAmqp.h"
#include "MsgBusAmqpUtils.h"

#include <fty/messagebus/utils.h>
#include <fty_log.h>

namespace
{
  // TODO implement it
  bool isServiceAvailable()
  {
    return true;
  }
} // namespace

namespace fty::messagebus::amqp
{
  static auto constexpr AMQP_TOPIC_PREFIX = "topic://";
  static auto constexpr AMQP_QUEUE_PREFIX = "queue://";

  using namespace fty::messagebus;

  using proton::receiver_options;
  using proton::source_options;

  MsgBusAmqp::~MsgBusAmqp()
  {
    // Cleaning amqp ressources
    if (isServiceAvailable())
    {
      logDebug("Cleaning Amqp ressources for: {}", m_clientName);
      try
      {
        for (const auto& [key, receiv] : m_subScriptions)
        {
          logDebug("Cleaning: {}...", key);
          //receiv.cancel();
        }
        logDebug("{} cleaned", m_clientName);
      }
      catch (const std::exception& e)
      {
        logError("Exception: {}", e.what());
      }
    }
  }

  fty::Expected<void> MsgBusAmqp::connect()
  {

    logDebug("Connecting to {} ...", m_endpoint);
    try
    {
    }
    catch (const std::exception& e)
    {
      logError("unexpected error: {}", e.what());
      return fty::unexpected(COM_STATE_CONNECT_FAILED);
    }

    return {};
  }

  fty::Expected<void> MsgBusAmqp::unreceive(const std::string& address)
  {
    if (!isServiceAvailable())
    {
      logDebug("Service not available");
      return fty::unexpected(DELIVERY_STATE_UNAVAILABLE);
    }

    try
    {
      m_subScriptions.at(address)->cancel();
      logTrace("{} - unsubscribed on: '{}'", m_clientName, address);
      return {};
    }
    catch (...)
    {
      logDebug("Unsubscribed ({})", DELIVERY_STATE_REJECTED);
      return fty::unexpected(DELIVERY_STATE_REJECTED);
    }
  }

  fty::Expected<void> MsgBusAmqp::receive(const std::string& address, MessageListener messageListener, const std::string& filter)
  {
    if (!isServiceAvailable())
    {
      logDebug("Service not available");
      return fty::unexpected(DELIVERY_STATE_UNAVAILABLE);
    }

    logDebug("Waiting to receive msg from: {}", address);

    ReceiverPointer receiver = std::make_shared<Receiver>(m_endpoint, address, messageListener, filter);
    std::thread thrd([=]() {
      proton::container(*receiver).run();
    });
    m_subScriptions.emplace(std::make_pair(address, receiver));
    thrd.detach();

    logDebug("Waiting to receive msg from: {} Accepted", address);
    return {};
  }

  fty::Expected<void> MsgBusAmqp::receive(const std::string& address, MessageListener messageListener)
  {
    return receive(address, messageListener, {});
  }

  fty::Expected<void> MsgBusAmqp::send(const std::string& address, const Message& message)
  {
    if (!isServiceAvailable())
    {
      logDebug("Service not available");
      return fty::unexpected(DELIVERY_STATE_UNAVAILABLE);
    }

    logDebug("Sending to {}", address);
    proton::message msgToSend = getAmqpMessage(message);
    //logDebug("Sending request payload: '{}' to: {} and wait message on reply queue {}", message.userData(), requestQueue, replyTo);

    Sender sender = Sender(m_endpoint, address);
    std::thread thrd([&]() {
      proton::container(sender).run();
    });
    sender.sendMsg(msgToSend);
    thrd.join();

    if (false)
    {
      logDebug("Message sent ({})", DELIVERY_STATE_REJECTED);
      return fty::unexpected(DELIVERY_STATE_REJECTED);
    }

    logDebug("Message sent (Accepted)");
    return {};
  }

  fty::Expected<Message> MsgBusAmqp::request(const Message& message, int receiveTimeOut)
  {
    try
    {
      if (!isServiceAvailable())
      {
        logDebug("Service not available");
        return fty::unexpected(DELIVERY_STATE_UNAVAILABLE);
      }

      auto protonMsg = getAmqpMessage(message);

      Requester requester(m_endpoint, protonMsg);
      std::thread thrd([&]() {
        proton::container(requester).run();
      });
      thrd.detach();

      MessagePointer response = std::make_shared<proton::message>();
      bool messageArrived = requester.tryConsumeMessageFor(response, receiveTimeOut);
      if (!messageArrived)
      {
        logDebug("No message arrive in time!");
        return fty::unexpected(DELIVERY_STATE_TIMEOUT);
      }

      logDebug("Message arrived ({})", proton::to_string(*response));
      return Message{getMetaData(*response), response->body().empty() ? std::string{} : proton::to_string(response->body())};
    }
    catch (std::exception& e)
    {
      return fty::unexpected(e.what());
    }
  }

} // namespace fty::messagebus::amqp