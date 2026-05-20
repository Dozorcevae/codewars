using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using PascalTypeAnalyzer.Application.Analyzer;
using PascalTypeAnalyzer.Infrastructure.FileSystem;
using PascalTypeAnalyzer.Presentation.ViewModels;
using PascalTypeAnalyzer.Presentation.Views;

namespace PascalTypeAnalyzer;

public partial class App : Avalonia.Application
{
    public override void Initialize() => AvaloniaXamlLoader.Load(this);

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            var analyzerService = new AnalyzerService();
            var fileService = new FileService();
            desktop.MainWindow = new MainWindow
            {
                DataContext = new MainWindowViewModel(analyzerService, fileService)
            };
        }

        base.OnFrameworkInitializationCompleted();
    }
}
