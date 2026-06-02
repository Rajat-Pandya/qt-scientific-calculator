/**
 * @file expressionparser.h
 * @brief Mathematical expression parser with operator precedence support
 * @details Implements a recursive descent parser for evaluating mathematical expressions
 *          Supports operators: +, -, *, /, ^, %, parentheses, and scientific functions
 *          Follows proper operator precedence (PEMDAS/BODMAS):
 *          - Parentheses
 *          - Exponents (^)
 *          - Multiplication, Division, Modulo (*, /, %)
 *          - Addition and Subtraction (+, -)
 */

#ifndef EXPRESSIONPARSER_H
#define EXPRESSIONPARSER_H

#include <QString>
#include <QStack>
#include <QMap>
#include <cmath>
#include <functional>
#include <stdexcept>

/**
 * @class ExpressionParser
 * @brief Parses and evaluates mathematical expressions with full error handling
 * 
 * This class provides comprehensive expression parsing capabilities:
 * - Basic arithmetic: +, -, *, /, %, ^
 * - Parentheses for grouping
 * - Scientific functions: sin, cos, tan, log, ln, sqrt, fact
 * - Mathematical constants: pi, e
 * - Unary operators (negative numbers)
 * - Proper operator precedence
 * - Comprehensive error handling
 */
class ExpressionParser
{
public:
    /**
     * @brief Constructor - initializes supported functions
     */
    ExpressionParser();
    
    /**
     * @brief Evaluate a mathematical expression
     * @param expression The expression string to evaluate
     * @param success Optional pointer to bool to indicate success/failure
     * @param errorMessage Optional pointer to string to store error message
     * @return The result of the evaluation (0 on error)
     * 
     * Example expressions:
     * - "2 + 3 * 4" -> 14
     * - "sin(30) + cos(60)" -> 1.0
     * - "sqrt(16) ^ 2" -> 16
     * - "fact(5)" -> 120
     */
    double evaluate(const QString &expression, bool *success = nullptr, QString *errorMessage = nullptr);
    
    /**
     * @brief Check if an expression is syntactically valid
     * @param expression The expression to validate
     * @return True if valid, false otherwise
     */
    bool isValid(const QString &expression) const;
    
    /**
     * @brief Format the expression for display (add spaces around operators)
     * @param expression Raw expression string
     * @return Formatted expression string
     */
    QString formatExpression(const QString &expression) const;
    
    /**
     * @brief Get list of supported functions
     * @return List of function names
     */
    QStringList getSupportedFunctions() const;
    
    /**
     * @brief Get list of supported operators
     * @return List of operator characters
     */
    QString getSupportedOperators() const;

private:
    /**
     * @brief Token types for lexical analysis
     */
    enum TokenType {
        NUMBER,         ///< Numeric value
        OPERATOR,       ///< Arithmetic operator
        FUNCTION,       ///< Mathematical function
        CONSTANT,       ///< Mathematical constant (pi, e)
        LPAREN,         ///< Left parenthesis '('
        RPAREN,         ///< Right parenthesis ')'
        END             ///< End of input
    };
    
    /**
     * @brief Token structure for lexical analysis
     */
    struct Token {
        TokenType type;
        QString value;
        double numericValue;
    };
    
    // Parser state
    QString m_expression;           ///< Current expression being parsed
    int m_position;                 ///< Current position in expression
    Token m_currentToken;           ///< Current token
    bool m_error;                   ///< Error flag
    QString m_errorMessage;         ///< Error message
    
    // Function map: maps function names to lambda implementations
    QMap<QString, std::function<double(double)>> m_functions;
    
    // Constant map
    QMap<QString, double> m_constants;
    
    // Initialization
    void initializeFunctions();
    void initializeConstants();
    
    // Lexical analysis
    void nextToken();
    void skipWhitespace();
    Token parseNumber();
    Token parseIdentifier();
    
    // Recursive descent parsing (grammar rules)
    double parseExpression();       ///< Parse +, -
    double parseTerm();             ///< Parse *, /, %
    double parsePower();            ///< Parse ^
    double parseFactor();           ///< Parse numbers, functions, parentheses, unary -
    double parsePrimary();          ///< Parse primary expressions
    
    // Function application
    double applyFunction(const QString &funcName, double value);
    
    // Mathematical operations
    double factorial(double n);
    double modulo(double a, double b);
    
    // Error handling
    void setError(const QString &message);
    void clearError();
    
    // Utility methods
    bool isOperator(QChar c) const;
    bool isValidIdentifierChar(QChar c) const;
    int getPrecedence(QChar op) const;
    bool isLeftAssociative(QChar op) const;
    double applyOperator(QChar op, double left, double right);
    
    // Validation helpers
    int countParentheses(const QString &expression) const;
    bool hasBalancedParentheses(const QString &expression) const;
    bool hasConsecutiveOperators(const QString &expression) const;
};

#endif // EXPRESSIONPARSER_H
