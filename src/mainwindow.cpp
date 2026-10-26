/**
 * @file mainwindow.cpp
 * @brief Scientific Calculator Implementation with advanced features
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_parser(new ExpressionParser())
    , m_waitingForOperand(true)
    , m_errorState(false)
    , m_decimalEntered(false)
    , m_isScientificMode(true)
    , m_isDarkTheme(true)
    , m_isHistoryVisible(true)
    , m_shiftPressed(false)
    , m_memory(0.0)
    , m_memoryHasValue(false)
    , m_maxHistorySize(MAX_HISTORY_ITEMS)
    , m_settings(new QSettings("QtCalculator", "ScientificCalc", this))
{
    ui->setupUi(this);
    
    initializeUI();
    connectSignals();
    loadSettings();
    applyTheme();
    
    setDisplayText("0");
    ui->expressionInput->setFocus();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete m_parser;
    delete ui;
}

void MainWindow::initializeUI()
{
    setWindowTitle("Scientific Calculator");
    setMinimumSize(600, 500);
    resize(900, 700);
    
    ui->display->setText("0");
    ui->display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    QList<QPushButton*> buttons = findChildren<QPushButton*>();
    for (QPushButton* btn : buttons) {
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setMinimumSize(50, 45);
        btn->setCursor(Qt::PointingHandCursor);
    }
    
    ui->expressionInput->setPlaceholderText("Enter expression (e.g., 2+3*4, sin(30), sqrt(16))");
    
    ui->historyList->setAlternatingRowColors(true);
    ui->historyList->setWordWrap(false);
    ui->historyList->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    updateMemoryDisplay();
    
    ui->btnMC->setToolTip("Memory Clear (MC)");
    ui->btnMR->setToolTip("Memory Recall (MR)");
    ui->btnMPlus->setToolTip("Memory Add (M+)");
    ui->btnMMinus->setToolTip("Memory Subtract (M-)");
    ui->btnToggleTheme->setToolTip("Toggle Dark/Light Theme");
    ui->btnToggleScientific->setToolTip("Toggle Scientific Functions");
    ui->btnToggleHistory->setToolTip("Toggle History Panel");
    
    setButtonStyles();
}

void MainWindow::connectSignals()
{
    connect(ui->expressionInput, &QLineEdit::returnPressed, 
            this, &MainWindow::on_expressionInput_returnPressed);
    connect(ui->expressionInput, &QLineEdit::textChanged, 
            this, &MainWindow::on_expressionInput_textChanged);
    
    connect(ui->btnClearHistory, &QPushButton::clicked, 
            this, &MainWindow::on_btnClearHistory_clicked);
    connect(ui->historyList, &QListWidget::clicked, 
            this, &MainWindow::on_historyItemClicked);
    connect(ui->historyList, &QListWidget::doubleClicked,
            this, &MainWindow::on_historyItemClicked);
    
    connect(ui->btnToggleTheme, &QPushButton::clicked, 
            this, &MainWindow::on_btnToggleTheme_clicked);
    connect(ui->btnToggleScientific, &QPushButton::clicked, 
            this, &MainWindow::on_btnToggleScientific_clicked);
    connect(ui->btnToggleHistory, &QPushButton::clicked, 
            this, &MainWindow::on_btnToggleHistory_clicked);
}

void MainWindow::loadSettings()
{
    m_isDarkTheme = m_settings->value("theme/dark", true).toBool();
    m_isScientificMode = m_settings->value("view/scientific", true).toBool();
    ui->btnToggleScientific->setChecked(m_isScientificMode);
    m_isHistoryVisible = m_settings->value("view/history", true).toBool();
    ui->btnToggleHistory->setChecked(m_isHistoryVisible);
    m_memory = m_settings->value("memory/value", 0.0).toDouble();
    m_memoryHasValue = m_settings->value("memory/hasValue", false).toBool();
}

void MainWindow::saveSettings()
{
    m_settings->setValue("theme/dark", m_isDarkTheme);
    m_settings->setValue("view/scientific", m_isScientificMode);
    m_settings->setValue("view/history", m_isHistoryVisible);
    m_settings->setValue("memory/value", m_memory);
    m_settings->setValue("memory/hasValue", m_memoryHasValue);
}

void MainWindow::on_btn0_clicked() { appendNumber("0"); }
void MainWindow::on_btn1_clicked() { appendNumber("1"); }
void MainWindow::on_btn2_clicked() { appendNumber("2"); }
void MainWindow::on_btn3_clicked() { appendNumber("3"); }
void MainWindow::on_btn4_clicked() { appendNumber("4"); }
void MainWindow::on_btn5_clicked() { appendNumber("5"); }
void MainWindow::on_btn6_clicked() { appendNumber("6"); }
void MainWindow::on_btn7_clicked() { appendNumber("7"); }
void MainWindow::on_btn8_clicked() { appendNumber("8"); }
void MainWindow::on_btn9_clicked() { appendNumber("9"); }

void MainWindow::on_btnPlus_clicked() { appendOperator("+"); }
void MainWindow::on_btnMinus_clicked() { appendOperator("-"); }
void MainWindow::on_btnMultiply_clicked() { appendOperator("*"); }
void MainWindow::on_btnDivide_clicked() { appendOperator("/"); }
void MainWindow::on_btnPower_clicked() { appendOperator("^"); }
void MainWindow::on_btnModulo_clicked() { appendOperator("%"); }

void MainWindow::on_btnSin_clicked() { appendFunction("sin"); }
void MainWindow::on_btnCos_clicked() { appendFunction("cos"); }
void MainWindow::on_btnTan_clicked() { appendFunction("tan"); }
void MainWindow::on_btnLog_clicked() { appendFunction("log"); }
void MainWindow::on_btnLn_clicked() { appendFunction("ln"); }
void MainWindow::on_btnSqrt_clicked() { appendFunction("sqrt"); }
void MainWindow::on_btnFactorial_clicked() { appendFunction("fact"); }
void MainWindow::on_btnPi_clicked() { appendToExpression("3.14159265358979323846"); }
void MainWindow::on_btnE_clicked() { appendToExpression("2.71828182845904523536"); }

void MainWindow::on_btnMC_clicked()
{
    m_memory = 0.0;
    m_memoryHasValue = false;
    updateMemoryDisplay();
}

void MainWindow::on_btnMR_clicked()
{
    if (m_memoryHasValue) {
        appendToExpression(QString::number(m_memory, 'g', 15));
    } else {
        displayError("Memory is empty");
    }
}

void MainWindow::on_btnMPlus_clicked()
{
    QString current = ui->display->text();
    bool ok;
    double value = current.toDouble(&ok);
    if (ok && !m_errorState) {
        m_memory += value;
        m_memoryHasValue = true;
        updateMemoryDisplay();
    } else if (m_errorState) {
        displayError("Cannot add error state to memory");
    }
}

void MainWindow::on_btnMMinus_clicked()
{
    QString current = ui->display->text();
    bool ok;
    double value = current.toDouble(&ok);
    if (ok && !m_errorState) {
        m_memory -= value;
        m_memoryHasValue = true;
        updateMemoryDisplay();
    } else if (m_errorState) {
        displayError("Cannot subtract from memory in error state");
    }
}

void MainWindow::on_btnEqual_clicked() { calculate(); }
void MainWindow::on_btnClear_clicked() { clearExpression(); }
void MainWindow::on_btnClearEntry_clicked() { clearEntry(); }
void MainWindow::on_btnBackspace_clicked() { backspace(); }

void MainWindow::on_btnDecimal_clicked()
{
    if (!m_decimalEntered) {
        appendToExpression(".");
        m_decimalEntered = true;
    }
}

void MainWindow::on_btnPercent_clicked()
{
    QString current = ui->expressionInput->text();
    if (current.isEmpty()) {
        displayError("Enter a number first");
        return;
    }
    
    bool ok;
    double value = current.toDouble(&ok);
    if (ok) {
        double result = value / 100.0;
        QString resultStr = formatNumber(result);
        setDisplayText(resultStr);
        addToHistory(current + "%", resultStr);
        m_currentExpression = resultStr;
        m_waitingForOperand = true;
    } else {
        displayError("Invalid number for percentage");
    }
}

void MainWindow::on_btnSign_clicked()
{
    QString current = ui->expressionInput->text();
    if (current.isEmpty()) {
        appendToExpression("-");
        return;
    }
    
    bool ok;
    double value = current.toDouble(&ok);
    if (ok) {
        value = -value;
        QString newValue = formatNumber(value);
        m_currentExpression = newValue;
        ui->expressionInput->setText(newValue);
        setDisplayText(newValue);
    } else {
        if (current.startsWith("-")) {
            current = current.mid(1);
        } else {
            current = "-" + current;
        }
        m_currentExpression = current;
        ui->expressionInput->setText(current);
        setDisplayText(current);
    }
}

void MainWindow::on_btnParenOpen_clicked() { appendToExpression("("); }
void MainWindow::on_btnParenClose_clicked() { appendToExpression(")"); }

void MainWindow::on_expressionInput_returnPressed()
{
    m_currentExpression = ui->expressionInput->text();
    calculate();
}

void MainWindow::on_expressionInput_textChanged(const QString &text)
{
    m_currentExpression = text;
    if (!text.isEmpty()) {
        setDisplayText(text);
    }
}

void MainWindow::on_btnClearHistory_clicked() { clearHistory(); }

void MainWindow::on_historyItemClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString item = ui->historyList->item(index.row())->text();
        QStringList parts = item.split("=");
        if (parts.size() >= 2) {
            QString result = parts.last().trimmed();
            if (!result.startsWith("Error")) {
                appendToExpression(result);
            }
        }
    }
}

void MainWindow::on_btnToggleTheme_clicked()
{
    m_isDarkTheme = !m_isDarkTheme;
    applyTheme();
    saveSettings();
}

void MainWindow::on_btnToggleScientific_clicked() { toggleScientificMode(); }
void MainWindow::on_btnToggleHistory_clicked() { toggleHistory(); }

void MainWindow::appendToExpression(const QString &text)
{
    if (m_errorState) { clearError(); }
    
    if (m_waitingForOperand) {
        m_currentExpression = text;
        m_waitingForOperand = false;
    } else {
        m_currentExpression += text;
    }
    
    ui->expressionInput->setText(m_currentExpression);
    setDisplayText(m_currentExpression);
    ui->expressionInput->setFocus();
}

void MainWindow::appendNumber(const QString &number)
{
    if (m_errorState) { clearExpression(); }
    
    if (m_waitingForOperand) {
        m_currentExpression = number;
        m_waitingForOperand = false;
        m_decimalEntered = (number == ".");
    } else {
        m_currentExpression += number;
    }
    
    ui->expressionInput->setText(m_currentExpression);
    setDisplayText(m_currentExpression);
    ui->expressionInput->setFocus();
}

void MainWindow::appendOperator(const QString &op)
{
    if (m_errorState) { clearError(); return; }
    
    if (m_waitingForOperand) {
        m_currentExpression = ui->display->text() + op;
    } else {
        if (!m_currentExpression.isEmpty() && 
            QString("+-*/^%").contains(m_currentExpression.right(1))) {
            m_currentExpression.chop(1);
        }
        m_currentExpression += op;
    }
    
    m_waitingForOperand = false;
    m_decimalEntered = false;
    ui->expressionInput->setText(m_currentExpression);
    setDisplayText(m_currentExpression);
    ui->expressionInput->setFocus();
}

void MainWindow::appendFunction(const QString &func)
{
    if (m_errorState) { clearError(); }
    
    if (m_waitingForOperand) {
        m_currentExpression = func + "(";
        m_waitingForOperand = false;
    } else {
        if (!m_currentExpression.isEmpty() && 
            (m_currentExpression.right(1)[0].isDigit() || 
             m_currentExpression.right(1) == ")")) {
            m_currentExpression += "*";
        }
        m_currentExpression += func + "(";
    }
    
    m_decimalEntered = false;
    ui->expressionInput->setText(m_currentExpression);
    setDisplayText(m_currentExpression);
    ui->expressionInput->setFocus();
}

void MainWindow::clearExpression()
{
    m_currentExpression.clear();
    m_waitingForOperand = true;
    m_errorState = false;
    m_decimalEntered = false;
    setDisplayText("0");
    ui->expressionInput->clear();
    ui->expressionInput->setFocus();
}

void MainWindow::clearEntry()
{
    if (m_errorState) {
        clearExpression();
    } else {
        setDisplayText("0");
        m_waitingForOperand = true;
        m_decimalEntered = false;
    }
    ui->expressionInput->setFocus();
}

void MainWindow::backspace()
{
    if (m_errorState) { clearExpression(); return; }
    
    QString text = ui->expressionInput->text();
    if (!text.isEmpty()) {
        text.chop(1);
        m_currentExpression = text;
        ui->expressionInput->setText(text);
        setDisplayText(text.isEmpty() ? "0" : text);
        m_decimalEntered = text.contains(".");
    }
    ui->expressionInput->setFocus();
}

void MainWindow::calculate()
{
    QString expression = ui->expressionInput->text().trimmed();
    if (expression.isEmpty()) return;
    
    while (!expression.isEmpty() && QString("+-*/^%").contains(expression.right(1))) {
        expression.chop(1);
    }
    
    if (expression.isEmpty()) return;
    
    bool success;
    QString errorMsg;
    double result = m_parser->evaluate(expression, &success, &errorMsg);
    
    if (success) {
        QString resultStr = formatNumber(result);
        setDisplayText(resultStr);
        ui->expressionInput->setText(expression + " = " + resultStr);
        addToHistory(expression, resultStr);
        m_currentExpression = resultStr;
        m_waitingForOperand = true;
        m_decimalEntered = resultStr.contains(".") || resultStr.contains("e");
        m_errorState = false;
    } else {
        displayError(errorMsg);
    }
    
    ui->expressionInput->setFocus();
}

void MainWindow::addToHistory(const QString &expression, const QString &result, bool isError)
{
    CalculationHistory entry;
    entry.expression = expression;
    entry.result = result;
    entry.timestamp = QDateTime::currentDateTime();
    entry.isError = isError;
    
    m_history.prepend(entry);
    while (m_history.size() > m_maxHistorySize) {
        m_history.removeLast();
    }
    
    updateHistoryDisplay();
}

void MainWindow::clearHistory()
{
    m_history.clear();
    ui->historyList->clear();
}

void MainWindow::updateHistoryDisplay()
{
    ui->historyList->clear();
    for (const auto &entry : m_history) {
        QString display = entry.expression + " = " + entry.result;
        QListWidgetItem *item = new QListWidgetItem(display);
        item->setToolTip("Calculated at: " + entry.timestamp.toString("yyyy-MM-dd hh:mm:ss"));
        
        if (entry.isError) { 
            item->setForeground(m_isDarkTheme ? QColor(255, 100, 100) : QColor(200, 0, 0));
        }
        
        ui->historyList->addItem(item);
    }
}

void MainWindow::updateMemoryDisplay()
{
    if (m_memoryHasValue) {
        QString memText = "M: " + formatNumber(m_memory);
        ui->memoryLabel->setText(memText);
        ui->memoryLabel->setStyleSheet(m_isDarkTheme 
            ? "QLabel { color: #4a90d9; font-weight: bold; }"
            : "QLabel { color: #0066cc; font-weight: bold; }");
    } else {
        ui->memoryLabel->setText("M: 0");
        ui->memoryLabel->setStyleSheet("");
    }
}

void MainWindow::displayError(const QString &message)
{
    m_errorState = true;
    setDisplayText("Error: " + message);
    addToHistory(ui->expressionInput->text(), "Error", true);
}

void MainWindow::clearError()
{
    m_errorState = false;
    m_currentExpression.clear();
    setDisplayText("0");
    ui->expressionInput->clear();
}

void MainWindow::setDisplayText(const QString &text)
{
    QString displayText = text;
    if (displayText.length() > MAX_DISPLAY_LENGTH) {
        displayText = displayText.left(MAX_DISPLAY_LENGTH - 3) + "...";
    }
    ui->display->setText(displayText);
}

QString MainWindow::getDisplayText() const
{
    return ui->display->text();
}

QString MainWindow::formatNumber(double number) const
{
    if (std::isnan(number)) return "NaN";
    if (std::isinf(number)) return number > 0 ? "Infinity" : "-Infinity";
    
    QString result = QString::number(number, 'g', 15);
    if (result.contains('.') && !result.contains('e')) {
        while (result.endsWith('0')) result.chop(1);
        if (result.endsWith('.')) result.chop(1);
    }
    return result;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) m_shiftPressed = true;
    handleKeyboardInput(event->key(), event->text());
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) m_shiftPressed = false;
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::handleKeyboardInput(int key, const QString &text)
{
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        appendNumber(QString::number(key - Qt::Key_0));
        return;
    }
    
    switch (key) {
    case Qt::Key_Plus:
        on_btnPlus_clicked();
        break;
    case Qt::Key_Minus:
        on_btnMinus_clicked();
        break;
    case Qt::Key_Asterisk:
        on_btnMultiply_clicked();
        break;
    case Qt::Key_Slash:
        on_btnDivide_clicked();
        break;
    case Qt::Key_AsciiCircum:
    case Qt::Key_6:
        if (m_shiftPressed) on_btnPower_clicked();
        break;
    case Qt::Key_Percent:
        on_btnPercent_clicked();
        break;
    case Qt::Key_ParenLeft:
        on_btnParenOpen_clicked();
        break;
    case Qt::Key_ParenRight:
        on_btnParenClose_clicked();
        break;
    case Qt::Key_Period:
        on_btnDecimal_clicked();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        on_btnEqual_clicked();
        break;
    case Qt::Key_Backspace:
        on_btnBackspace_clicked();
        break;
    case Qt::Key_Delete:
        on_btnClearEntry_clicked();
        break;
    case Qt::Key_Escape:
        on_btnClear_clicked();
        break;
    case Qt::Key_S:
        if (m_shiftPressed) on_btnSin_clicked();
        else appendToExpression("s");
        break;
    case Qt::Key_C:
        if (m_shiftPressed) on_btnCos_clicked();
        else appendToExpression("c");
        break;
    case Qt::Key_T:
        if (m_shiftPressed) on_btnTan_clicked();
        else appendToExpression("t");
        break;
    case Qt::Key_L:
        if (m_shiftPressed) on_btnLog_clicked();
        else appendToExpression("l");
        break;
    case Qt::Key_N:
        if (m_shiftPressed) on_btnLn_clicked();
        else appendToExpression("n");
        break;
    case Qt::Key_R:
        if (m_shiftPressed) on_btnSqrt_clicked();
        else appendToExpression("r");
        break;
    case Qt::Key_P:
        if (m_shiftPressed) on_btnPi_clicked();
        else appendToExpression("p");
        break;
    case Qt::Key_E:
        if (m_shiftPressed) on_btnE_clicked();
        else appendToExpression("e");
        break;
    case Qt::Key_F:
        if (m_shiftPressed) on_btnFactorial_clicked();
        else appendToExpression("f");
        break;
    default:
        if (!text.isEmpty() && text[0].isPrint() && !text[0].isSpace()) {
            appendToExpression(text);
        }
    }
}

bool MainWindow::expressionEndsWithOperator() const
{
    if (m_currentExpression.isEmpty()) return false;
    return QString("+-*/^%").contains(m_currentExpression.right(1));
}

bool MainWindow::isExpressionValid() const
{
    return m_parser->isValid(m_currentExpression);
}

bool MainWindow::isScientificFunction(const QString &text) const
{
    QStringList functions = {"sin", "cos", "tan", "log", "ln", "sqrt", "fact"};
    return functions.contains(text.toLower());
}

void MainWindow::toggleScientificMode()
{
    m_isScientificMode = ui->btnToggleScientific->isChecked();
    
    QList<QPushButton*> sciButtons = {
        ui->btnSin, ui->btnCos, ui->btnTan,
        ui->btnLog, ui->btnLn, ui->btnSqrt,
        ui->btnPower, ui->btnModulo,
        ui->btnFactorial, ui->btnPi, ui->btnE
    };
    
    for (QPushButton* btn : sciButtons) {
        btn->setVisible(m_isScientificMode);
    }
    
    saveSettings();
}

void MainWindow::toggleHistory()
{
    m_isHistoryVisible = ui->btnToggleHistory->isChecked();
    ui->historyWidget->setVisible(m_isHistoryVisible);
    saveSettings();
}

void MainWindow::applyTheme()
{
    if (m_isDarkTheme) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }
    setButtonStyles();
    updateMemoryDisplay();
}

void MainWindow::applyDarkTheme()
{
    QString darkStylesheet = R"(
        QMainWindow {
            background-color: #1e1e1e;
        }
        QWidget {
            background-color: #1e1e1e;
            color: #ffffff;
        }
        QLineEdit {
            background-color: #2d2d2d;
            color: #ffffff;
            border: 2px solid #3c3c3c;
            border-radius: 8px;
            padding: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #4a90d9;
        }
        QPushButton {
            background-color: #3c3c3c;
            color: #ffffff;
            border: 1px solid #4a4a4a;
            border-radius: 6px;
            padding: 10px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
            border: 2px solid #4a90d9;
        }
        QPushButton:pressed {
            background-color: #4a90d9;
            color: white;
        }
        QPushButton:checked {
            background-color: #4a90d9;
            color: white;
        }
        QListWidget {
            background-color: #2d2d2d;
            color: #ffffff;
            border: 1px solid #3c3c3c;
            border-radius: 8px;
            padding: 5px;
            font-size: 12px;
        }
        QListWidget::item {
            padding: 5px;
            border-radius: 3px;
        }
        QListWidget::item:alternate {
            background-color: #363636;
        }
        QListWidget::item:selected {
            background-color: #4a90d9;
            color: white;
        }
        QListWidget::item:hover {
            background-color: #4a4a4a;
        }
        QLabel {
            color: #ffffff;
            font-size: 13px;
        }
        QScrollBar:vertical {
            background-color: #2d2d2d;
            width: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background-color: #4a4a4a;
            border-radius: 6px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #5a5a5a;
        }
    )";
    
    setStyleSheet(darkStylesheet);
    ui->btnToggleTheme->setText("Light Theme");
}

void MainWindow::applyLightTheme()
{
    QString lightStylesheet = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QWidget {
            background-color: #f5f5f5;
            color: #333333;
        }
        QLineEdit {
            background-color: #ffffff;
            color: #333333;
            border: 2px solid #cccccc;
            border-radius: 8px;
            padding: 8px;
            font-size: 14px;
        }
        QLineEdit:focus {
            border: 2px solid #4a90d9;
        }
        QPushButton {
            background-color: #e8e8e8;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 6px;
            padding: 10px;
            font-weight: bold;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #d8d8d8;
            border: 2px solid #4a90d9;
        }
        QPushButton:pressed {
            background-color: #4a90d9;
            color: white;
        }
        QPushButton:checked {
            background-color: #4a90d9;
            color: white;
        }
        QListWidget {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 8px;
            padding: 5px;
            font-size: 12px;
        }
        QListWidget::item {
            padding: 5px;
            border-radius: 3px;
        }
        QListWidget::item:alternate {
            background-color: #f0f0f0;
        }
        QListWidget::item:selected {
            background-color: #4a90d9;
            color: white;
        }
        QListWidget::item:hover {
            background-color: #e8e8e8;
        }
        QLabel {
            color: #333333;
            font-size: 13px;
        }
        QScrollBar:vertical {
            background-color: #f0f0f0;
            width: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background-color: #cccccc;
            border-radius: 6px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background-color: #aaaaaa;
        }
    )";
    
    setStyleSheet(lightStylesheet);
    ui->btnToggleTheme->setText("Dark Theme");
}

void MainWindow::setButtonStyles()
{
    QList<QPushButton*> numberButtons = {
        ui->btn0, ui->btn1, ui->btn2, ui->btn3, ui->btn4,
        ui->btn5, ui->btn6, ui->btn7, ui->btn8, ui->btn9
    };
    
    for (QPushButton* btn : numberButtons) {
        btn->setStyleSheet(m_isDarkTheme 
            ? "QPushButton { background-color: #4a4a4a; font-size: 16px; }"
            : "QPushButton { background-color: #ffffff; font-size: 16px; }");
    }
    
    QList<QPushButton*> operatorButtons = {
        ui->btnPlus, ui->btnMinus, ui->btnMultiply, ui->btnDivide,
        ui->btnPower, ui->btnModulo
    };
    
    for (QPushButton* btn : operatorButtons) {
        btn->setStyleSheet(
            "QPushButton { "
            "   background-color: #ff9500; "
            "   color: white; "
            "   font-size: 16px; "
            "   font-weight: bold; "
            "}"
        );
    }
    
    ui->btnEqual->setStyleSheet(
        "QPushButton { "
        "   background-color: #4caf50; "
        "   color: white; "
        "   font-size: 18px; "
        "   font-weight: bold; "
        "}"
    );
    
    QList<QPushButton*> clearButtons = { ui->btnClear, ui->btnClearEntry };
    for (QPushButton* btn : clearButtons) {
        btn->setStyleSheet(
            "QPushButton { "
            "   background-color: #f44336; "
            "   color: white; "
            "   font-size: 14px; "
            "}"
        );
    }
    
    QList<QPushButton*> sciButtons = {
        ui->btnSin, ui->btnCos, ui->btnTan,
        ui->btnLog, ui->btnLn, ui->btnSqrt,
        ui->btnFactorial, ui->btnPi, ui->btnE,
        ui->btnPercent, ui->btnSign
    };
    
    for (QPushButton* btn : sciButtons) {
        btn->setStyleSheet(m_isDarkTheme 
            ? "QPushButton { background-color: #5a4a3a; font-size: 13px; }"
            : "QPushButton { background-color: #fff8e7; font-size: 13px; }");
    }
    
    QList<QPushButton*> memoryButtons = {
        ui->btnMC, ui->btnMR, ui->btnMPlus, ui->btnMMinus
    };
    
    for (QPushButton* btn : memoryButtons) {
        btn->setStyleSheet(m_isDarkTheme 
            ? "QPushButton { background-color: #3a4a5a; font-size: 13px; }"
            : "QPushButton { background-color: #e7f0ff; font-size: 13px; }");
    }
}
