using System.Collections.ObjectModel;
using System.Windows.Input;
using Avalonia.Controls;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using PascalTypeAnalyzer.Application.Analyzer;
using PascalTypeAnalyzer.Domain.Diagnostics;
using PascalTypeAnalyzer.Domain.Tokens;
using PascalTypeAnalyzer.Infrastructure.FileSystem;

namespace PascalTypeAnalyzer.Presentation.ViewModels;

public partial class MainWindowViewModel : ViewModelBase
{
    private readonly AnalyzerService _analyzer;
    private readonly FileService _fileService;

    [ObservableProperty] private string _sourceText = "type TAge = integer;\nTIndex = 1..10;\nTColor = (red, green, blue);\nTIntArray = array[1..10] of integer;";
    [ObservableProperty] private string _treeText = string.Empty;
    [ObservableProperty] private string _status = "Готов к анализу";
    [ObservableProperty] private string _currentFilePath = "Новый файл";
    [ObservableProperty] private int _errorCount;

    public ObservableCollection<SyntaxTreeNodeViewModel> SyntaxTreeNodes { get; } = new();
    public ObservableCollection<DiagnosticMessage> Diagnostics { get; } = new();
    public ObservableCollection<Token> Tokens { get; } = new();

    public ICommand NewCommand { get; }
    public ICommand OpenCommand { get; }
    public ICommand SaveCommand { get; }
    public ICommand SaveAsCommand { get; }
    public ICommand AnalyzeCommand { get; }
    public ICommand HelpCommand { get; }
    public ICommand AboutCommand { get; }
    public ICommand ExitCommand { get; }
    public ICommand UndoCommand { get; }
    public ICommand CutCommand { get; }
    public ICommand CopyCommand { get; }
    public ICommand PasteCommand { get; }
    public ICommand SelectAllCommand { get; }

    public MainWindowViewModel(AnalyzerService analyzer, FileService fileService)
    {
        _analyzer = analyzer;
        _fileService = fileService;

        NewCommand = new RelayCommand(NewFile);
        OpenCommand = new AsyncRelayCommand<Window?>(OpenFileAsync);
        SaveCommand = new AsyncRelayCommand<Window?>(SaveFileAsync);
        SaveAsCommand = new AsyncRelayCommand<Window?>(SaveFileAsAsync);
        AnalyzeCommand = new RelayCommand(RunAnalysis);
        HelpCommand = new AsyncRelayCommand<Window?>(ShowHelpAsync);
        AboutCommand = new AsyncRelayCommand<Window?>(ShowAboutAsync);
        ExitCommand = new RelayCommand<Window?>(w => w?.Close());
        UndoCommand = new RelayCommand(() => Status = "Отмена доступна через системные сочетания клавиш.");
        CutCommand = new RelayCommand(() => Status = "Вырезать: используйте Cmd/Ctrl+X.");
        CopyCommand = new RelayCommand(() => Status = "Копировать: используйте Cmd/Ctrl+C.");
        PasteCommand = new RelayCommand(() => Status = "Вставить: используйте Cmd/Ctrl+V.");
        SelectAllCommand = new RelayCommand(() => Status = "Выделить все: используйте Cmd/Ctrl+A.");
    }

    private void NewFile()
    {
        SourceText = string.Empty;
        CurrentFilePath = "Новый файл";
        Status = "Создан новый файл";
    }

    private async Task OpenFileAsync(Window? window)
    {
        if (window?.StorageProvider is null) return;
        var result = await _fileService.OpenFileAsync(window.StorageProvider);
        if (result.Content is null) return;

        SourceText = result.Content;
        CurrentFilePath = result.Path ?? "Новый файл";
        Status = "Файл открыт";
    }

    private async Task SaveFileAsync(Window? window)
    {
        if (CurrentFilePath == "Новый файл")
        {
            await SaveFileAsAsync(window);
            return;
        }

        await _fileService.SaveToPathAsync(CurrentFilePath, SourceText);
        Status = "Файл сохранен";
    }

    private async Task SaveFileAsAsync(Window? window)
    {
        if (window?.StorageProvider is null) return;
        var path = await _fileService.SaveFileAsAsync(window.StorageProvider, SourceText);
        if (path is null) return;
        CurrentFilePath = path;
        Status = "Файл сохранен";
    }

    private void RunAnalysis()
    {
        var result = _analyzer.Analyze(SourceText);

        Tokens.Clear();
        foreach (var token in result.Tokens) Tokens.Add(token);

        Diagnostics.Clear();
        foreach (var d in result.Diagnostics) Diagnostics.Add(d);

        SyntaxTreeNodes.Clear();
        if (result.SyntaxTree is not null) SyntaxTreeNodes.Add(new SyntaxTreeNodeViewModel(result.SyntaxTree));

        TreeText = result.TreeText;
        ErrorCount = result.Diagnostics.Count(x => x.IsError);
        Status = result.IsSuccess ? "Анализ завершен успешно" : "Анализ завершен с ошибками";
    }

    private static async Task ShowHelpAsync(Window? owner)
    {
        if (owner is null) return;
        var text = "Введите раздел type языка Pascal и нажмите 'Запустить анализ'.\n\nПример:\ntype TAge = integer;\nTIndex = 1..10;";
        await ShowDialog(owner, "Руководство пользователя", text);
    }

    private static async Task ShowAboutAsync(Window? owner)
    {
        if (owner is null) return;
        await ShowDialog(owner, "О программе", "PascalTypeAnalyzer\nКурсовой проект по разработке языкового процессора.");
    }

    private static async Task ShowDialog(Window owner, string title, string message)
    {
        var dlg = new Window
        {
            Title = title,
            Width = 480,
            Height = 240,
            Content = new TextBlock { Text = message, TextWrapping = Avalonia.Media.TextWrapping.Wrap, Margin = new Thickness(16) }
        };

        await dlg.ShowDialog(owner);
    }
}
