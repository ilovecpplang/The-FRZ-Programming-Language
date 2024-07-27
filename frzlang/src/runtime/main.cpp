#include "frzo.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <cassert>
#include <regex>

[[noreturn]]
void inputerr(unsigned int line, std::string place)
{
    std::cerr << "Invalid input: line-" << line << " " << place << "\n";
    std::exit(EXIT_FAILURE);
}

void fdebug(std::vector<std::string>& stack, std::variant<frz::Token, std::string, flt> e, unsigned int ln)
{
    if (std::holds_alternative<frz::Token>(e))
    {
        frz::Token token = std::get<frz::Token>(e);
        if (token == frz::Token::TRUE)
        {
            stack.push_back("TRUE");
        }
        else if (token == frz::Token::FALSE)
        {
            stack.push_back("FALSE");
        }
        else
        {
            inputerr(ln, "DEBUG");
        }
    }
    else if (std::holds_alternative<flt>(e))
    {
        stack.push_back(std::get<flt>(e).str());
    }
    else if (std::holds_alternative<std::string>(e))
    {
        stack.push_back(std::get<std::string>(e));
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Invalid command argument.\n";
        return EXIT_FAILURE;
    }

    std::ifstream fin(argv[1]);
    if (!fin)
    {
        std::cerr << "Could not open the file.\n";
        return EXIT_FAILURE;
    }

    std::regex str("\".*\"");
    std::regex num("[0-9]+");

    std::string line;
    unsigned int ln = 0;
    frz::env env;
    while (std::getline(fin, line))
    {
        ln++;
        if (line == "STORE-NOREF")
        {
            auto name = env.stack.at(env.stack.size()-1);
            if (std::holds_alternative<std::string>(name))
            {
                env.vars.insert(std::make_pair(std::get<std::string>(name), ""));
            }
            else
            {
                inputerr(ln, "STORE-NOREF");
            }
        }
        else if (line == "STORE")
        {
            auto name = env.stack.at(env.stack.size()-1), value = env.stack.at(env.stack.size()-2);
            if (std::holds_alternative<std::string>(name))
            {
                std::visit([&](const auto& v){
                    env.vars.insert(std::make_pair(std::get<std::string>(name), v));
                }, value);
            }
            else
            {
                inputerr(ln, "STORE");
            }
        }
        else if (line == "DEBUG")
        {
            std::vector<std::string> stack, vars;
            for (auto e : env.stack)
            {
                fdebug(stack, e, ln);
            }
            std::cout << "[DEBUG] STACK ";
            for (auto e : stack)
            {
                std::cout << e << " ";
            }
            std::cout << std::endl;
        }
        else if (std::regex_match(line, str))
        {
            env.stack.push_back(line.substr(1, line.length()-2));
        }
        else if (std::regex_match(line, num))
        {
            env.stack.push_back(flt(line));
        }
        else
        {
            inputerr(ln, line + " (Not found such a command or value type)");
        }
    }
}