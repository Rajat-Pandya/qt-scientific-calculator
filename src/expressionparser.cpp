/**
 * @file expressionparser.cpp
 * @brief Implementation of mathematical expression parser
 */

#include "expressionparser.h"
#include <QDebug>

ExpressionParser::ExpressionParser()
    : m_position(0)
    , m_error(false)
{
    initializeFunctions();
    initializeConstants();
}

void ExpressionParser::initializeFunctions()
{
    m_functions["sin"] = [](double x) { return std::sin(x * M_PI / 180.0); };
    m_functions["cos"] = [](double x) { return std::cos(x * M_PI / 180.0); };
    m_functions["tan"] = [](double x) { return std::tan(x * M_PI / 180.0); };
    m_functions["log"] = [](double x) { 
        if (x <= 0) throw std::runtime_error("Logarithm undefined for non-positive values");
        return std::log10(x); 
    };
    m_functions["ln"] = [](double x) { 
        if (x <= 0) throw std::runtime_error("Natural logarithm undefined for non-positive values");
        return std::log(x); 
    };
    m_functions["sqrt"] = [](double x) { 
        if (x < 0) throw std::runtime_error("Square root undefined for negative values");
        return std::sqrt(x); 
    };
    m_functions["fact"] = [this](double x) { return factorial(x); };
}

void ExpressionParser::initializeConstants()
{
    m_constants["pi"] = M_PI;
    m_constants["e"] = M_E;
}

double ExpressionParser::factorial(double n)
{
    if (n < 0) {
        throw std::runtime_error("Factorial undefined for negative numbers");
    }
    if (n != std::floor(n)) {
        throw std::runtime_error("Factorial only defined for integers");
    }
    
    unsigned long long result = 1;
    for (int i = 2; i <= static_cast<int>(n); ++i) {
        result *= i;
        if (result > 1e15) {
            throw std::runtime_error("Factorial result too large");
        }
    }
    return static_cast<double>(result);
}

double ExpressionParser::modulo(double a, double b)
{
    if (b == 0) {
        throw std::runtime_error("Modulo by zero");
    }
    return std::fmod(a, b);
}

double ExpressionParser::evaluate(const QString &expression, bool *success, QString *errorMessage)
{
    m_expression = expression.trimmed();
    m_position = 0;
    m_error = false;
    m_errorMessage.clear();
    
    if (m_expression.isEmpty()) {
        setError("Empty expression");
        if (success) *success = false;
        if (errorMessage) *errorMessage = m_errorMessage;
        return 0;
    }
    
    // Check parentheses balance
    if (!hasBalancedParentheses(m_expression)) {
        setError("Unbalanced parentheses");
        if (success) *success = false;
        if (errorMessage) *errorMessage = m_errorMessage;
        return 0;
    }
    
    nextToken();
    double result = 0;
    
    try {
        result = parseExpression();
        if (m_currentToken.type != END && !m_error) {
            setError("Unexpected token: " + m_currentToken.value);
        }
    } catch (const std::exception &e) {
        setError(QString::fromStdString(e.what()));
    }
    
    if (success) *success = !m_error;
    if (errorMessage) *errorMessage = m_errorMessage;
    
    return m_error ? 0 : result;
}

void ExpressionParser::nextToken()
{
    skipWhitespace();
    
    if (m_position >= m_expression.length()) {
        m_currentToken = {END, "", 0};
        return;
    }
    
    QChar current = m_expression[m_position];
    
    // Number (integer or decimal)
    if (current.isDigit() || current == '.') {
        m_currentToken = parseNumber();
        return;
    }
    
    // Identifiers (functions or constants)
    if (current.isLetter()) {
        QString identifier;
        while (m_position < m_expression.length() && 
               (m_expression[m_position].isLetterOrNumber() || m_expression[m_position] == '_')) {
            identifier += m_expression[m_position];
            ++m_position;
        }
        
        QString idLower = identifier.toLower();
        
        if (m_functions.contains(idLower)) {
            m_currentToken = {FUNCTION, identifier, 0};
        } else if (m_constants.contains(idLower)) {
            m_currentToken = {CONSTANT, identifier, m_constants[idLower]};
        } else {
            setError("Unknown function or constant: " + identifier);
            m_currentToken = {END, "", 0};
        }
        return;
    }
    
    // Operators and parentheses
    ++m_position;
    switch (current.unicode()) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '%':
        m_currentToken = {OPERATOR, QString(current), 0};
        break;
    case '(':
        m_currentToken = {LPAREN, "(", 0};
        break;
    case ')':
        m_currentToken = {RPAREN, ")", 0};
        break;
    default:
        setError("Unknown character: " + QString(current));
        m_currentToken = {END, "", 0};
    }
}

void ExpressionParser::skipWhitespace()
{
    while (m_position < m_expression.length() && m_expression[m_position].isSpace()) {
        ++m_position;
    }
}

ExpressionParser::Token ExpressionParser::parseNumber()
{
    QString numberStr;
    bool hasDecimal = false;
    bool hasExponent = false;
    
    while (m_position < m_expression.length()) {
        QChar c = m_expression[m_position];
        
        if (c.isDigit()) {
            numberStr += c;
            ++m_position;
        } else if (c == '.' && !hasDecimal && !hasExponent) {
            hasDecimal = true;
            numberStr += c;
            ++m_position;
        } else if ((c == 'e' || c == 'E') && !hasExponent && !numberStr.isEmpty()) {
            hasExponent = true;
            numberStr += c;
            ++m_position;
            // Handle sign in exponent
            if (m_position < m_expression.length() && 
                (m_expression[m_position] == '+' || m_expression[m_position] == '-')) {
                numberStr += m_expression[m_position];
                ++m_position;
            }
        } else {
            break;
        }
    }
    
    bool ok;
    double value = numberStr.toDouble(&ok);
    if (!ok) {
        setError("Invalid number: " + numberStr);
        return {END, "", 0};
    }
    return {NUMBER, numberStr, value};
}

double ExpressionParser::parseExpression()
{
    double left = parseTerm();
    
    while (m_currentToken.type == OPERATOR && 
           (m_currentToken.value == "+" || m_currentToken.value == "-")) {
        QString op = m_currentToken.value;
        nextToken();
        double right = parseTerm();
        
        if (m_error) return 0;
        
        if (op == "+") {
            left += right;
        } else {
            left -= right;
        }
    }
    
    return left;
}

double ExpressionParser::parseTerm()
{
    double left = parsePower();
    
    while (m_currentToken.type == OPERATOR && 
           (m_currentToken.value == "*" || m_currentToken.value == "/" || m_currentToken.value == "%")) {
        QString op = m_currentToken.value;
        nextToken();
        double right = parsePower();
        
        if (m_error) return 0;
        
        if (op == "*") {
            left *= right;
        } else if (op == "/") {
            if (right == 0) {
                throw std::runtime_error("Division by zero");
            }
            left /= right;
        } else {
            left = modulo(left, right);
        }
    }
    
    return left;
}

double ExpressionParser::parsePower()
{
    double left = parseFactor();
    
    if (m_currentToken.type == OPERATOR && m_currentToken.value == "^") {
        nextToken();
        double right = parsePower();  // Right associative
        
        if (m_error) return 0;
        
        left = std::pow(left, right);
    }
    
    return left;
}

double ExpressionParser::parseFactor()
{
    if (m_error) return 0;
    
    // Handle unary plus/minus
    if (m_currentToken.type == OPERATOR && 
        (m_currentToken.value == "+" || m_currentToken.value == "-")) {
        QString op = m_currentToken.value;
        nextToken();
        double value = parseFactor();
        return (op == "-") ? -value : value;
    }
    
    return parsePrimary();
}

double ExpressionParser::parsePrimary()
{
    if (m_error) return 0;
    
    switch (m_currentToken.type) {
    case NUMBER:
    case CONSTANT: {
        double value = m_currentToken.numericValue;
        nextToken();
        return value;
    }
    
    case FUNCTION: {
        QString funcName = m_currentToken.value.toLower();
        nextToken();
        
        if (m_currentToken.type != LPAREN) {
            setError("Expected '(' after function name");
            return 0;
        }
        nextToken();
        
        double arg = parseExpression();
        
        if (m_currentToken.type != RPAREN) {
            setError("Expected ')' after function argument");
            return 0;
        }
        nextToken();
        
        return applyFunction(funcName, arg);
    }
    
    case LPAREN: {
        nextToken();
        double value = parseExpression();
        
        if (m_currentToken.type != RPAREN) {
            setError("Expected ')'");
            return 0;
        }
        nextToken();
        return value;
    }
    
    default:
        setError("Unexpected token: " + m_currentToken.value);
        return 0;
    }
}

double ExpressionParser::applyFunction(const QString &funcName, double value)
{
    try {
        return m_functions[funcName](value);
    } catch (const std::exception &e) {
        setError(QString::fromStdString(e.what()));
        return 0;
    }
}

void ExpressionParser::setError(const QString &message)
{
    m_error = true;
    m_errorMessage = message;
}

void ExpressionParser::clearError()
{
    m_error = false;
    m_errorMessage.clear();
}

bool ExpressionParser::isValid(const QString &expression) const
{
    ExpressionParser tempParser;
    bool success;
    tempParser.evaluate(expression, &success, nullptr);
    return success;
}

QString ExpressionParser::formatExpression(const QString &expression) const
{
    QString formatted;
    for (int i = 0; i < expression.length(); ++i) {
        QChar c = expression[i];
        if (isOperator(c) || c == '(' || c == ')') {
            if (!formatted.isEmpty() && !formatted.endsWith(' ')) {
                formatted += ' ';
            }
            formatted += c;
            formatted += ' ';
        } else {
            formatted += c;
        }
    }
    return formatted.trimmed();
}

QStringList ExpressionParser::getSupportedFunctions() const
{
    return QStringList(m_functions.keys());
}

QString ExpressionParser::getSupportedOperators() const
{
    return "+, -, *, /, ^, %";
}

bool ExpressionParser::isOperator(QChar c) const
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

bool ExpressionParser::isValidIdentifierChar(QChar c) const
{
    return c.isLetterOrNumber() || c == '_';
}

int ExpressionParser::getPrecedence(QChar op) const
{
    switch (op.unicode()) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 2;
    case '^':
        return 3;
    default:
        return 0;
    }
}

bool ExpressionParser::isLeftAssociative(QChar op) const
{
    return op != '^';  // Only ^ is right-associative
}

double ExpressionParser::applyOperator(QChar op, double left, double right)
{
    switch (op.unicode()) {
    case '+': return left + right;
    case '-': return left - right;
    case '*': return left * right;
    case '/':
        if (right == 0) throw std::runtime_error("Division by zero");
        return left / right;
    case '^': return std::pow(left, right);
    case '%': return modulo(left, right);
    default: throw std::runtime_error("Unknown operator");
    }
}

int ExpressionParser::countParentheses(const QString &expression) const
{
    int count = 0;
    for (QChar c : expression) {
        if (c == '(') count++;
        else if (c == ')') count--;
    }
    return count;
}

bool ExpressionParser::hasBalancedParentheses(const QString &expression) const
{
    return countParentheses(expression) == 0;
}

bool ExpressionParser::hasConsecutiveOperators(const QString &expression) const
{
    QString lastOp;
    for (int i = 0; i < expression.length(); ++i) {
        QChar c = expression[i];
        if (isOperator(c)) {
            if (!lastOp.isEmpty() && c != '-') {
                return true;
            }
            lastOp = c;
        } else if (!c.isSpace()) {
            lastOp.clear();
        }
    }
    return false;
}
