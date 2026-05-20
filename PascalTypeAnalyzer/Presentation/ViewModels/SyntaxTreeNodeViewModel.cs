using CommunityToolkit.Mvvm.ComponentModel;
using PascalTypeAnalyzer.Domain.Syntax;

namespace PascalTypeAnalyzer.Presentation.ViewModels;

public sealed partial class SyntaxTreeNodeViewModel : ObservableObject
{
    [ObservableProperty] private bool _isExpanded;

    public string Header { get; }
    public List<SyntaxTreeNodeViewModel> Children { get; }

    public SyntaxTreeNodeViewModel(SyntaxNode node)
    {
        Header = string.IsNullOrWhiteSpace(node.Value) ? node.Name : $"{node.Name}: {node.Value}";
        Children = node.Children.Select(c => new SyntaxTreeNodeViewModel(c)).ToList();
    }

    partial void OnIsExpandedChanged(bool value)
    {
        if (!value)
        {
            CollapseRecursively();
        }
    }

    public void CollapseRecursively()
    {
        foreach (var child in Children)
        {
            child.IsExpanded = false;
            child.CollapseRecursively();
        }
    }
}
