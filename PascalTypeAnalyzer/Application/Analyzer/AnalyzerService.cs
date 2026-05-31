using System.Text;
using PascalTypeAnalyzer.Application.Lexing;
using PascalTypeAnalyzer.Application.Parsing;
using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Syntax;

namespace PascalTypeAnalyzer.Application.Analyzer;

/// <summary>
/// Единая точка запуска анализа: последовательно вызывает лексер и парсер,
/// объединяет диагностику и подготавливает дерево для представления.
/// </summary>
public sealed class AnalyzerService
{
    private readonly Lexer _lexer = new();

    public AnalysisResult Analyze(string source)
    {
        var (tokens, lexerDiagnostics) = _lexer.Lex(source);
        var parser = new Parser(tokens);
        var (tree, parserDiagnostics) = parser.Parse();

        var diagnostics = new List<DiagnosticMessage>();
        diagnostics.AddRange(lexerDiagnostics);
        diagnostics.AddRange(parserDiagnostics);

        if (diagnostics.Count == 0)
        {
            diagnostics.Add(new DiagnosticMessage(
                1,
                1,
                "Анализ выполнен успешно. Ошибок не обнаружено.",
                DiagnosticSeverity.Information));
        }

        return new AnalysisResult
        {
            IsSuccess = diagnostics.All(diagnostic => !diagnostic.IsError),
            Tokens = tokens,
            SyntaxTree = tree,
            Diagnostics = diagnostics,
            TreeText = BuildTreeText(tree)
        };
    }

    private static string BuildTreeText(SyntaxNode? root)
    {
        if (root is null)
        {
            return string.Empty;
        }

        var builder = new StringBuilder();
        AppendNode(root, 0);
        return builder.ToString();

        // Один и тот же SyntaxNode служит источником и для TreeView, и для
        // текстовой вкладки. Поэтому оба представления всегда согласованы.
        void AppendNode(SyntaxNode node, int depth)
        {
            builder.Append(' ', depth * 2);
            builder.Append(node.Name);
            if (!string.IsNullOrWhiteSpace(node.Value))
            {
                builder.Append(": ");
                builder.Append(node.Value);
            }

            builder.AppendLine();
            foreach (var child in node.Children)
            {
                AppendNode(child, depth + 1);
            }
        }
    }
}
