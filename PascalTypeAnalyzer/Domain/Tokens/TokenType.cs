namespace PascalTypeAnalyzer.Domain.Tokens;

/// <summary>Виды лексем поддерживаемого подъязыка Pascal.</summary>
public enum TokenType
{
    TypeKeyword,
    ArrayKeyword,
    OfKeyword,
    StandardTypeKeyword,
    Identifier,
    IntegerNumber,
    Equal,
    Semicolon,
    Comma,
    LeftParen,
    RightParen,
    LeftBracket,
    RightBracket,
    Range,
    EndOfInput,
    Unknown
}
