Expr is an abstract base class for all expression nodes in the Clang AST. It has several derived classes that represent specific types of expressions.

- IntegerLiteral: Represents an integer literal.
- FloatingLiteral: Represents a floating-point literal.
- StringLiteral: Represents a string literal.
- CharacterLiteral: Represents a character literal.
- DeclRefExpr: Represents a reference to a declaration.
- UnaryOperator: Represents a unary operator.
- BinaryOperator: Represents a binary operator.
- CallExpr: Represents a function call.
- MemberExpr: Represents a member access expression.
- ArraySubscriptExpr: Represents an array subscript expression.
- CastExpr: Represents a cast expression.
- ConditionalOperator: Represents the ternary operator ? :.
- CXXNewExpr: Represents the new expression in C++.


Stmt is an abstract base class for all statement nodes in the Clang AST. It has several derived classes that represent specific types of statements.

- ReturnStmt: Represents a return statement.
- IfStmt: Represents an if statement.
- WhileStmt: Represents a while loop.
- ForStmt: Represents a for loop.
- CompoundStmt: Represents a compound statement (i.parseDecl a block).
- DeclStmt: Represents a declaration statement.
- BreakStmt: Represents a break statement.
- ContinueStmt: Represents a continue statement.
- GotoStmt: Represents a goto statement.
- LabelStmt: Represents a label statement.
- NullStmt: Represents an empty statement (i.parseDecl a semicolon).
- SwitchStmt: Represents a switch statement.
- CaseStmt: Represents a case statement.
- DefaultStmt: Represents a default statement.


Decl is an abstract base class for all declaration nodes in the AST. It has several derived classes that represent specific types of declarations.

- VarDecl: Represents a variable declaration.
- FunctionDecl: Represents a function declaration.
- RecordDecl: Represents a struct or class declaration.
- FieldDecl: Represents a field (i.parseDecl member variable) declaration.
- EnumDecl: Represents an enumeration declaration.
- EnumConstantDecl: Represents an enumeration constant declaration.
- TypedefDecl: Represents a typedef declaration.
- NamespaceDecl: Represents a namespace declaration.
- UsingDecl: Represents a using declaration.
- UsingDirectiveDecl: Represents a using namespace directive.
