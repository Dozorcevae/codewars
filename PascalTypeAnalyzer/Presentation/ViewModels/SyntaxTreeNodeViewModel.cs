using PascalTypeAnalyzer.Domain.Syntax;

namespace PascalTypeAnalyzer.Presentation.ViewModels;

public sealed class SyntaxTreeNodeViewModel
{
    public string Header { get; }
    public List<SyntaxTreeNodeViewModel> Children { get; }

    public SyntaxTreeNodeViewModel(SyntaxNode node)
    {
        Header = string.IsNullOrWhiteSpace(node.Value) ? node.Name : $"{node.Name}: {node.Value}";
        Children = node.Children.Select(c => new SyntaxTreeNodeViewModel(c)).ToList();
    }
}
