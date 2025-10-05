#include "lexer.h"
#include "exception.h"
#include <cctype>
#include <sstream>
#include <unordered_set>

Lexer::Lexer(const char *filename) : in(filename) {
    if (!in) {
        throw std::runtime_error("Failed to open file");
    }
}

Lexer::~Lexer() {
    in.close();
}

Token Lexer::getNextToken() {
    static char ch = '\0';

    auto readChar = [&]() {
        if (in.get(ch)) {
            if (ch == '\n') {
                line++;
                col = 0;
            } else {
                col++;
            }
        } else {
            ch = '\0';
        }
    };

    auto peekChar = [&]() -> char {
        return static_cast<char>(in.peek());
    };

    auto eatWhitespace = [&]() {
        while (std::isspace(static_cast<unsigned char>(ch)) || (ch == '\0' && !in.eof())) {
            readChar();
        }
    };

    auto readIdentifier = [&]() -> std::string {
        std::stringstream ss;
        while ('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_') {
            ss << ch;
            readChar();
        }
        return ss.str();
    };

    auto readNumber = [&]() -> std::string {
        std::stringstream ss;
        while ('0' <= ch && ch <= '9') {
            ss << ch;
            readChar();
        }
        return ss.str();
    };

    auto isKeyword = [&](const std::string& s) -> bool {
        static const std::unordered_set<std::string> keywords = {
            "func", "var", "let", "if", "else", "while", "print", "return"
        };
        return keywords.count(s) > 0;
    };

    // If first time, read initial character
    if (col == 0 && line == 1 && ch == '\0') {
        readChar();
    }

    eatWhitespace();

    int tokenLine = line;
    int tokenCol = col; 

    // Delimiters
    switch (ch) {
        case '\0':
            {
                return Token(Token::EndOfFile, "", tokenLine, tokenCol);
            }
        case '{': case '}':
        case '(': case ')':
        case ',': case ';':
            {
                std::string lexeme(1, ch);
                readChar();
                return Token(Token::Delimiter, lexeme, tokenLine, tokenCol);
            }
        case ':':
            if (peekChar() == '=') {
                std::string lexeme(1, ch);
                readChar(); // consume ':'
                lexeme += ch;
                readChar(); // consume '='
                return Token(Token::AssignOp, lexeme, tokenLine, tokenCol);
            } else {
                std::string lexeme(1, ch);
                readChar();
                return Token(Token::Delimiter, lexeme, tokenLine, tokenCol);
            }
        case '!': case '=':
            if (peekChar() == '=') {
                std::string lexeme(1, ch);
                readChar(); // consume ch
                lexeme += ch;
                readChar(); // consume '='
                return Token(Token::Op, lexeme, tokenLine, tokenCol);
            } else {
                // throw error
                throw LexerException(tokenLine, tokenCol);
            }
        // Operators
        case '+': case '-':
        case '*': case '/':
            {
                std::string lexeme(1, ch);
                readChar();
                return Token(Token::Op, lexeme, tokenLine, tokenCol);
            }
        case '<': case '>':
            {
                std::string lexeme(1, ch);
                if (peekChar() == '=') {
                    readChar(); // consume first
                    lexeme += ch; // now ch == '='
                    readChar();
                } else {
                    readChar();
                }
                return Token(Token::Op, lexeme, tokenLine, tokenCol);
            }
        default:
            {
                // Identifiers and keywords
                if ('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_') {
                    std::string lexeme = readIdentifier();
                    Token::Type type = isKeyword(lexeme) ? Token::Keyword : Token::Identifier;
                    return Token(type, lexeme, tokenLine, tokenCol);
                }

                // Numbers
                if ('0' <= ch && ch <= '9') {
                    std::string num = readNumber();
                    long long num2 = std::stoll(num);
                    if (num2 > 2147483647 || num2 < -2147483648) {
                        throw LexerException(tokenLine, tokenCol);
                    }
                    return Token(Token::Integer, num, tokenLine, tokenCol);
                }

                // Comments
                if (ch == '#') {
                    // Skip the '#' character
                    readChar();
                    // Skip all characters until end of line or end of file
                    while (ch != '\n' && ch != '\0') {
                        readChar();
                    }
                }

                // Unknown character — raise exception
                if (!(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0')) {
                    throw LexerException(tokenLine, tokenCol);
                }

                readChar();
                return getNextToken();
            }
    }
}

void Lexer::printTokens() {
    for (const auto& token : tokens) {
        token.print();
    }
}