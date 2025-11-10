using System.Text.Json;
using CsvHelper;
using CsvHelper.Configuration;
using Finance.Domain.Entities;
using System.Globalization;
using Finance.Domain.Ports.Services;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace Finance.Infrastructure.Io.Export;

public sealed class MultiFormatExportVisitor : IExportVisitor
{
    public async Task VisitAsync(
        User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations,
        string path,
        CancellationToken ct = default)
    {
        if (string.IsNullOrWhiteSpace(path))
            throw new ArgumentException("Export path is required.");

        var ext = Path.GetExtension(path).ToLowerInvariant();
        if (ext is ".json")
        {
            await ExportJsonAsync(user, accounts, categories, operations, path, ct);
            return;
        }
        if (ext is ".yml" or ".yaml")
        {
            await ExportYamlAsync(user, accounts, categories, operations, path, ct);
            return;
        }
        
        await ExportCsvFolderAsync(user, accounts, categories, operations, path, ct);
    }
    
    private sealed class JsonDto
    {
        public string UserEmail { get; set; } = "";
        public string UserName { get; set; } = "";
        public List<AccountDto> Accounts { get; set; } = new();
        public List<CategoryDto> Categories { get; set; } = new();
        public List<OperationDto> Operations { get; set; } = new();
    }
    private sealed class AccountDto { public string Name { get; set; } = ""; public decimal Initial { get; set; } }
    private sealed class CategoryDto { public string Name { get; set; } = ""; public bool IsIncome { get; set; } }
    private sealed class OperationDto
    {
        public string Account { get; set; } = "";
        public string? Category { get; set; }
        public bool IsIncome { get; set; }
        public decimal Amount { get; set; }
        public string Date { get; set; } = "";
        public string? Desc { get; set; }
    }

    private Task ExportJsonAsync(
        User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations,
        string file,
        CancellationToken ct)
    {
        var dto = BuildDto(user, accounts, categories, operations);

        var options = new JsonSerializerOptions
        {
            WriteIndented = true,
            Encoder = System.Text.Encodings.Web.JavaScriptEncoder.UnsafeRelaxedJsonEscaping
        };

        var json = JsonSerializer.Serialize(dto, options);
        return File.WriteAllTextAsync(file, json, ct);
    }
    
    private Task ExportYamlAsync(
        User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations,
        string file,
        CancellationToken ct)
    {
        var dto = BuildDto(user, accounts, categories, operations);
        var serializer = new SerializerBuilder()
            .WithNamingConvention(CamelCaseNamingConvention.Instance)
            .Build();
        var yaml = serializer.Serialize(dto);
        return File.WriteAllTextAsync(file, yaml, ct);
    }

    private static JsonDto BuildDto(
        User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations)
    {
        var accs = accounts.Select(a => new AccountDto { Name = a.Name, Initial = a.Balance }).ToList();
        var cats = categories.Select(c => new CategoryDto { Name = c.Name, IsIncome = c.IsIncome }).ToList();

        var accountsById = accounts.ToDictionary(a => a.Id, a => a.Name);
        var categoriesById = categories.ToDictionary(c => c.Id, c => c.Name);

        var ops = operations.Select(o => new OperationDto
        {
            Account = accountsById.TryGetValue(o.AccountId, out var an) ? an : o.AccountId.ToString(),
            Category = o.CategoryId.HasValue && categoriesById.TryGetValue(o.CategoryId.Value, out var cn) ? cn : null,
            IsIncome = o.Amount >= 0,
            Amount = Math.Abs(o.Amount),
            Date = o.OccurredOn.ToString("yyyy-MM-dd"),
            Desc = o.Description
        }).ToList();

        return new JsonDto
        {
            UserEmail = user.Email,
            UserName = user.Name,
            Accounts = accs,
            Categories = cats,
            Operations = ops
        };
    }
    
    private async Task ExportCsvFolderAsync(
        User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations,
        string folder,
        CancellationToken ct)
    {
        if (!Directory.Exists(folder))
            Directory.CreateDirectory(folder);

        var config = new CsvConfiguration(CultureInfo.InvariantCulture) { HasHeaderRecord = true };
        
        await File.WriteAllTextAsync(Path.Combine(folder, "Users.csv"), $"{user.Id},{user.Email},{user.Name}\n", ct);
        
        using (var writer = new StreamWriter(Path.Combine(folder, "Accounts.csv")))
        using (var csv = new CsvWriter(writer, config))
            await csv.WriteRecordsAsync(accounts, ct);
        
        using (var writer = new StreamWriter(Path.Combine(folder, "Categories.csv")))
        using (var csv = new CsvWriter(writer, config))
            await csv.WriteRecordsAsync(categories, ct);
        
        using (var writer = new StreamWriter(Path.Combine(folder, "Operations.csv")))
        using (var csv = new CsvWriter(writer, config))
        {
            csv.Context.RegisterClassMap<OperationCsvMap>();
            await csv.WriteRecordsAsync(operations, ct);
        }
    }

    private sealed class OperationCsvMap : ClassMap<Operation>
    {
        public OperationCsvMap()
        {
            Map(m => m.Id);
            Map(m => m.UserId);
            Map(m => m.AccountId);
            Map(m => m.CategoryId);
            Map(m => m.Amount);
            Map(m => m.Description);
            Map(m => m.OccurredOn);
            Map(m => m.CreatedAt);
        }
    }
}