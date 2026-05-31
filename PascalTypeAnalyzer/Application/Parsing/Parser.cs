using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Syntax;
using PascalTypeAnalyzer.Domain.Tokens;

namespace PascalTypeAnalyzer.Application.Parsing;

/// <summary>
/// Синтаксический анализатор раздела type. Каждый метод Parse* соответствует
/// одному нетерминалу грамматики и строит свою часть синтаксического дерева.
/// </summary>
public sealed class Parser
{
    private static readonly string[] StandardTypes = ["integer", "real", "boolean", "char", "string"];

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
        if (!Match(TokenType.TypeKeyword, "в начале раздела ожидается ключевое слово \"type\".", out var typeToken))
        {
            RecoverToDeclarationBoundary();
            return node;
        }

        node.Add(Terminal("TypeKeyword", typeToken));

        if (Current.Type == TokenType.EndOfInput)
        {
            AddError(Current, "после ключевого слова \"type\" ожидается имя типа.");
            return node;
        }

        while (Current.Type != TokenType.EndOfInput)
        {
            // Восстановление после ошибки обязано продвинуться по потоку токенов.
            // Частично разобранное объявление сохраняем в дереве до синхронизации:
            // пользователь увидит уже распознанную часть конструкции.
            var positionBeforeDeclaration = _position;
            var diagnosticsBeforeDeclaration = _diagnostics.Count;
            var declaration = ParseTypeDeclaration();
            if (declaration is not null)
            {
                node.Add(declaration);
            }

            if (_diagnostics.Count == diagnosticsBeforeDeclaration)
            {
                continue;
            }

            RecoverToDeclarationBoundary();
            if (_position == positionBeforeDeclaration && Current.Type != TokenType.EndOfInput)
            {
                Next();
            }
        }

        return node;
    }

    public SyntaxNode? ParseTypeDeclaration()
    {
        var node = new SyntaxNode("TypeDeclaration");
        if (!Match(TokenType.Identifier, "после ключевого слова \"type\" ожидается имя типа.", out var identifier))
        {
            return null;
        }

        node.Add(Terminal("Identifier", identifier));

        if (!Match(TokenType.Equal, "после имени типа ожидается символ \"=\".", out var equal))
        {
            return node;
        }

        node.Add(Terminal("Equal", equal));

        var diagnosticsBeforeTypeSpec = _diagnostics.Count;
        var typeSpec = ParseTypeSpec();
        if (typeSpec is null)
        {
            return node;
        }

        node.Add(typeSpec);
        if (_diagnostics.Count > diagnosticsBeforeTypeSpec)
        {
            return node;
        }

        if (!Match(TokenType.Semicolon, BuildMissingSemicolonMessage(typeSpec), out var semicolon))
        {
            return node;
        }

        node.Add(Terminal("Semicolon", semicolon));
        return node;
    }

    public SyntaxNode? ParseTypeSpec() => Current.Type switch
    {
        TokenType.StandardTypeKeyword => ParseStandardType(),
        TokenType.IntegerNumber => ParseSubrangeType(),
        TokenType.LeftParen => ParseEnumType(),
        TokenType.ArrayKeyword => ParseArrayType(),
        TokenType.Identifier => ParseUserDefinedType(),
        _ => ErrorNode("ожидается описание типа: стандартный тип, диапазон, перечисление, массив или имя пользовательского типа.")
    };

    public SyntaxNode ParseStandardType()
    {
        var token = Next();
        return new SyntaxNode("StandardType", token.Value, token);
    }

    public SyntaxNode? ParseSubrangeType()
    {
        var node = new SyntaxNode("SubrangeType");
        if (!Match(TokenType.IntegerNumber, "в интервальном типе ожидается левая целая граница.", out var left))
        {
            return null;
        }

        node.Add(Terminal("Integer", left));

        if (!Match(TokenType.Range, "в интервальном типе ожидается знак диапазона \"..\".", out var range))
        {
            return node;
        }

        node.Add(Terminal("Range", range));

        if (!Match(TokenType.IntegerNumber, "в интервальном типе ожидается правая целая граница.", out var right))
        {
            return node;
        }

        node.Add(Terminal("Integer", right));
        return node;
    }

    public SyntaxNode? ParseEnumType()
    {
        var node = new SyntaxNode("EnumType");
        if (!Match(TokenType.LeftParen, "для перечислимого типа ожидается символ \"(\".", out var leftParen))
        {
            return null;
        }

        node.Add(Terminal("LeftParen", leftParen));

        if (!Match(TokenType.Identifier, "в перечислимом типе ожидается имя элемента.", out var identifier))
        {
            return node;
        }

        node.Add(Terminal("Identifier", identifier));

        while (Current.Type == TokenType.Comma)
        {
            node.Add(Terminal("Comma", Next()));
            if (!Match(TokenType.Identifier, "после запятой ожидается имя элемента перечисления.", out var nextIdentifier))
            {
                return node;
            }

            node.Add(Terminal("Identifier", nextIdentifier));
        }

        if (!Match(TokenType.RightParen, "в конце перечислимого типа ожидается символ \")\".", out var rightParen))
        {
            return node;
        }

        node.Add(Terminal("RightParen", rightParen));
        return node;
    }

    public SyntaxNode? ParseArrayType()
    {
        var node = new SyntaxNode("ArrayType");
        node.Add(Terminal("ArrayKeyword", Next()));

        if (!Match(TokenType.LeftBracket, "после ключевого слова \"array\" ожидается символ \"[\".", out var leftBracket))
        {
            return node;
        }

        node.Add(Terminal("LeftBracket", leftBracket));

        var diagnosticsBeforeIndexType = _diagnostics.Count;
        var indexType = ParseIndexType();
        if (indexType is null)
        {
            return node;
        }

        node.Add(indexType);
        if (_diagnostics.Count > diagnosticsBeforeIndexType)
        {
            return node;
        }

        if (!Match(TokenType.RightBracket, "после типа индекса ожидается символ \"]\".", out var rightBracket))
        {
            return node;
        }

        node.Add(Terminal("RightBracket", rightBracket));

        if (!Match(TokenType.OfKeyword, "после символа \"]\" ожидается ключевое слово \"of\".", out var ofKeyword))
        {
            return node;
        }

        node.Add(Terminal("OfKeyword", ofKeyword));

        var diagnosticsBeforeElementType = _diagnostics.Count;
        var elementType = ParseTypeSpec();
        if (elementType is null)
        {
            return node;
        }

        node.Add(elementType);
        if (_diagnostics.Count > diagnosticsBeforeElementType)
        {
            return node;
        }
        return node;
    }

    public SyntaxNode? ParseIndexType() => Current.Type switch
    {
        TokenType.IntegerNumber => ParseSubrangeType(),
        TokenType.Identifier => ParseUserDefinedType(),
        _ => ErrorNode("в массиве ожидается индексный тип: диапазон или имя пользовательского типа.")
    };

    private SyntaxNode ParseUserDefinedType()
    {
        var identifier = Next();
        return new SyntaxNode("IdentifierType", identifier.Value, identifier);
    }

    private bool Match(TokenType type, string message, out Token token)
    {
        token = Current;
        if (Current.Type != type)
        {
            AddError(Current, message);
            return false;
        }

        token = Next();
        return true;
    }

    private SyntaxNode? ErrorNode(string message)
    {
        AddError(Current, message);
        return null;
    }

    private void AddError(Token token, string message)
        => _diagnostics.Add(new DiagnosticMessage(token.Line, token.Column, message));

    private static SyntaxNode Terminal(string name, Token token) => new(name, token.Value, token);

    private static string BuildMissingSemicolonMessage(SyntaxNode typeSpec)
    {
        const string message = "описание типа должно заканчиваться символом \";\".";
        if (typeSpec.Name != "IdentifierType" || string.IsNullOrWhiteSpace(typeSpec.Value))
        {
            return message;
        }

        var standardType = StandardTypes.FirstOrDefault(candidate =>
            CalculateLevenshteinDistance(typeSpec.Value, candidate) == 1);

        return standardType is null
            ? message
            : $"{message} Если вы хотели указать стандартный тип \"{standardType}\", проверьте его написание.";
    }

    private static int CalculateLevenshteinDistance(string left, string right)
    {
        var distances = new int[left.Length + 1, right.Length + 1];
        for (var i = 0; i <= left.Length; i++) distances[i, 0] = i;
        for (var j = 0; j <= right.Length; j++) distances[0, j] = j;

        for (var i = 1; i <= left.Length; i++)
        {
            for (var j = 1; j <= right.Length; j++)
            {
                var substitutionCost = char.ToLowerInvariant(left[i - 1]) == char.ToLowerInvariant(right[j - 1]) ? 0 : 1;
                distances[i, j] = Math.Min(
                    Math.Min(distances[i - 1, j] + 1, distances[i, j - 1] + 1),
                    distances[i - 1, j - 1] + substitutionCost);
            }
        }

        return distances[left.Length, right.Length];
    }

    private void RecoverToDeclarationBoundary()
    {
        // Точка с запятой завершает ошибочное объявление. Повторное слово type
        // также поглощаем: оно начинает новый раздел и не должно зациклить парсер.
        while (Current.Type is not TokenType.Semicolon and not TokenType.TypeKeyword and not TokenType.EndOfInput)
        {
            Next();
        }

        if (Current.Type is TokenType.Semicolon or TokenType.TypeKeyword)
        {
            Next();
        }
    }

    private Token Current => _tokens[Math.Min(_position, _tokens.Count - 1)];

    private Token Next() => _tokens[Math.Min(_position++, _tokens.Count - 1)];
}
