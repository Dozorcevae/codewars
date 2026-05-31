using PascalTypeAnalyzer.Domain.Tokens;

namespace PascalTypeAnalyzer.Domain.Syntax;

/// <summary>
/// Узел синтаксического дерева. Терминальные узлы дополнительно хранят токен,
/// чтобы дерево можно было связать с конкретным местом исходного текста.
/// </summary>
public sealed class SyntaxNode
{
    public string Name { get; }
    public string? Value { get; }
    public Token? Token { get; }
    public List<SyntaxNode> Children { get; } = new();

    public SyntaxNode(string name, string? value = null, Token? token = null)
    {
        Name = name;
        Value = value;
        Token = token;
    }

    /// <summary>Добавляет дочерний узел в порядке, заданном грамматикой.</summary>
    public void Add(SyntaxNode child) => Children.Add(child);
}
