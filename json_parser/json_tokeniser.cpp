#include <string>
#include <optional>

#include "json_value.h"
#include "json_tokeniser.h"

using namespace std;
using namespace json;

std::tuple<token_type, std::optional<json_value>> json_tokenizer::next_token()
{
	while (!finished() && is_whitespace())
		_pos++;

	if (finished())
	{
		// unexpected? possible error
		return { token_type::Null, nullopt };
	}

	switch (current())
	{
	case '{': _pos++; return { token_type::ObjectStart, nullopt };
	case '}': _pos++; return { token_type::ObjectEnd, nullopt };
	case '[': _pos++; return { token_type::ArrayStart, nullopt };
	case ']': _pos++; return { token_type::ArrayEnd, nullopt };

	case '\"':
	case '\'':
		{
			auto str = get_string();
			return { token_type::String, json_value(str) };
		}

	case 'f':
	case 'F': _pos += 5;  return { token_type::False, nullopt };

	case 't':
	case 'T': _pos += 4;  return { token_type::True, nullopt };

	case 'n':
	case 'N': _pos += 4;  return { token_type::Null, nullopt };
	}

	if (is_numeric())
	{
		auto nbr = get_number();
		return { token_type::Number, nbr };
	}

	// shouldn't get here
	return { token_type::Null, nullopt };
}

bool json_tokenizer::finished()
{
	return _pos >= _end;
}

bool json_tokenizer::is_whitespace()
{
	return isblank(_text[_pos])
		|| isspace(_text[_pos])
		|| _text[_pos] == ':'
		|| _text[_pos] == ',';
}

bool json_tokenizer::is_numeric()
{
	return isdigit(_text[_pos])
		|| _text[_pos] == '+'
		|| _text[_pos] == '-'
		|| _text[_pos] == '.';
}

char json_tokenizer::current()
{
	return _text[_pos];
}

std::string json_tokenizer::get_string()
{
	auto start = _pos+1;
	auto delim = current();
	
	_pos++;
	while (!finished() && current() != delim)
	{
		_pos++;
		if (current() == '\\')
			// skip escaped chars
			_pos++;
	}
	auto end = _pos;

	_pos++;
	return std::string{ _text.substr(start, end - start) };
}

double json_tokenizer::get_number()
{
	char* end;
	auto value = std::strtod(&_text[_pos], &end);
	_pos += end - &_text[_pos];
	return value;
}
