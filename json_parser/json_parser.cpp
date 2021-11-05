#pragma once

#include "json_value.h"
#include "json_parser.h"
#include "json_tokeniser.h"

using namespace std;
using namespace twobits::json;

json_value parse_array(json_tokenizer& tokeniser);
json_value parse_value(token_type type, std::optional<json_value> token, json_tokenizer& tokeniser);
json_object parse_object(json_tokenizer& tokeniser);

json_object twobits::json::parse_json(std::string_view json_text)
{
	json_tokenizer tok(json_text);

	auto [type, token] = tok.next_token();
	if (type != token_type::ObjectStart)
	{
		// error ?
	}

	return parse_object(tok);
}

json_object parse_object(json_tokenizer& tokeniser)
{
	json_object obj;

	while (!tokeniser.finished())
	{
		auto [type, token] = tokeniser.next_token();

		switch (type)
		{
			case token_type::ObjectEnd:
				return obj;

			default:
			{
				auto label = token.value().string_value();

				auto [type, token] = tokeniser.next_token();
				auto value = parse_value(type, token, tokeniser);

				obj.add(label, value);
			}
		}
	}

	return obj;
}

json_value parse_array(json_tokenizer& tokeniser)
{
	std::vector<json_value> values;

	while (!tokeniser.finished())
	{
		auto [type, token] = tokeniser.next_token();

		switch (type)
		{
			case token_type::ArrayEnd:
				return values;

			default:
			{
				auto value = parse_value(type, token, tokeniser);
				values.push_back(value);
			}
		}
	}

	return json_value(values);
}

json_value parse_value(token_type type, std::optional<json_value> token, json_tokenizer& tokeniser)
{
	switch (type)
	{
		case token_type::ObjectStart:
			return json_value(parse_object(tokeniser).as_value());

		case token_type::ArrayStart:
			return parse_array(tokeniser);

		case token_type::False:
			return json_value(false);

		case token_type::True:
			return json_value(true);

		case token_type::Number:
			return token.value();

		case token_type::String:
			return token.value();

		default:
		case token_type::Null:
			return json_value(json_value_type::Null);
	}
}
