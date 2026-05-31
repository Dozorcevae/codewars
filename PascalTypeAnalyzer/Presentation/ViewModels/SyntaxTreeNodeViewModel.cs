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
        Header = string.IsNullOrWhiteSpace(node.Value) ? node.Name : $"{node.Name}: {node.Value}";
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
}
