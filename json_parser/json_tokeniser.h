#pragma once

#include <string>
#include <optional>

#include "json_value.h"

namespace twobits::json
{
	enum class token_type
	{
		ObjectStart,
		ObjectEnd,
		ArrayStart,
		ArrayEnd,
		String,
		Number,
		True,
		False,
		Colon,
		Comma,
		Null
	};

	class json_tokenizer
	{
	public:
		json_tokenizer(std::string_view text)
			: _text(text),
			_pos(0),
			_end(text.length())
		{
		}

		std::tuple<token_type, std::optional<json_value>> next_token();

		bool finished();
		bool is_whitespace();
		bool is_numeric();
		char current();

		std::string get_string();
		double get_number();

	private:
		std::string_view _text;
		size_t _pos;
		size_t _end;
	};
};
