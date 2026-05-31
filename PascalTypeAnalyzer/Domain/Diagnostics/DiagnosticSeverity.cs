namespace PascalTypeAnalyzer.Domain.Diagnostics;

/// <summary>
/// Уровень диагностического сообщения. Ошибки влияют на результат анализа,
/// а информационные сообщения помогают пользователю понять результат.
/// </summary>
public enum DiagnosticSeverity
{
    Information,
    Warning,
    Error
}
