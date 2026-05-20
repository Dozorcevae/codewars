namespace PascalTypeAnalyzer.Domain.Diagnostics;

public sealed record DiagnosticMessage(int Line, int Column, string Message, bool IsError = true)
{
    public override string ToString() => $"{(IsError ? "Ошибка" : "Инфо")} в строке {Line}, столбце {Column}: {Message}";
}
