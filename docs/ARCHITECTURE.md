# Architecture Documentation

## Calculator Application Architecture

### Overview

The Scientific Calculator is built using a modular architecture with clear separation of concerns:

```
┌─────────────────────────────────────────┐
│           UI Layer (mainwindow)         │
│  - Handles user interactions           │
│  - Manages themes and styling          │
│  - Displays results and history        │
└──────────────────┬──────────────────────┘
                   │
┌──────────────────▼──────────────────────┐
│        Logic Layer (ExpressionParser)  │
│  - Parses mathematical expressions     │
│  - Evaluates with proper precedence    │
│  - Handles errors and edge cases       │
└──────────────────┬──────────────────────┘
                   │
┌──────────────────▼──────────────────────┐
│         Math Layer (Standard C++)      │
│  - <cmath> functions                   │
│  - Mathematical constants            │
└─────────────────────────────────────────┘
```

## Components

### 1. MainWindow (mainwindow.h/cpp)

**Responsibilities:**
- UI management and layout
- Event handling (buttons, keyboard)
- Theme switching
- History management
- Memory operations
- Display updates

**Key Features:**
- Qt Widgets-based interface
- Custom QSS styling
- Settings persistence
- Keyboard shortcuts

### 2. ExpressionParser (expressionparser.h/cpp)

**Responsibilities:**
- Tokenize input expressions
- Parse with recursive descent
- Evaluate with operator precedence
- Handle scientific functions
- Error detection and reporting

**Grammar:**
```
Expression := Term { ('+' | '-') Term }
Term := Power { ('*' | '/' | '%') Power }
Power := Factor [ '^' Power ]
Factor := Number | Function | '(' Expression ')' | ('+' | '-') Factor
Function := Identifier '(' Expression ')'
Number := Digit+ [ '.' Digit* ] [ ('e' | 'E') [ '+' | '-' ] Digit+ ]
Identifier := Letter+
```

**Token Types:**
- NUMBER: Numeric values
- OPERATOR: +, -, *, /, ^, %
- FUNCTION: sin, cos, tan, log, ln, sqrt, fact
- CONSTANT: pi, e
- LPAREN/RPAREN: (, )
- END: End of input

### 3. CalculationHistory Structure

**Fields:**
- `expression`: Original expression
- `result`: Calculated result
- `timestamp`: When calculated
- `isError`: Whether it was an error

## Data Flow

### Calculation Flow

```
1. User Input (button click / keyboard)
   ↓
2. appendNumber/appendOperator/appendFunction
   ↓
3. Update expressionInput
   ↓
4. User presses = or Enter
   ↓
5. calculate() called
   ↓
6. ExpressionParser::evaluate(expression)
   ↓
7. Tokenize and parse expression
   ↓
8. Evaluate with recursive descent
   ↓
9. Return result or error
   ↓
10. Update display
    ↓
11. Add to history
```

### Theme Flow

```
1. User clicks Theme button
   ↓
2. Toggle m_isDarkTheme
   ↓
3. applyTheme() called
   ↓
4. applyDarkTheme() or applyLightTheme()
   ↓
5. setStyleSheet() with QSS
   ↓
6. setButtonStyles() for individual buttons
   ↓
7. saveSettings() to persist
```

## Error Handling Strategy

### Parser Errors
- Empty expressions
- Invalid characters
- Unbalanced parentheses
- Unknown functions
- Invalid numbers

### Mathematical Errors
- Division by zero
- Logarithm of non-positive
- Square root of negative
- Factorial of non-integer
- Overflow

### UI Errors
- Cannot use error state in memory
- Memory empty on recall
- Invalid input format

## Memory Management

### Smart Practices
1. **RAII**: All resources managed by constructors/destructors
2. **Qt Parent-Child**: UI elements auto-deleted by parent
3. **Raw Pointers**: Only for non-Qt objects (parser)
4. **Manual Delete**: In destructor for parser

### Memory Breakdown
- MainWindow: Allocated on stack in main()
- ExpressionParser: Allocated in constructor, deleted in destructor
- UI Components: Managed by Qt's parent-child system
- Settings: Uses QSettings (managed by Qt)

## Thread Safety

### Current Design
- Single-threaded application
- All operations in main thread
- No shared mutable state
- Settings use Qt's thread-safe QSettings

### Future Considerations
- If adding complex calculations, consider worker thread
- Use signals/slots for thread communication
- Protect shared data with mutexes

## Performance Considerations

### Optimization Strategies
1. **Lazy Evaluation**: Only calculate when needed
2. **String Building**: Use QString efficiently
3. **History Limiting**: Max 100 items to prevent memory bloat
4. **Expression Validation**: Quick check before full parse

### Bottlenecks
- Expression parsing: O(n) for n tokens
- History display: O(n) for n items
- Factorial: O(n) calculation
- Power: std::pow is optimized

## Testing Strategy

### Unit Tests (Recommended)
- ExpressionParser tests
  - Valid expressions
  - Invalid expressions
  - Edge cases (division by zero)
- MainWindow tests
  - Button clicks
  - Keyboard input
  - Theme switching

### Integration Tests
- End-to-end calculations
- History persistence
- Settings save/load

### Manual Testing
- UI responsiveness
- Visual appearance
- Theme switching
- Error message display

## Extension Points

### Adding New Functions

1. Add to `initializeFunctions()` in ExpressionParser:
```cpp
m_functions["newfunc"] = [](double x) { return customCalculation(x); };
```

2. Add button in mainwindow.ui

3. Connect slot in mainwindow.cpp

### Adding New Themes

1. Create `applyNewTheme()` method

2. Define QSS stylesheet

3. Add theme enum

4. Update settings handling

### Adding History Export

1. Add menu item or button

2. Implement export method

3. Use QFile for file I/O

4. Format as CSV/JSON

## Security Considerations

### Input Validation
- All expressions validated before parsing
- No arbitrary code execution
- Numbers bounded by double limits

### File Operations
- Settings use QSettings (safe)
- No user-defined file paths currently
- Future: Sanitize file paths

## Build System

### qmake Configuration
- Cross-platform project file
- Separate directories for source, headers, UI
- Resource compilation for stylesheets

### Dependencies
- Qt 6.x (Core, GUI, Widgets)
- Standard C++ library
- No external libraries

## Future Architecture Improvements

### 1. Plugin System
- Load functions from plugins
- Dynamic function registration

### 2. Undo/Redo
- Command pattern
- Operation history stack

### 3. Multi-threading
- Complex calculations in worker thread
- Non-blocking UI

### 4. Custom Functions
- User-defined functions
- Function storage in settings

### 5. Graphing
- Plot expressions
- 2D/3D graphing capability
