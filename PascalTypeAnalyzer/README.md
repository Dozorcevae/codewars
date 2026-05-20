# PascalTypeAnalyzer

Кроссплатформенное desktop-приложение на Avalonia UI для лексического и синтаксического анализа раздела описания типов языка Pascal.

## Технологии
- C# / .NET 8
- Avalonia UI 11
- MVVM
- CommunityToolkit.Mvvm

## Запуск на macOS
```bash
dotnet restore
dotnet run
```

## Корректные примеры
```pascal
type TAge = integer;
type TIndex = 1..10;
type TColor = (red, green, blue);
type TIntArray = array[1..10] of integer;
type TAge = integer; TIndex = 1..10; TColor = (red, green, blue);
```

## Некорректные примеры
```pascal
type = integer;
type TAge integer;
type TRange = 1.10;
type TArr = array[1..10] integer;
```

## Архитектура
- **Domain**: токены, узлы синтаксического дерева, диагностические сообщения.
- **Application**: Lexer, Parser (recursive descent), AnalyzerService и AnalysisResult.
- **Infrastructure**: FileService для Open/Save через StorageProvider.
- **Presentation**: MVVM (ViewModels + Avalonia Views), команды меню/панели инструментов.

## Покрытие требований курсовой
- Реализованы грамматика, лексический и синтаксический анализ.
- Добавлен GUI: меню, панель инструментов, редактор, вкладки результатов, строка статуса.
- Поддержаны команды работы с файлами и запуск анализа.
- Строится синтаксическое дерево и его текстовое представление.
- Ошибки отображаются как понятные диагностические сообщения с координатами.
- Проект кроссплатформенный (macOS/Windows/Linux).
