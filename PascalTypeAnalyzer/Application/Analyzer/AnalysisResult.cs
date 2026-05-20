using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Syntax;
using PascalTypeAnalyzer.Domain.Tokens;

namespace PascalTypeAnalyzer.Application.Analyzer;

public sealed class AnalysisResult
{
    public required bool IsSuccess { get; init; }
    public required List<Token> Tokens { get; init; }
    public required SyntaxNode? SyntaxTree { get; init; }
    public required List<DiagnosticMessage> Diagnostics { get; init; }
    public required string TreeText { get; init; }
}
