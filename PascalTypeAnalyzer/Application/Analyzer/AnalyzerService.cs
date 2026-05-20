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

        AddDidYouMeanHint(source, diagnostics);

        if (diagnostics.Count == 0)
            diagnostics.Add(new DiagnosticMessage(1, 1, "Анализ выполнен успешно. Ошибок не обнаружено.", false));

        return new AnalysisResult
        {
            IsSuccess = diagnostics.All(d => !d.IsError),
            Tokens = tokens,
            SyntaxTree = tree,
            Diagnostics = diagnostics,
            TreeText = BuildTreeText(tree)
        };
    }


    private static void AddDidYouMeanHint(string source, List<DiagnosticMessage> diagnostics)
    {
        if (!diagnostics.Any(d => d.IsError && d.Message.Contains("символом ';'", StringComparison.OrdinalIgnoreCase)))
        {
            return;
        }

        var words = source.Split(new[] { ' ', '\t', '\r', '\n', ';', '=', '(', ')', '[', ']', ',' }, StringSplitOptions.RemoveEmptyEntries);
        var standard = new[] { "integer", "real", "boolean", "char", "string" };

        foreach (var w in words)
        {
            if (standard.Any(x => x.Equals(w, StringComparison.OrdinalIgnoreCase)))
            {
                continue;
            }

            var near = standard.FirstOrDefault(x => Levenshtein(w.ToLowerInvariant(), x) == 1);
            if (near is null)
            {
                continue;
            }

            var hasHint = diagnostics.Any(d => !d.IsError && d.Message.Contains("проверьте его написание", StringComparison.OrdinalIgnoreCase));
            if (!hasHint)
            {
                diagnostics.Add(new DiagnosticMessage(1, 1, $"Описание типа должно заканчиваться символом ';'. Если вы хотели указать стандартный тип {near}, проверьте его написание.", false));
            }

            break;
        }
    }

    private static int Levenshtein(string a, string b)
    {
        var d = new int[a.Length + 1, b.Length + 1];
        for (var i = 0; i <= a.Length; i++) d[i, 0] = i;
        for (var j = 0; j <= b.Length; j++) d[0, j] = j;

        for (var i = 1; i <= a.Length; i++)
        {
            for (var j = 1; j <= b.Length; j++)
            {
                var cost = a[i - 1] == b[j - 1] ? 0 : 1;
                d[i, j] = Math.Min(Math.Min(d[i - 1, j] + 1, d[i, j - 1] + 1), d[i - 1, j - 1] + cost);
            }
        }

        return d[a.Length, b.Length];
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
