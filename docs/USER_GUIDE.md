# Documentation

## Calculator Documentation

### Table of Contents

1. [Getting Started](#getting-started)
2. [Features](#features)
3. [Keyboard Shortcuts](#keyboard-shortcuts)
4. [Expression Syntax](#expression-syntax)
5. [Scientific Functions](#scientific-functions)
6. [Memory Operations](#memory-operations)
7. [Error Messages](#error-messages)
8. [Settings](#settings)

## Getting Started

### First Launch

When you first launch the calculator:
1. The application starts in Dark theme by default
2. Scientific functions are visible
3. History panel is shown on the right
4. Memory is empty (M: 0)

### Basic Usage

1. **Enter Numbers**: Click digit buttons or type on keyboard
2. **Operators**: Click operator buttons or use keyboard
3. **Calculate**: Press = or Enter
4. **Clear**: Use C (Clear All) or CE (Clear Entry)

## Features

### Expression Input

Type mathematical expressions directly:
- Numbers: `0-9`, `.` for decimal
- Operators: `+`, `-`, `*`, `/`, `^`, `%`
- Parentheses: `(`, `)`
- Functions: `sin()`, `cos()`, `tan()`, `log()`, `ln()`, `sqrt()`, `fact()`
- Constants: `pi`, `e`

### History

- Stores last 100 calculations
- Click history item to reuse result
- Clear with "Clear History" button
- Shows timestamp on hover

### Themes

- **Dark Theme**: Easy on the eyes, professional look
- **Light Theme**: Clean and bright
- Toggle with "Theme" button

### Memory

- **MC**: Clear memory
- **MR**: Recall memory value
- **M+**: Add to memory
- **M-**: Subtract from memory

## Keyboard Shortcuts

### Numbers and Basic Operations
| Key | Function |
|-----|----------|
| 0-9 | Input digit |
| . | Decimal point |
| + | Addition |
| - | Subtraction |
| * | Multiplication |
| / | Division |
| ^ | Power |
| % | Modulo |
| ( ) | Parentheses |

### Control Keys
| Key | Function |
|-----|----------|
| Enter/Return | Calculate |
| Backspace | Delete last character |
| Delete | Clear entry |
| Escape | Clear all |

### Scientific Functions (with Shift)
| Key + Shift | Function |
|-------------|----------|
| Shift+S | sin() |
| Shift+C | cos() |
| Shift+T | tan() |
| Shift+L | log() |
| Shift+N | ln() |
| Shift+R | sqrt() |
| Shift+F | fact() |
| Shift+P | pi constant |
| Shift+E | e constant |

## Expression Syntax

### Basic Arithmetic
```
2 + 3          = 5
10 - 5         = 5
4 * 5          = 20
20 / 4         = 5
2 ^ 3          = 8
10 % 3         = 1
```

### Complex Expressions
```
(2 + 3) * 4    = 20
2 + 3 * 4      = 14 (not 20, due to precedence)
sin(30)        = 0.5
sqrt(16) + 5   = 9
fact(5)        = 120
```

### Operator Precedence
1. Parentheses `()`
2. Functions `sin()`, `cos()`, etc.
3. Exponents `^`
4. Multiplication, Division, Modulo `*`, `/`, `%`
5. Addition, Subtraction `+`, `-`

## Scientific Functions

### Trigonometric Functions (Degrees)
- `sin(x)`: Sine of angle x
- `cos(x)`: Cosine of angle x
- `tan(x)`: Tangent of angle x

### Logarithmic Functions
- `log(x)`: Base-10 logarithm
- `ln(x)`: Natural logarithm (base e)

### Other Functions
- `sqrt(x)`: Square root
- `fact(n)`: Factorial (n!)
- `pow(x,y)` or `x^y`: Power

### Constants
- `pi`: 3.14159265358979323846
- `e`: 2.71828182845904523536

## Memory Operations

### Memory Clear (MC)
Clears memory to 0. Memory indicator shows "M: 0".

### Memory Recall (MR)
Inserts memory value into current expression.

### Memory Add (M+)
Adds current display value to memory.

### Memory Subtract (M-)
Subtracts current display value from memory.

### Memory Indicator
- Blue text when memory has value
- Normal text when memory is empty

## Error Messages

### Division by Zero
```
Error: Division by zero
```

### Invalid Expression
```
Error: Unbalanced parentheses
Error: Unknown function
Error: Invalid number
```

### Function Domain Errors
```
Error: Logarithm undefined for non-positive values
Error: Square root undefined for negative values
Error: Factorial undefined for negative numbers
```

### Memory Errors
```
Error: Memory is empty
Error: Cannot add error state to memory
```

## Settings

Settings are automatically saved and include:
- Theme preference (Dark/Light)
- Scientific mode visibility
- History panel visibility
- Memory value
- Calculation history

Settings are stored using QSettings and persist between sessions.
