namespace PascalTypeAnalyzer.Domain.Tokens;

/// <summary>
/// Лексема исходного текста с абсолютной позицией и координатами для UI.
/// </summary>
public sealed record Token(
    TokenType Type,
    string Value,
    int Position,
    int Line,
    int Column);
