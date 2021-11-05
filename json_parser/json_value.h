#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

namespace twobits::json
{
	enum class json_value_type
	{
		Null,
		Number,
		Boolean,
		String,
		Array,
		Object
	};

	class json_value
	{
	public:
		json_value()
			: _type(json_value_type::Null) {};

		json_value(const json_value& other) = default;

		json_value(json_value_type vt)
			: _type(vt) {}

		json_value(double value)
			: _type(json_value_type::Number), _value(value) {}

		json_value(std::string value)
			: _type(json_value_type::String), _value(value) {}

		json_value(bool value)
			: _type(json_value_type::Boolean), _value(value) {}

		json_value(std::unordered_map<std::string, json_value> value)
			: _type(json_value_type::Object), _value(value) {}

		json_value(std::vector<json_value> values)
			: _type(json_value_type::Array), _value(values) {}

		json_value_type type() { return _type; };

		std::string string_value()	const { return std::get<std::string>(_value); }
		operator std::string() { return string_value(); };

		double number_value() { return std::get<double>(_value); }
		operator double() { return number_value(); }

		bool boolean_value() { return std::get<bool>(_value); }
		operator bool() { return boolean_value(); }

		std::unordered_map<std::string, json_value> object_value() { return std::get<std::unordered_map<std::string, json_value>>(_value); }

		std::vector<json_value> array_value() { return std::get<std::vector<json_value>>(_value); }

	private:
		json_value_type _type;
		std::variant<double, std::string, bool, std::unordered_map<std::string, json_value>, std::vector<json_value>> _value;
	};

	class json_object
	{
	public:
		json_object() = default;
		json_object(const json_object& other) = default;
		json_object(std::unordered_map<std::string, json_value>& values)
			: _values(values)
		{
		}

		void add(std::string name, json_value value)
		{
			_values.insert({ name, value });
		}

		json_value as_value()
		{
			return json_value(_values);
		}

		std::tuple<bool, json_value> item(std::string name)
		{
			auto result = _values.find(name);
			if (result != _values.end())
				return { true, result->second };

			return { false, json_value() };
		}

		json_value operator[](std::string name)
		{
			auto result = _values.find(name);
			if (result != _values.end())
				return result->second;

			throw std::exception("not found");
		}

	private:
		std::unordered_map<std::string, json_value> _values;
	};
};