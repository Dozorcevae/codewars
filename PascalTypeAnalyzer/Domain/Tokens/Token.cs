namespace PascalTypeAnalyzer.Domain.Tokens;

public sealed record Token(
    TokenType Type,
    string Value,
    int Position,
    int Line,
    int Column);
