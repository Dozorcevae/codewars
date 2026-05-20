using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Tokens;

namespace PascalTypeAnalyzer.Application.Lexing;

public sealed class Lexer
{
    private static readonly HashSet<string> StandardTypes = new(StringComparer.OrdinalIgnoreCase)
    {
        "integer", "real", "boolean", "char", "string"
    };

    public (List<Token> Tokens, List<DiagnosticMessage> Diagnostics) Lex(string text)
    {
        var tokens = new List<Token>();
        var diagnostics = new List<DiagnosticMessage>();

        var pos = 0;
        var line = 1;
        var column = 1;

        while (pos < text.Length)
        {
            var ch = text[pos];
            if (char.IsWhiteSpace(ch))
            {
                Advance(ch, ref pos, ref line, ref column);
                continue;
            }

            var startPos = pos;
            var startLine = line;
            var startCol = column;

            if (char.IsLetter(ch))
            {
                var value = ReadWhile(text, ref pos, ref line, ref column, c => char.IsLetterOrDigit(c) || c == '_');
                tokens.Add(new Token(GetWordTokenType(value), value, startPos, startLine, startCol));
                continue;
            }

            if (char.IsDigit(ch))
            {
                var value = ReadWhile(text, ref pos, ref line, ref column, char.IsDigit);
                tokens.Add(new Token(TokenType.IntegerNumber, value, startPos, startLine, startCol));
                continue;
            }

            switch (ch)
            {
                case '=':
                    tokens.Add(new Token(TokenType.Equal, "=", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case ';':
                    tokens.Add(new Token(TokenType.Semicolon, ";", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case ',':
                    tokens.Add(new Token(TokenType.Comma, ",", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case '(':
                    tokens.Add(new Token(TokenType.LeftParen, "(", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case ')':
                    tokens.Add(new Token(TokenType.RightParen, ")", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case '[':
                    tokens.Add(new Token(TokenType.LeftBracket, "[", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case ']':
                    tokens.Add(new Token(TokenType.RightBracket, "]", startPos, startLine, startCol));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
                case '.':
                    if (pos + 1 < text.Length && text[pos + 1] == '.')
                    {
                        tokens.Add(new Token(TokenType.Range, "..", startPos, startLine, startCol));
                        Advance('.', ref pos, ref line, ref column);
                        Advance('.', ref pos, ref line, ref column);
                    }
                    else
                    {
                        tokens.Add(new Token(TokenType.Unknown, ".", startPos, startLine, startCol));
                        diagnostics.Add(new DiagnosticMessage(startLine, startCol, "Одиночная точка '.' недопустима, ожидается '..'."));
                        Advance(ch, ref pos, ref line, ref column);
                    }
                    break;
                default:
                    tokens.Add(new Token(TokenType.Unknown, ch.ToString(), startPos, startLine, startCol));
                    diagnostics.Add(new DiagnosticMessage(startLine, startCol, $"Неизвестный символ '{ch}'."));
                    Advance(ch, ref pos, ref line, ref column);
                    break;
            }
        }

        tokens.Add(new Token(TokenType.EndOfInput, string.Empty, pos, line, column));
        return (tokens, diagnostics);
    }

    private static TokenType GetWordTokenType(string value)
    {
        if (value.Equals("type", StringComparison.OrdinalIgnoreCase)) return TokenType.TypeKeyword;
        if (value.Equals("array", StringComparison.OrdinalIgnoreCase)) return TokenType.ArrayKeyword;
        if (value.Equals("of", StringComparison.OrdinalIgnoreCase)) return TokenType.OfKeyword;
        if (StandardTypes.Contains(value)) return TokenType.StandardTypeKeyword;
        return TokenType.Identifier;
    }

    private static string ReadWhile(string text, ref int pos, ref int line, ref int column, Func<char, bool> predicate)
    {
        var start = pos;
        while (pos < text.Length && predicate(text[pos])) Advance(text[pos], ref pos, ref line, ref column);
        return text[start..pos];
    }

    private static void Advance(char ch, ref int pos, ref int line, ref int column)
    {
        pos++;
        if (ch == '\n') { line++; column = 1; } else { column++; }
    }
}
