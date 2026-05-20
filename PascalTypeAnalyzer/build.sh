#!/bin/bash
set -e

echo "Cleaning project..."
dotnet clean ./PascalTypeAnalyzer.csproj

echo "Restoring packages..."
dotnet restore ./PascalTypeAnalyzer.csproj

echo "Building project..."
dotnet build ./PascalTypeAnalyzer.csproj

echo "Running application..."
dotnet run --project ./PascalTypeAnalyzer.csproj
