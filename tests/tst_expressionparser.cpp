#include <QtTest>
#include <cmath>
#include "expressionparser.h"

class TestExpressionParser : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    // Basic arithmetic & precedence
    void testPrecedence();
    void testRightAssociativePower();
    void testParentheses();
    void testModulo();
    void testUnaryMinus();

    // Scientific functions
    void testSin();
    void testCos();
    void testTan();
    void testLog();
    void testLn();
    void testSqrt();
    void testFactorial();

    // Constants
    void testConstants();

    // Error handling
    void testDivisionByZero();
    void testLogNonPositive();
    void testSqrtNegative();
    void testFactorialNonInteger();
    void testFactorialOverflow();
    void testUnbalancedParentheses();
    void testUnknownCharacter();
    void testEmptyExpression();

    // Utility methods
    void testIsValid();
    void testFormatExpression();
    void testGetSupportedFunctions();

private:
    ExpressionParser *parser;
};

void TestExpressionParser::init()
{
    parser = new ExpressionParser();
}

void TestExpressionParser::cleanup()
{
    delete parser;
    parser = nullptr;
}

void TestExpressionParser::testPrecedence()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("2+3*4", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 14.0);
}

void TestExpressionParser::testRightAssociativePower()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("2^3^2", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 512.0); // 2^(3^2) = 2^9 = 512
}

void TestExpressionParser::testParentheses()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("(2+3)*4", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 20.0);
}

void TestExpressionParser::testModulo()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("10%3", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 1.0);
}

void TestExpressionParser::testUnaryMinus()
{
    bool success = false;
    QString errorMsg;
    double result1 = parser->evaluate("-5+10", &success, &errorMsg);
    QVERIFY(success);
    QCOMPARE(result1, 5.0);

    double result2 = parser->evaluate("3*-2", &success, &errorMsg);
    QVERIFY(success);
    QCOMPARE(result2, -6.0);
}

void TestExpressionParser::testSin()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("sin(30)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QVERIFY(std::abs(result - 0.5) < 1e-9);
}

void TestExpressionParser::testCos()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("cos(60)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QVERIFY(std::abs(result - 0.5) < 1e-9);

    double resultZero = parser->evaluate("cos(0)", &success, &errorMsg);
    QVERIFY(success);
    QCOMPARE(resultZero, 1.0);
}

void TestExpressionParser::testTan()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("tan(45)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QVERIFY(std::abs(result - 1.0) < 1e-9);
}

void TestExpressionParser::testLog()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("log(100)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 2.0);
}

void TestExpressionParser::testLn()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("ln(e)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QVERIFY(std::abs(result - 1.0) < 1e-9);
}

void TestExpressionParser::testSqrt()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("sqrt(16)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 4.0);
}

void TestExpressionParser::testFactorial()
{
    bool success = false;
    QString errorMsg;
    double result = parser->evaluate("fact(5)", &success, &errorMsg);
    QVERIFY(success);
    QVERIFY(errorMsg.isEmpty());
    QCOMPARE(result, 120.0);
}

void TestExpressionParser::testConstants()
{
    bool success = false;
    QString errorMsg;
    double resultPi = parser->evaluate("pi", &success, &errorMsg);
    QVERIFY(success);
    QCOMPARE(resultPi, M_PI);

    double resultE = parser->evaluate("e", &success, &errorMsg);
    QVERIFY(success);
    QCOMPARE(resultE, M_E);
}

void TestExpressionParser::testDivisionByZero()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("10/0", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Division by zero"));
}

void TestExpressionParser::testLogNonPositive()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("log(0)", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Logarithm undefined for non-positive values"));

    parser->evaluate("log(-5)", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Logarithm undefined for non-positive values"));
}

void TestExpressionParser::testSqrtNegative()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("sqrt(-4)", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Square root undefined for negative values"));
}

void TestExpressionParser::testFactorialNonInteger()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("fact(4.5)", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Factorial only defined for integers"));
}

void TestExpressionParser::testFactorialOverflow()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("fact(20)", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Factorial result too large"));
}

void TestExpressionParser::testUnbalancedParentheses()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("(2+3", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Unbalanced parentheses"));

    parser->evaluate("2+3)", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Unbalanced parentheses"));
}

void TestExpressionParser::testUnknownCharacter()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("2@3", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Unknown character: @"));
}

void TestExpressionParser::testEmptyExpression()
{
    bool success = true;
    QString errorMsg;
    parser->evaluate("   ", &success, &errorMsg);
    QVERIFY(!success);
    QCOMPARE(errorMsg, QString("Empty expression"));
}

void TestExpressionParser::testIsValid()
{
    QVERIFY(parser->isValid("2+3*4"));
    QVERIFY(parser->isValid("sin(30)+cos(60)"));
    QVERIFY(!parser->isValid("10/0"));
    QVERIFY(!parser->isValid("(2+3"));
    QVERIFY(!parser->isValid(""));
}

void TestExpressionParser::testFormatExpression()
{
    QString formatted = parser->formatExpression("2+3*4");
    QCOMPARE(formatted, QString("2 + 3 * 4"));
}

void TestExpressionParser::testGetSupportedFunctions()
{
    QStringList funcs = parser->getSupportedFunctions();
    funcs.sort();
    QStringList expected = {"cos", "fact", "ln", "log", "sin", "sqrt", "tan"};
    expected.sort();
    QCOMPARE(funcs, expected);
}

QTEST_MAIN(TestExpressionParser)
#include "tst_expressionparser.moc"
