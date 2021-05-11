#pragma once

#include <cstdlib>

#include "../Config.h"
#include "ILog.h"

#define PB_ASSERT(condition)    do { ((condition) ? (void)0 : pb::ILog::Log(pb::LogLevel::Error, __FILE__, __LINE__, #condition) ); PB_DEBUG_BREAK(); } while(false)
//#define PB_ASSERT_MSG(condition, ...) ((condition) ? (void)0 : pb::ILog::Log(pb::LogLevel::Error, __FILE__, __LINE__, __VA_ARGS__) )
#define PB_ASSERT_MSG(condition, ...) ((condition) ? (void)0 : pb::ILog::TLog(pb::LogLevel::Error, __FILE__, __LINE__, __VA_ARGS__) )

#define PB_WARNING(condition)   ((condition) ? (void)0 : pb::ILog::Log(pb::LogLevel::Warning, #condition, __FILE__, __LINE__) )
#define PB_WARNING_MSG(condition, ...) ((condition) ? (void)0 : pb::ILog::TLog(pb::LogLevel::Warning, #condition, __FILE__, __LINE__, __VA_ARGS__) )

#define PB_TRACE(...)   do{ pb::ILog::TLog(pb::LogLevel::Debug, __FILE__, __LINE__, __VA_ARGS__); }while(0)

namespace pb {


}