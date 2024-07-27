#pragma once
#include <vector>
#include <string>
#include <map>
#include <variant>
#include <boost/multiprecision/cpp_dec_float.hpp>

using flt = boost::multiprecision::cpp_dec_float_100;

namespace frz
{
    enum class Token
    {
        STORE_NOREF,
        STORE,
        DEBUG,
        LOAD,
        INPUT,
        STDOUT,
        STDOUT_ERR,
        DEF,
        PARAM,
        ENDDEF,
        PRINT,
        CALL,
        ENDCALL,
        USE,
        INCLUDE,
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        ADD_STRING,
        SUBSTR,
        OR,
        AND,
        EQ,
        NEQ,
        GT,
        LT,
        TRUE,
        FALSE,
        IF_TRUE,
        IF_FALSE,
        ENDIF,
        WHILE,
        ENDWHILE,
        STRING,
        NUMBER
    };

    struct env
    {
        std::vector<std::variant<Token, std::string, flt>> stack;
        std::map<std::string, std::variant<Token, std::string, flt>> vars;
        std::map<std::string, std::vector<std::variant<Token, std::string, flt>>> funcs;
    };
}