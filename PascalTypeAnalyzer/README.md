# PascalTypeAnalyzer

`PascalTypeAnalyzer` — кроссплатформенное desktop-приложение для курсовой работы по дисциплине «Теория формальных языков и компиляторов». Программа выполняет лексический и синтаксический анализ ограниченного подъязыка Pascal: раздела описания типов `type`.

## Возможности

- редактирование исходного текста в многострочном редакторе;
- открытие и сохранение `.pas` / `.txt` файлов;
- лексический анализ с выводом типа, значения и координат каждой лексемы;
- синтаксический анализ методом рекурсивного спуска;
- построение визуального и текстового синтаксического дерева;
- понятные сообщения об ошибках с номером строки и столбца;
- мягкое восстановление после ошибок без падения приложения;
- прокрутка дерева, текстового дерева, сообщений и таблицы лексем;
- кроссплатформенная работа на macOS, Windows и Linux.

## Технологии

- C#;
- .NET 8;
- Avalonia UI 11;
- MVVM;
- CommunityToolkit.Mvvm.

## Запуск и сборка

Перейдите в каталог проекта:

```bash
cd PascalTypeAnalyzer
```

Соберите приложение:

```bash
dotnet restore ./PascalTypeAnalyzer.csproj
dotnet build ./PascalTypeAnalyzer.csproj
```

Запустите приложение:

```bash
dotnet run --project ./PascalTypeAnalyzer.csproj
```

На macOS можно выполнить полный сценарий очистки, сборки и запуска:

```bash
./build.sh
```

## Публикация для macOS Apple Silicon

Для создания self-contained сборки под `osx-arm64` выполните:

```bash
./publish-macos.sh
```

Результат появится в каталоге:

```text
bin/Release/net8.0/osx-arm64/publish/
```

Каталог `bin/` является генерируемым и не добавляется в Git.

## Поддерживаемая грамматика

```ebnf
TypeSection = "type", TypeDeclaration, { TypeDeclaration } ;
TypeDeclaration = Identifier, "=", TypeSpec, ";" ;
TypeSpec = StandardType | SubrangeType | EnumType | ArrayType | Identifier ;
StandardType = "integer" | "real" | "boolean" | "char" | "string" ;
SubrangeType = IntegerNumber, "..", IntegerNumber ;
EnumType = "(", Identifier, { ",", Identifier }, ")" ;
ArrayType = "array", "[", IndexType, "]", "of", TypeSpec ;
IndexType = SubrangeType | Identifier ;
```

Слово `type` указывается один раз в начале раздела. Имена пользовательских типов разрешены.

## Корректные примеры

```pascal
type TAge = integer;
```

```pascal
type TAge = integer;
TIndex = 1..10;
TColor = (red, green, blue);
TIntArray = array[1..10] of integer;
```

```pascal
type TAge = MyCustomType;
```

## Примеры ошибок

```pascal
type TAge = integer
```

Отсутствует завершающая точка с запятой.

```pascal
type TIndex = 1.10;
```

Одиночная точка недопустима: для диапазона требуется `..`.

```pascal
type = integer;
```

После ключевого слова `type` ожидается имя типа.

## Структура проекта

- `Domain` — токены, синтаксические узлы и диагностические сообщения;
- `Application/Lexing` — лексический анализатор;
- `Application/Parsing` — рекурсивный синтаксический анализатор;
- `Application/Analyzer` — единая точка запуска анализа и построение текстового дерева;
- `Infrastructure/FileSystem` — кроссплатформенная работа с файлами;
- `Presentation/ViewModels` — состояние окна и визуального дерева;
- `Presentation/Views` — Avalonia UI.

## Что демонстрируется в курсовой работе

Приложение показывает полный учебный цикл обработки формального языка: выделение лексем, построение синтаксического дерева по грамматике, восстановление после ошибок и представление результата пользователю в графическом интерфейсе.
