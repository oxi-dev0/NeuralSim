#include "Log.h"

namespace Debug {

	std::shared_ptr<spdlog::logger> Log::s_Logger;

	void Log::Init()
	{
		// "{Colour Range}[{Timestamp}]: {Text}{End Colour Range}
		spdlog::set_pattern("%^[%T]: %v%$");
		s_Logger = spdlog::stdout_color_mt("ENGINE");

		// Print all messages
		s_Logger->set_level(spdlog::level::level_enum::trace);
	}
}