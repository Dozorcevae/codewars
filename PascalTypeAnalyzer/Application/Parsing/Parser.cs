using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Syntax;
using PascalTypeAnalyzer.Domain.Tokens;

namespace PascalTypeAnalyzer.Application.Parsing;

public sealed class Parser
{
    private readonly IReadOnlyList<Token> _tokens;
    private readonly List<DiagnosticMessage> _diagnostics = new();
    private int _position;

    public Parser(IReadOnlyList<Token> tokens) => _tokens = tokens;

    public (SyntaxNode? Root, List<DiagnosticMessage> Diagnostics) Parse()
    {
        var root = ParseTypeSection();
        return (root, _diagnostics);
    }

    public SyntaxNode ParseTypeSection()
    {
        var node = new SyntaxNode("TypeSection");
        if (!Match(TokenType.TypeKeyword, "после начала ввода ожидается ключевое слово 'type'.", out var typeToken))
        {
            Recover();
            return node;
        }

        node.Add(new SyntaxNode("type", typeToken.Value, typeToken));

        while (Current.Type != TokenType.EndOfInput)
        {
            var decl = ParseTypeDeclaration();
            if (decl is not null) node.Add(decl);
            else Recover();
        }

        return node;
    }

    public SyntaxNode? ParseTypeDeclaration()
    {
        var node = new SyntaxNode("TypeDeclaration");
        if (!Match(TokenType.Identifier, "после ключевого слова 'type' ожидается имя типа.", out var id)) return null;
        node.Add(new SyntaxNode("Identifier", id.Value, id));

        if (!Match(TokenType.Equal, "после имени типа ожидается символ '='.", out var eq)) return null;
        node.Add(new SyntaxNode("=", eq.Value, eq));

        var typeSpec = ParseTypeSpec();
        if (typeSpec is null) return null;
        node.Add(typeSpec);

        if (!Match(TokenType.Semicolon, "описание типа должно заканчиваться символом ';'.", out var semicolon)) return null;
        node.Add(new SyntaxNode(";", semicolon.Value, semicolon));

        return node;
    }

    public SyntaxNode? ParseTypeSpec() => Current.Type switch
    {
        TokenType.StandardTypeKeyword => ParseStandardType(),
        TokenType.IntegerNumber => ParseSubrangeType(),
        TokenType.LeftParen => ParseEnumType(),
        TokenType.ArrayKeyword => ParseArrayType(),
        TokenType.Identifier => ConsumeIdentifierType(),
        _ => ErrorNode("ожидается описание типа (стандартный, интервальный, перечислимый, массив или пользовательский тип).")
    };

    public SyntaxNode ParseStandardType()
    {
        var token = Next();
        return new SyntaxNode("StandardType", token.Value, token);
    }

    public SyntaxNode? ParseSubrangeType()
    {
        var node = new SyntaxNode("SubrangeType");
        if (!Match(TokenType.IntegerNumber, "в интервальном типе ожидается левая целая граница.", out var left)) return null;
        node.Add(new SyntaxNode("Integer", left.Value, left));

        if (!Match(TokenType.Range, "в интервальном типе ожидается знак диапазона '..'.", out var range)) return null;
        node.Add(new SyntaxNode("Range", range.Value, range));

        if (!Match(TokenType.IntegerNumber, "в интервальном типе ожидается правая целая граница.", out var right)) return null;
        node.Add(new SyntaxNode("Integer", right.Value, right));
        return node;
    }

    public SyntaxNode? ParseEnumType()
    {
        var node = new SyntaxNode("EnumType");
        if (!Match(TokenType.LeftParen, "ожидается '(' для перечислимого типа.", out var leftParen)) return null;
        node.Add(new SyntaxNode("(", leftParen.Value, leftParen));

        if (!Match(TokenType.Identifier, "в перечислимом типе ожидается идентификатор элемента.", out var id)) return null;
        node.Add(new SyntaxNode("Identifier", id.Value, id));

        while (Current.Type == TokenType.Comma)
        {
            var comma = Next();
            node.Add(new SyntaxNode(",", comma.Value, comma));
            if (!Match(TokenType.Identifier, "после запятой ожидается идентификатор элемента перечисления.", out var nextId)) return null;
            node.Add(new SyntaxNode("Identifier", nextId.Value, nextId));
        }

        if (!Match(TokenType.RightParen, "ожидается ')' в конце перечислимого типа.", out var rp)) return null;
        node.Add(new SyntaxNode(")", rp.Value, rp));
        return node;
    }

    public SyntaxNode? ParseArrayType()
    {
        var node = new SyntaxNode("ArrayType");
        var array = Next();
        node.Add(new SyntaxNode("array", array.Value, array));
        if (!Match(TokenType.LeftBracket, "после ключевого слова 'array' ожидается '['.", out var lb)) return null;
        node.Add(new SyntaxNode("[", lb.Value, lb));

        var index = ParseIndexType();
        if (index is null) return null;
        node.Add(index);

        if (!Match(TokenType.RightBracket, "после типа индекса ожидается ']'.", out var rb)) return null;
        node.Add(new SyntaxNode("]", rb.Value, rb));

        if (!Match(TokenType.OfKeyword, "после ']' ожидается ключевое слово 'of'.", out var of)) return null;
        node.Add(new SyntaxNode("of", of.Value, of));

        var nested = ParseTypeSpec();
        if (nested is null) return null;
        node.Add(nested);
        return node;
    }

    public SyntaxNode? ParseIndexType() => Current.Type switch
    {
        TokenType.IntegerNumber => ParseSubrangeType(),
        TokenType.Identifier => ConsumeIdentifierType(),
        _ => ErrorNode("в массиве ожидается индексный тип (интервал или имя типа).")
    };

    private SyntaxNode ConsumeIdentifierType()
    {
        var id = Next();
        return new SyntaxNode("IdentifierType", id.Value, id);
    }

    private bool Match(TokenType type, string message, out Token token)
    {
        token = Current;
        if (Current.Type == type)
        {
            token = Next();
            return true;
        }

        _diagnostics.Add(new DiagnosticMessage(Current.Line, Current.Column, message));
        return false;
    }

    private SyntaxNode? ErrorNode(string message)
    {
        _diagnostics.Add(new DiagnosticMessage(Current.Line, Current.Column, message));
        return null;
    }

    private void Recover()
    {
        while (Current.Type is not TokenType.Semicolon and not TokenType.TypeKeyword and not TokenType.EndOfInput)
            Next();
        if (Current.Type == TokenType.Semicolon) Next();
    }

    private Token Current => _tokens[Math.Min(_position, _tokens.Count - 1)];
    private Token Next() => _tokens[Math.Min(_position++, _tokens.Count - 1)];
}
