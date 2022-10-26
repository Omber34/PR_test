//
// Created by Herman on 10/26/2022.
//

#pragma once

#include "ChatPacket.h"
#include "spdlog/logger.h"

namespace core
{
  class Logger
  {
  public:
    static void init(const std::string& loggerName);
    static void logPacket(const ChatPacket &packet);
    static void logEvent(const ChatEvent &event);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void critical(const std::string& message);

  private:
    static Logger& getInst();
    Logger() = default;

    Logger(const Logger &other) = default;

    Logger &operator=(const Logger &other) = default;

    Logger(Logger&& other) = default;

    Logger &operator=(Logger &&other) = default;
    std::shared_ptr<spdlog::logger> logger;

  };

}
