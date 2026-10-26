/**
 * @file mainwindow.h
 * @brief Scientific Calculator Main Window
 * @details A professional-grade calculator with expression parsing,
 *          scientific functions, memory, history, and theme support
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QVector>
#include <QPair>
#include <QTimer>
#include <QDateTime>
#include <QListWidgetItem>
#include <QSettings>
#include "expressionparser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @struct CalculationHistory
 * @brief Stores a single calculation entry with timestamp
 */
struct CalculationHistory {
    QString expression;
    QString result;
    QDateTime timestamp;
    bool isError;
};

/**
 * @class MainWindow
 * @brief Main application window for the scientific calculator
 * 
 * Features:
 * - Basic arithmetic operations (+, -, *, /, %)
 * - Scientific functions (sin, cos, tan, log, ln, sqrt, pow, factorial, etc.)
 * - Expression parsing with proper operator precedence
 * - Memory functions (MC, MR, M+, M-)
 * - Calculation history with timestamps
 * - Full keyboard support
 * - Dual themes (Dark/Light)
 * - Error handling with user-friendly messages
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent widget
     */
    explicit MainWindow(QWidget *parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~MainWindow();

protected:
    /**
     * @brief Handle keyboard events
     * @param event Key event
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief Handle key release events
     * @param event Key event
     */
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    // Number buttons (0-9)
    void on_btn0_clicked();
    void on_btn1_clicked();
    void on_btn2_clicked();
    void on_btn3_clicked();
    void on_btn4_clicked();
    void on_btn5_clicked();
    void on_btn6_clicked();
    void on_btn7_clicked();
    void on_btn8_clicked();
    void on_btn9_clicked();
    
    // Basic operators
    void on_btnPlus_clicked();
    void on_btnMinus_clicked();
    void on_btnMultiply_clicked();
    void on_btnDivide_clicked();
    void on_btnPower_clicked();
    void on_btnModulo_clicked();
    
    // Scientific functions
    void on_btnSin_clicked();
    void on_btnCos_clicked();
    void on_btnTan_clicked();
    void on_btnLog_clicked();
    void on_btnLn_clicked();
    void on_btnSqrt_clicked();
    void on_btnFactorial_clicked();
    void on_btnPi_clicked();
    void on_btnE_clicked();
    
    // Memory functions
    void on_btnMC_clicked();    // Memory Clear
    void on_btnMR_clicked();    // Memory Recall
    void on_btnMPlus_clicked(); // Memory Add
    void on_btnMMinus_clicked(); // Memory Subtract
    
    // Control buttons
    void on_btnEqual_clicked();
    void on_btnClear_clicked();
    void on_btnClearEntry_clicked();
    void on_btnBackspace_clicked();
    void on_btnDecimal_clicked();
    void on_btnPercent_clicked();
    void on_btnSign_clicked();      // Toggle sign (+/-)
    void on_btnParenOpen_clicked();
    void on_btnParenClose_clicked();
    
    // Expression input
    void on_expressionInput_returnPressed();
    void on_expressionInput_textChanged(const QString &text);
    
    // History
    void on_btnClearHistory_clicked();
    void on_historyItemClicked(const QModelIndex &index);
    
    // Theme
    void on_btnToggleTheme_clicked();
    
    // Mode toggle
    void on_btnToggleScientific_clicked();
    void on_btnToggleHistory_clicked();

private:
    // UI pointer
    Ui::MainWindow *ui;
    
    // Expression parser
    ExpressionParser *m_parser;
    
    // Calculator state
    QString m_currentExpression;
    QString m_displayText;
    bool m_waitingForOperand;
    bool m_errorState;
    bool m_decimalEntered;
    bool m_isScientificMode;
    bool m_isDarkTheme;
    bool m_isHistoryVisible;
    bool m_shiftPressed;        // For shift+key combinations
    
    // Memory storage
    double m_memory;
    bool m_memoryHasValue;
    
    // Calculation history
    QVector<CalculationHistory> m_history;
    int m_maxHistorySize;
    
    // Settings for persisting theme
    QSettings *m_settings;
    
    // Constants
    static const int MAX_DISPLAY_LENGTH = 50;
    static const int MAX_HISTORY_ITEMS = 100;
    
    // Initialization methods
    void initializeUI();
    void connectSignals();
    void loadSettings();
    void saveSettings();
    
    // Theme management
    void applyTheme();
    void applyDarkTheme();
    void applyLightTheme();
    void setButtonStyles();
    
    // Expression handling
    void appendToExpression(const QString &text);
    void appendNumber(const QString &number);
    void appendOperator(const QString &op);
    void appendFunction(const QString &func);
    void clearExpression();
    void clearEntry();
    void backspace();
    void calculate();
    
    // History management
    void addToHistory(const QString &expression, const QString &result, bool isError = false);
    void clearHistory();
    void updateHistoryDisplay();
    
    // Memory management
    void updateMemoryDisplay();
    
    // Error handling
    void displayError(const QString &message);
    void clearError();
    
    // Display management
    void setDisplayText(const QString &text);
    QString getDisplayText() const;
    QString formatNumber(double number) const;
    
    // Keyboard handling
    void handleKeyboardInput(int key, const QString &text);
    void handleScientificKey(int key);
    
    // Utility methods
    bool expressionEndsWithOperator() const;
    bool isExpressionValid() const;
    bool isScientificFunction(const QString &text) const;
    
    // UI toggles
    void toggleScientificMode();
    void toggleHistory();
};

#endif // MAINWINDOW_H
