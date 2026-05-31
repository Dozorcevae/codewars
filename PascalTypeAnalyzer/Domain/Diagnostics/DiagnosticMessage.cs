namespace PascalTypeAnalyzer.Domain.Diagnostics;

/// <summary>
/// Понятное пользователю сообщение анализатора с координатами исходного текста.
/// </summary>
public sealed record DiagnosticMessage(
    int Line,
    int Column,
    string Message,
    DiagnosticSeverity Severity = DiagnosticSeverity.Error)
{
    public bool IsError => Severity == DiagnosticSeverity.Error;

    public override string ToString()
        => $"{GetSeverityTitle()} в строке {Line}, столбце {Column}: {Message}";

    private string GetSeverityTitle() => Severity switch
    {
        DiagnosticSeverity.Information => "Инфо",
        DiagnosticSeverity.Warning => "Предупреждение",
        _ => "Ошибка"
    };
}
