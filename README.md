# Scientific Calculator

<p align="center">
  <img src="screenshots/calculator_icon.png" alt="Calculator Icon" width="128">
</p>

<p align="center">
  <b>A professional-grade scientific calculator built with Qt6 and C++</b>
</p>

<p align="center">
  <img alt="Qt Version" src="https://img.shields.io/badge/Qt-6.x-green.svg">
  <img alt="C++ Standard" src="https://img.shields.io/badge/C%2B%2B-17-blue.svg">
  <img alt="License" src="https://img.shields.io/badge/license-MIT-yellow.svg">
  <img alt="Platform" src="https://img.shields.io/badge/platform-Windows%20%7C%20macOS%20%7C%20Linux-lightgrey.svg">
</p>

---

## Project Overview

This is a **portfolio-quality scientific calculator** application that demonstrates modern C++ and Qt development practices. Built with clean architecture principles, it features a responsive UI, comprehensive error handling, and full expression parsing capabilities.

### Key Highlights

- **Expression Parser**: Recursive descent parser with proper operator precedence
- **Scientific Functions**: Complete set of mathematical operations
- **Memory Management**: Full MC/MR/M+/M- functionality
- **History Tracking**: Persistent calculation history with timestamps
- **Dual Themes**: Dark and Light mode support
- **Keyboard Support**: Full keyboard navigation and shortcuts
- **Cross-Platform**: Works on Windows, macOS, and Linux

---

## Features

### Basic Operations
- Addition (+), Subtraction (-), Multiplication (*), Division (/)
- Modulo operations (%)
- Decimal number support with proper input validation
- Clear (C), Clear Entry (CE), Backspace
- Sign toggle (+/-)
- Percentage calculations

### Scientific Operations
| Function | Description | Example |
|----------|-------------|---------|
| `sin(x)` | Sine (degrees) | sin(30) = 0.5 |
| `cos(x)` | Cosine (degrees) | cos(60) = 0.5 |
| `tan(x)` | Tangent (degrees) | tan(45) = 1 |
| `log(x)` | Base-10 logarithm | log(100) = 2 |
| `ln(x)` | Natural logarithm | ln(e) = 1 |
| `sqrt(x)` | Square root | sqrt(16) = 4 |
| `x^y` | Power operation | 2^3 = 8 |
| `fact(x)` | Factorial | fact(5) = 120 |
| `pi` | Pi constant (3.14159...) | - |
| `e` | Euler's number (2.71828...) | - |

### Expression Parsing

The calculator supports complex expressions with proper operator precedence:

```
(5 + 2) * 4       = 28
sin(30) + cos(60) = 1
sqrt(16) ^ 2      = 16
fact(5) / 10      = 12
3.14159 * 2       = 6.28318
```

**Operator Precedence (PEMDAS):**
1. Parentheses
2. Exponents (^)
3. Multiplication, Division, Modulo (*, /, %)
4. Addition, Subtraction (+, -)

### Memory Functions
- **MC** (Memory Clear): Clears memory
- **MR** (Memory Recall): Recalls stored value
- **M+** (Memory Add): Adds current value to memory
- **M-** (Memory Subtract): Subtracts current value from memory

Visual indicator shows when memory contains a value.

### Calculation History
- Stores up to 100 previous calculations
- Shows expression and result
- Timestamps for each entry
- Click history item to reuse result
- Clear history button
- Session-only history (cleared on exit)

### Keyboard Shortcuts

| Key | Action |
|-----|--------|
| 0-9 | Input numbers |
| +, -, *, / | Operators |
| ^ | Power operator |
| ( ) | Parentheses |
| . | Decimal point |
| Enter/Return | Calculate |
| Backspace | Delete last character |
| Delete | Clear entry |
| Escape | Clear all |
| Shift+S | sin() |
| Shift+C | cos() |
| Shift+T | tan() |
| Shift+L | log() |
| Shift+N | ln() |
| Shift+R | sqrt() |
| Shift+P | pi |
| Shift+E | e |
| Shift+F | factorial |

### Themes

Toggle between **Dark** and **Light** themes:
- Professional color schemes
- Smooth transitions
- Persisted user preference

---

## Technologies Used

- **Framework**: Qt 6.x
- **Language**: C++17
- **Build System**: qmake
- **UI Framework**: Qt Widgets
- **Styling**: QSS (Qt Style Sheets)
- **Math Library**: Standard C++ `<cmath>`

---

## Project Structure

```
Calculator/
├── src/                          # Source files
│   ├── main.cpp                  # Application entry point
│   ├── mainwindow.cpp            # Main window implementation
│   └── expressionparser.cpp      # Math expression parser
├── include/                      # Header files
│   ├── mainwindow.h              # Main window declarations
│   └── expressionparser.h        # Parser declarations
├── ui/                           # UI files
│   └── mainwindow.ui             # Qt Designer layout
├── screenshots/                  # Application screenshots
├── docs/                         # Documentation
├── Calculator.pro                # Qt project file
├── .gitignore                    # Git ignore rules
└── README.md                     # This file
```

---

## Installation

### Prerequisites

- Qt 6.x or higher
- MinGW, MSVC (Windows), or GCC/Clang (Linux/macOS)
- CMake or qmake

### Building from Source

#### Windows

```bash
# Clone the repository
git clone https://github.com/Rajat-Pandya/qt-scientific-calculator.git
cd qt-scientific-calculator

# Build with qmake
qmake Calculator.pro
mingw32-make

# Run
./release/Calculator.exe
```

#### macOS

```bash
# Clone the repository
git clone https://github.com/Rajat-Pandya/qt-scientific-calculator.git
cd qt-scientific-calculator

# Build
qmake Calculator.pro
make

# Run
./Calculator.app/Contents/MacOS/Calculator
```

#### Linux

```bash
# Clone the repository
git clone https://github.com/Rajat-Pandya/qt-scientific-calculator.git
cd qt-scientific-calculator

# Build
qmake Calculator.pro
make

# Run
./Calculator
```

### Using Qt Creator

1. Open `Calculator.pro` in Qt Creator
2. Configure project with your kit
3. Build (Ctrl+B) and Run (Ctrl+R)

---

## Usage

### Basic Calculations

1. Enter numbers using the keypad or keyboard
2. Click operators or type them
3. Press `=` or Enter to calculate

### Scientific Functions

1. Click the function button (e.g., `sin`)
2. Enter the argument in parentheses
3. Close parenthesis and press `=`

**Example:** `sin(30)` → `0.5`

### Complex Expressions

Enter expressions naturally:
```
(4 + 6) * 2 / 5
sin(30) + cos(60)
sqrt(16) ^ 2 + fact(3)
```

### Memory Operations

1. Calculate a value
2. Click **M+** to add to memory
3. Click **MR** to recall memory value
4. Click **MC** to clear memory

### History

- View previous calculations in the right panel
- Click any history item to load its result
- Click "Clear History" to remove all entries

### Theme Switching

Click the **Theme** button to toggle between Dark and Light modes.

---

## Architecture

### Expression Parser

The calculator uses a **recursive descent parser** that implements:

- **Tokenization**: Converts input string to tokens
- **Grammar Rules**: Enforces operator precedence
- **Error Handling**: Comprehensive validation
- **Scientific Functions**: Built-in function support

**Parsing Flow:**
```
Expression -> Term { (+|-) Term }
Term -> Power { (*|/|%) Power }
Power -> Factor [ ^ Power ]
Factor -> Number | Function | ( Expression ) | Unary
```

### Error Handling

The application gracefully handles:
- Division by zero
- Invalid expressions
- Unbalanced parentheses
- Invalid function arguments
- Overflow errors

### Memory Management

- Uses RAII principles
- Proper cleanup in destructors
- Smart pointer usage where applicable

---

## Screenshots

<p align="center">
  <img src="screenshots/dark_mode.png" alt="Dark Mode" width="400">
  <img src="screenshots/light_mode.png" alt="Light Mode" width="400">
</p>

<p align="center">
  <i>Dark and Light theme comparison</i>
</p>

<p align="center">
  <img src="screenshots/scientific_mode.png" alt="Scientific Mode" width="400">
  <img src="screenshots/history_panel.png" alt="History Panel" width="400">
</p>

<p align="center">
  <i>Scientific functions and calculation history</i>
</p>

---

## Code Quality

- **Clean Code**: Well-organized, readable, and maintainable
- **Documentation**: Comprehensive inline documentation
- **Error Handling**: Robust error checking and reporting
- **Modern C++**: Uses C++17 features
- **Qt Best Practices**: Follows Qt coding standards
- **No Memory Leaks**: Proper resource management

---

## Future Improvements

- [ ] Unit tests for calculator logic
- [ ] Save calculation history to file
- [ ] Additional scientific functions (hyperbolic, constants)
- [ ] Graph plotting capabilities
- [ ] Unit conversion features
- [ ] Multi-language support
- [ ] Customizable themes
- [ ] Calculator settings/preferences
- [ ] Export history to CSV
- [ ] Scientific notation toggle

---

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

### Development Setup

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## Author

**Rajat Pandya**
- GitHub: [@Rajat-Pandya](https://github.com/Rajat-Pandya)

---

<p align="center">
  <i>Built with</i> ❤️ <i>using Qt and C++</i>
</p>
