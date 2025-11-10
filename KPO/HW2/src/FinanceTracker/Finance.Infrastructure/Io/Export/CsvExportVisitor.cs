using System.Globalization;
using CsvHelper;
using CsvHelper.Configuration;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Services;

namespace Finance.Infrastructure.Io.Export;

public sealed class CsvExportVisitor : IExportVisitor
{
    public async Task VisitAsync(
        User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations,
        string folder,
        CancellationToken ct = default)
    {
        Directory.CreateDirectory(folder);

        var config = new CsvConfiguration(CultureInfo.InvariantCulture)
        {
            HasHeaderRecord = true
        };
        
        await using (var writer = new StreamWriter(Path.Combine(folder, "Users.csv")))
        await using (var csv = new CsvWriter(writer, config))
            await csv.WriteRecordsAsync(new[] { user }, ct);

        await using (var writer = new StreamWriter(Path.Combine(folder, "Accounts.csv")))
        await using (var csv = new CsvWriter(writer, config))
            await csv.WriteRecordsAsync(accounts, ct);
        
        await using (var writer = new StreamWriter(Path.Combine(folder, "Categories.csv")))
        await using (var csv = new CsvWriter(writer, config))
            await csv.WriteRecordsAsync(categories, ct);
        
        var opsToExport = operations.Select(o => new OperationCsvModel
        {
            Id = o.Id,
            UserId = o.UserId,
            AccountId = o.AccountId,
            CategoryId = o.CategoryId,
            Amount = o.Amount,
            Description = o.Description,
            OccurredOn = o.OccurredOn.ToString("yyyy-MM-dd"),
            CreatedAt = o.CreatedAt.ToString("yyyy-MM-dd HH:mm:ss")
        });

        await using (var writer = new StreamWriter(Path.Combine(folder, "Operations.csv")))
        await using (var csv = new CsvWriter(writer, config))
            await csv.WriteRecordsAsync(opsToExport, ct);
    }

    private sealed class OperationCsvModel
    {
        public Guid Id { get; set; }
        public Guid UserId { get; set; }
        public Guid AccountId { get; set; }
        public Guid? CategoryId { get; set; }
        public decimal Amount { get; set; }
        public string? Description { get; set; }
        public string OccurredOn { get; set; } = "";
        public string CreatedAt { get; set; } = "";
    }
}