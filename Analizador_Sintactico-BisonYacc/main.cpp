#include<iostream>
#include<fstream>
#include<string>
#include<string_view>
#include<algorithm>

#include "parser.tab.hpp"

bool es_Numero(std::string_view str)
{
    if (str.empty())
       return false;

    bool puntoActivo = false;
    for (char c : str )
    {
       if ( !(std::isdigit(c) || (c == '.' && !puntoActivo) ) )
          return false;
       puntoActivo = puntoActivo || (c == '.');
    }

    return true;
}

bool es_OpeComp(std::string_view str){
    const std::vector<std::string> operadores{"<", ">", "<=", ">=", "=="};
    for(const auto& op : operadores)
        if (op == str){ 
            return true; 
        }

    return false;
}

bool es_Tipo(std::string_view str){
    const std::vector<std::string> tipos{"int", "float", "double", "long", "short", "char", 
                                            "string", "bool", "const", "short", "const"};
    for(const auto& tipo : tipos)
        if (tipo == str){
            return true;
        }

    return false;
}

bool es_Keyword(std::string_view str){
    const std::vector<std::string> keywords{"return", "if", "else", "do", "while", "for", "switch", "break", "continue", 
                                            "case", "default", "class", "true", "false", "new", "delete", "public", 
                                            "protected", "private", "this", "throw", "try", "catch", "void", "typedef",
                                            "typeid", "typename", "using", "union", "extern", "namespace", "volatile",
                                            };
    for(const auto& keyword : keywords)
        if (keyword == str){
            return true;
        }

    return false;
}

std::ifstream file;

yy::parser::symbol_type obtener_siguiente_token(){
    std::string s;
    char c;
    while (true) {
        
        double numero;
        if (!file.eof()) {
            file.get(c);
        }else{
            return yy::parser::make_YYEOF();
            // return "";
        }
        if (c == ' ' || c == '\n' || file.eof()){
            if (s.empty()) {  
                if (file.eof()){
                    return yy::parser::make_YYEOF();
                    // return "";
                }else{
                    return obtener_siguiente_token();
                }
            }else{
                if (!file.eof()){
                    file.unget();
                }
                if (es_Tipo(s)) {
                    return yy::parser::make_TIPO();
                }
                if (es_Keyword(s)) {
                    return yy::parser::make_KEYWORD();
                    // std::cout << "Retorna ";
                    // return s;
                }
                if (s == "pow") {
                    return yy::parser::make_POW_CMATH();
                }
                if (s == "sqrt") {
                    return yy::parser::make_SQRT_CMATH();
                }
                if (s == "sin") {
                    return yy::parser::make_SIN_CMATH();
                }
                if (s == "cos") {
                    return yy::parser::make_COS_CMATH();
                }
                if (s == "log") {
                    return yy::parser::make_LOG_CMATH();
                }
                if (s == "log10") {
                    return yy::parser::make_LOG10_CMATH();
                }
                if (s == "log2") {
                    return yy::parser::make_LOG2_CMATH();
                }
                if (s == ",") {
                    return yy::parser::make_SEPARADOR();
                }
                if (s == ";") {
                    return yy::parser::make_FINAL_SENT();
                }
                if (s == "=") {
                    return yy::parser::make_OPE_ASIGN();
                }
                if (s == "+") {
                    return yy::parser::make_SUM();
                }
                if (s == "-") {
                    return yy::parser::make_REST();
                }
                if (s == "*") {
                    return yy::parser::make_MULT();
                }
                if (s == "/") {
                    return yy::parser::make_DIV();
                }
                if (s == "{") {
                    return yy::parser::make_LLAVE_IZQ();
                }
                if (s == "}") {
                    return yy::parser::make_LLAVE_DER();
                }
                if (s == "(") {
                    return yy::parser::make_PAREN_IZQ();
                }
                if (s == ")") {
                    return yy::parser::make_PAREN_DER();
                }
                if (es_OpeComp(s)) {
                    return yy::parser::make_OPE_COMP();
                    // return s;
                }
                if (s == "cout") {
                    return yy::parser::make_OBJ_COUT();
                }
                if (s == "cin") {
                    return yy::parser::make_OBJ_CIN();
                }
                if (s == "<<") {
                    return yy::parser::make_OPE_SALIDA();
                }
                if (s == ">>") {
                    return yy::parser::make_OPE_ENTRADA();
                }
                if (!s.empty()) { 
                    if (es_Numero(s)){
                        numero = std::stod(s);
                        return yy::parser::make_NUMERO(numero); 
                    }
                    return yy::parser::make_ID(s); 
                }
            }
        }else if (  (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || 
                    (c >= '0' && c <= '9') || 
                    (c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == '=') ||
                    (c == '<') || (c == '>') || 
                    (c == '{') || (c == '}') || (c == '(') || (c == ')') || (c == ';') || (c == ',') ||
                    (c == '.') ) {// reading some name
            s += c; 
        }else{
            return yy::parser::make_YYUNDEF();
            // return "ERROR";
        }
    }
}
int main(){
    // system("pause");
    file.open("input.txt");

    // std::string temp = obtener_siguiente_token();
    // while (!temp.empty())
    // {
    //     std::cout << temp << " ";
    //     temp = obtener_siguiente_token();
    // }
    // std::cout << "\n";
    // system("pause");

    yy::parser p;
        p.parse();
    return 0;
}

namespace yy{
    parser::symbol_type yylex()
    {
        return obtener_siguiente_token();
    }
}