//
// Created by Herman on 10/26/2022.
//

#include "Logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"
#include "AppEvent.h"
#include "CoreUtility.h"

constexpr const auto logfileMask = "logs/{0}.log";
constexpr const auto logfileSize = 1048576 * 5;
constexpr const auto logfilesCount = 3;

void core::Logger::logPacket(const core::ChatPacket &packet)
{
  const constexpr auto logPacketMask = "id:{0}, seq:{1}, size:{2}, payload '{3}'";
  const auto logger = getInst().logger;
  if (logger)
    logger->info(logPacketMask, packet.event_id(), packet.sequence_index(), packet.body_length(), reinterpret_cast<const char*>(packet.body()));

  spdlog::info(logPacketMask, packet.event_id(), packet.sequence_index(), packet.body_length(), reinterpret_cast<const char*>(packet.body()));
}

void core::Logger::logEvent(const core::ChatEvent &event)
{
  const constexpr auto logEventMask = "{0} : {1} : {2}";
  const auto logger = getInst().logger;
  if (logger)
    logger->info(logEventMask, event.user.toStdString(), enumValueToString(event.type), event.message.message.toStdString());

  spdlog::info(logEventMask, event.user.toStdString(), enumValueToString(event.type), event.message.message.toStdString());
}

core::Logger &core::Logger::getInst()
{
  static Logger instance;
  return instance;
}

void core::Logger::init(const std::string &loggerName)
{
  const auto filename = fmt::format(logfileMask, loggerName);
  getInst().logger = spdlog::rotating_logger_mt(loggerName, filename, logfileSize, logfilesCount);
}

void core::Logger::info(const std::string &message)
{
  const auto logger = getInst().logger;
  if (logger)
    logger->info(message);

  spdlog::info(message);
}

void core::Logger::warning(const std::string &message)
{
  const auto logger = getInst().logger;
  if (logger)
    logger->warn(message);

  spdlog::warn(message);
}

void core::Logger::critical(const std::string &message)
{
  const auto logger = getInst().logger;
  if (logger)
    logger->critical(message);

  spdlog::critical(message);
}


