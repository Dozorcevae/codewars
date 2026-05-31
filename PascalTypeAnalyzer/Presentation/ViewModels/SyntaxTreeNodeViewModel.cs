using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.ComponentModel;
using PascalTypeAnalyzer.Domain.Syntax;

namespace PascalTypeAnalyzer.Presentation.ViewModels;

/// <summary>
/// Узел дерева для TreeView. Состояние раскрытия хранится во ViewModel,
/// чтобы поведение дерева не зависело от code-behind конкретного окна.
/// </summary>
public sealed partial class SyntaxTreeNodeViewModel : ObservableObject
{
    [ObservableProperty]
    private bool _isExpanded;

    public string Header { get; }

    public ObservableCollection<SyntaxTreeNodeViewModel> Children { get; }

    public SyntaxTreeNodeViewModel(SyntaxNode node)
    {
        Header = BuildHeader(node);
        Children = new ObservableCollection<SyntaxTreeNodeViewModel>(
            node.Children.Select(child => new SyntaxTreeNodeViewModel(child)));
    }

    partial void OnIsExpandedChanged(bool value)
    {
        if (!value)
        {
            CollapseChildrenRecursively();
        }
    }

    /// <summary>
    /// Сворачивает всех потомков. Благодаря этому после повторного раскрытия
    /// родителя пользователь начинает просмотр ветки с понятного состояния.
    /// </summary>
    private void CollapseChildrenRecursively()
    {
        foreach (var child in Children)
        {
            child.IsExpanded = false;
            child.CollapseChildrenRecursively();
        }
    }

    private static string BuildHeader(SyntaxNode node)
    {
        if (node.Name == "TypeDeclaration")
        {
            return BuildTypeDeclarationHeader(node);
        }

        return string.IsNullOrWhiteSpace(node.Value) ? node.Name : $"{node.Name}: {node.Value}";
    }

    private static string BuildTypeDeclarationHeader(SyntaxNode declaration)
    {
        var identifier = declaration.Children.FirstOrDefault(child => child.Name == "Identifier")?.Value;
        var typeSpec = FindTypeSpecAfterEqual(declaration);
        var typeSummary = typeSpec is null ? null : BuildTypeSpecSummary(typeSpec);

        // При частичном дереве после ошибки не пытаемся угадывать подпись:
        // базовый заголовок остается корректным и не вводит пользователя в заблуждение.
        return string.IsNullOrWhiteSpace(identifier) || string.IsNullOrWhiteSpace(typeSummary)
            ? declaration.Name
            : $"{declaration.Name}: {identifier} = {typeSummary}";
    }

    private static SyntaxNode? FindTypeSpecAfterEqual(SyntaxNode declaration)
    {
        var equalIndex = declaration.Children.FindIndex(child => child.Name == "Equal");
        return equalIndex >= 0 && equalIndex + 1 < declaration.Children.Count
            ? declaration.Children[equalIndex + 1]
            : null;
    }

    private static string? BuildTypeSpecSummary(SyntaxNode typeSpec) => typeSpec.Name switch
    {
        "StandardType" => typeSpec.Value,
        "SubrangeType" => BuildSubrangeSummary(typeSpec),
        "EnumType" => "enum",
        "ArrayType" => "array",
        "IdentifierType" => typeSpec.Value,
        "Identifier" => typeSpec.Value,
        _ => null
    };

    private static string? BuildSubrangeSummary(SyntaxNode subrange)
    {
        var boundaries = subrange.Children
            .Where(child => child.Name == "Integer")
            .Select(child => child.Value)
            .Where(value => !string.IsNullOrWhiteSpace(value))
            .ToArray();

        return boundaries.Length == 2 ? $"{boundaries[0]}..{boundaries[1]}" : null;
    }
}
