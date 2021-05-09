#pragma once

#include <cstdlib>

#include "ILog.h"

#define PB_ASSERT(condition)    ((condition) ? (void)0 : pb::ILog::Log(pb::LogLevel::Error, #condition, __FILE__, __LINE__) )
#define PB_ASSERT_MSG(condition, ...) ((condition) ? (void)0 : pb::ILog::Log(pb::LogLevel::Error, #condition, __FILE__, __LINE__, __VA_ARGS__) )

namespace pb {


}