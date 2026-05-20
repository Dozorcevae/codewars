using PascalTypeAnalyzer.Domain.Tokens;

namespace PascalTypeAnalyzer.Domain.Syntax;

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

    public void Add(SyntaxNode child) => Children.Add(child);
}
