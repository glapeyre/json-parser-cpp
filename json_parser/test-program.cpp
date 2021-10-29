// json_parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "json_value.h"
#include "json_parser.h"

int main()
{
    std::string test = "{ 'number':3.14159, 'string': 'hello world', 'boolean': false, 'array':[ 123, 456, 789, { 'embedded object': 'boo!' } ] }";

    auto obj = parse_json(test);

    printf("number= %f\n", (double)obj["number"]);

    auto [found, flag] = obj.item("boolean");
    printf("flag= %d\n", (bool)flag);

    auto ar = obj["array"].array_value();
    for (auto& item : ar)
    {
        switch (item.type())
        {
            case json::json_value_type::String:
                std::cout << (std::string)item;
                break;

            case json::json_value_type::Number:
                std::cout << (double)item << ", ";
                break;

            case json::json_value_type::Object:
            {
                auto ob = item.object_value();
                for (auto& [k,v] : ob)
                    std::cout << k << " = " << (std::string)v << ", ";
            }
            break;
        }
    }

    try
    {
        auto nope = obj["not found"];
    }
    catch (std::exception& ex)
    {
        std::cout << "Ooops! " << ex.what() << std::endl;
    }

    return 0;
}
