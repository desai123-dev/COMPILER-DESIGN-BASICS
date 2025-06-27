#include <iostream>
#include <sstream>
#include <cctype>
#include <stdexcept>

using namespace std;

enum TokenType { NUMBER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, END };

struct Token {
    TokenType type;
    double value;
};

class Lexer {
    string input;
    size_t pos;
public:
    Lexer(const string &in) : input(in), pos(0) {}

    Token getNextToken() {
        while (pos < input.length() && isspace(input[pos])) pos++;
        if (pos >= input.length()) return { END, 0 };
        char curr = input[pos];
        if (isdigit(curr) || curr == '.') {
            double val;
            size_t len;
            val = stod(&input[pos], &len);
            pos += len;
            return { NUMBER, val };
        }
        pos++;
        switch (curr) {
            case '+': return { PLUS, 0 };
            case '-': return { MINUS, 0 };
            case '*': return { MUL, 0 };
            case '/': return { DIV, 0 };
            case '(': return { LPAREN, 0 };
            case ')': return { RPAREN, 0 };
            default: throw runtime_error("Invalid character");
        }
    }
};

class Parser {
    Lexer lexer;
    Token current;

    void eat(TokenType type) {
        if (current.type == type) current = lexer.getNextToken();
        else throw runtime_error("Unexpected token");
    }

    double factor() {
        if (current.type == NUMBER) {
            double val = current.value;
            eat(NUMBER);
            return val;
        } else if (current.type == LPAREN) {
            eat(LPAREN);
            double val = expr();
            eat(RPAREN);
            return val;
        }
        throw runtime_error("Expected number or (");
    }

    double term() {
        double result = factor();
        while (current.type == MUL || current.type == DIV) {
            if (current.type == MUL) {
                eat(MUL);
                result *= factor();
            } else {
                eat(DIV);
                double denom = factor();
                if (denom == 0) throw runtime_error("Division by zero");
                result /= denom;
            }
        }
        return result;
    }

    double expr() {
        double result = term();
        while (current.type == PLUS || current.type == MINUS) {
            if (current.type == PLUS) {
                eat(PLUS);
                result += term();
            } else {
                eat(MINUS);
                result -= term();
            }
        }
        return result;
    }

public:
    Parser(const string &in) : lexer(in) {
        current = lexer.getNextToken();
    }

    double parse() {
        return expr();
    }
};

int main() {
    cout << "Enter an arithmetic expression:\n";
    string input;
    getline(cin, input);
    try {
        Parser parser(input);
        double result = parser.parse();
        cout << "Result = " << result << endl;
    } catch (const exception &e) {
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}
