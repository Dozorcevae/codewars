using System.Text;
using Avalonia.Platform.Storage;

namespace PascalTypeAnalyzer.Infrastructure.FileSystem;

public sealed class FileService
{
    public async Task<(string? Path, string? Content)> OpenFileAsync(IStorageProvider storageProvider)
    {
        var files = await storageProvider.OpenFilePickerAsync(new FilePickerOpenOptions
        {
            Title = "Открыть Pascal-файл",
            AllowMultiple = false,
            FileTypeFilter = new[] { new FilePickerFileType("Текст") { Patterns = new[] { "*.txt", "*.pas" } } }
        });

        var file = files.FirstOrDefault();
        if (file is null) return (null, null);

        await using var stream = await file.OpenReadAsync();
        using var reader = new StreamReader(stream, Encoding.UTF8);
        return (file.Path.LocalPath, await reader.ReadToEndAsync());
    }

    public async Task<string?> SaveFileAsAsync(IStorageProvider storageProvider, string content)
    {
        var file = await storageProvider.SaveFilePickerAsync(new FilePickerSaveOptions
        {
            Title = "Сохранить файл",
            SuggestedFileName = "types.pas",
            FileTypeChoices = new[] { new FilePickerFileType("Pascal") { Patterns = new[] { "*.pas", "*.txt" } } }
        });

        if (file is null) return null;
        await using var stream = await file.OpenWriteAsync();
        stream.SetLength(0);
        await using var writer = new StreamWriter(stream, Encoding.UTF8);
        await writer.WriteAsync(content);
        return file.Path.LocalPath;
    }

    public async Task SaveToPathAsync(string path, string content)
        => await File.WriteAllTextAsync(path, content, Encoding.UTF8);
}
