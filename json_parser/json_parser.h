#pragma once

#include <string>
#include "json_value.h"


json::json_object parse_json(std::string_view json_text);

