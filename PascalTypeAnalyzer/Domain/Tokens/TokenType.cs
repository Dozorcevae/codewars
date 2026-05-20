namespace PascalTypeAnalyzer.Domain.Tokens;

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
