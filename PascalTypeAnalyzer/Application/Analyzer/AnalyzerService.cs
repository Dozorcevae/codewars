using System.Text;
using PascalTypeAnalyzer.Application.Lexing;
using PascalTypeAnalyzer.Application.Parsing;
using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Syntax;

namespace PascalTypeAnalyzer.Application.Analyzer;

public sealed class AnalyzerService
{
    private readonly Lexer _lexer = new();

    public AnalysisResult Analyze(string source)
    {
        var (tokens, lexDiagnostics) = _lexer.Lex(source);
        var parser = new Parser(tokens);
        var (tree, parseDiagnostics) = parser.Parse();

        var diagnostics = new List<DiagnosticMessage>();
        diagnostics.AddRange(lexDiagnostics);
        diagnostics.AddRange(parseDiagnostics);

        if (diagnostics.Count == 0)
            diagnostics.Add(new DiagnosticMessage(1, 1, "Анализ выполнен успешно.", false));

        return new AnalysisResult
        {
            IsSuccess = diagnostics.All(d => !d.IsError),
            Tokens = tokens,
            SyntaxTree = tree,
            Diagnostics = diagnostics,
            TreeText = BuildTreeText(tree)
        };
    }

    private static string BuildTreeText(SyntaxNode? root)
    {
        if (root is null) return string.Empty;

        var sb = new StringBuilder();
        Walk(root, 0);
        return sb.ToString();

        void Walk(SyntaxNode node, int depth)
        {
            sb.Append(' ', depth * 2);
            sb.Append(node.Name);
            if (!string.IsNullOrWhiteSpace(node.Value)) sb.Append($": {node.Value}");
            sb.AppendLine();
            foreach (var child in node.Children) Walk(child, depth + 1);
        }
    }
}
