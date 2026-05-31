#!/bin/bash
set -e

echo "Publishing PascalTypeAnalyzer for macOS arm64..."
dotnet publish ./PascalTypeAnalyzer.csproj -c Release -r osx-arm64 --self-contained true

echo "Publish completed."
echo "Output:"
echo "bin/Release/net8.0/osx-arm64/publish/"
