using System.Globalization;
using CsvHelper;
using CsvHelper.Configuration;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Infrastructure.Io.Import;

public sealed class CsvImporter : IImportService
{
    private readonly IUserRepository _users;
    private readonly IAccountRepository _accounts;
    private readonly ICategoryRepository _categories;
    private readonly IOperationRepository _operations;

    public CsvImporter(
        IUserRepository users,
        IAccountRepository accounts,
        ICategoryRepository categories,
        IOperationRepository operations)
    {
        _users = users;
        _accounts = accounts;
        _categories = categories;
        _operations = operations;
    }

    public async Task ImportAsync(string folder, CancellationToken ct = default)
    {
        var config = new CsvConfiguration(CultureInfo.InvariantCulture)
        {
            HasHeaderRecord = true,
            MissingFieldFound = null,
            HeaderValidated = null
        };
        
        var usersPath = Path.Combine(folder, "Users.csv");
        if (File.Exists(usersPath))
        {
            using var reader = new StreamReader(usersPath);
            using var csv = new CsvReader(reader, config);
            var records = csv.GetRecords<User>().ToList();
            foreach (var u in records)
            {
                if (await _users.GetByIdAsync(u.Id, ct) is null)
                    await _users.AddAsync(u, ct);
            }
            await _users.SaveChangesAsync(ct);
        }
        
        var accountsPath = Path.Combine(folder, "Accounts.csv");
        if (File.Exists(accountsPath))
        {
            using var reader = new StreamReader(accountsPath);
            using var csv = new CsvReader(reader, config);
            var records = csv.GetRecords<BankAccount>().ToList();
            foreach (var a in records)
            {
                if (await _accounts.GetByIdAsync(a.Id, ct) is null)
                    await _accounts.AddAsync(a, ct);
            }
            await _accounts.SaveChangesAsync(ct);
        }
        
        var catPath = Path.Combine(folder, "Categories.csv");
        if (File.Exists(catPath))
        {
            using var reader = new StreamReader(catPath);
            using var csv = new CsvReader(reader, config);
            var records = csv.GetRecords<Category>().ToList();
            foreach (var c in records)
            {
                if (await _categories.GetByIdAsync(c.Id, ct) is null)
                    await _categories.AddAsync(c, ct);
            }
            await _categories.SaveChangesAsync(ct);
        }
        
        var opPath = Path.Combine(folder, "Operations.csv");
        if (File.Exists(opPath))
        {
            using var reader = new StreamReader(opPath);
            using var csv = new CsvReader(reader, config);
            var ops = csv.GetRecords<OperationCsvModel>().ToList();

            foreach (var x in ops)
            {
                if (await _operations.GetByIdAsync(x.Id, ct) is not null)
                    continue;

                var op = new Operation
                {
                    Id = x.Id,
                    UserId = x.UserId,
                    AccountId = x.AccountId,
                    CategoryId = x.CategoryId,
                    Amount = x.Amount,
                    Description = x.Description,
                    OccurredOn = DateOnly.ParseExact(x.OccurredOn, "yyyy-MM-dd", CultureInfo.InvariantCulture),
                    CreatedAt = DateTime.ParseExact(x.CreatedAt, "yyyy-MM-dd HH:mm:ss", CultureInfo.InvariantCulture)
                };

                await _operations.AddAsync(op, ct);
            }

            await _operations.SaveChangesAsync(ct);
        }
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