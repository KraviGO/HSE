using System.Text.Json;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Infrastructure.Io.Import;

public sealed class JsonImporter : IImportService
{
    private readonly IUserRepository _users;
    private readonly IAccountRepository _accounts;
    private readonly ICategoryRepository _categories;
    private readonly IOperationRepository _operations;

    public JsonImporter(
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

    public async Task ImportAsync(string path, CancellationToken ct = default)
    {
        var raw = await File.ReadAllTextAsync(path, ct);
        var dto = JsonSerializer.Deserialize<JsonDto>(raw, new JsonSerializerOptions
        {
            PropertyNameCaseInsensitive = true
        }) ?? throw new InvalidOperationException("Invalid JSON.");

        // --- user ---
        var user = await _users.GetByEmailAsync(dto.UserEmail, ct);
        if (user is null)
        {
            user = new User { Id = Guid.NewGuid(), Email = dto.UserEmail, Name = dto.UserName };
            await _users.AddAsync(user, ct);
            await _users.SaveChangesAsync(ct);
        }

        // --- accounts ---
        var accountsMap = new Dictionary<string, Guid>(StringComparer.OrdinalIgnoreCase);
        foreach (var a in dto.Accounts)
        {
            var existing = await _accounts.GetByUserAsync(user.Id, ct);
            var account = existing.FirstOrDefault(x => x.Name.Equals(a.Name, StringComparison.OrdinalIgnoreCase));
            if (account is null)
            {
                account = new BankAccount { Id = Guid.NewGuid(), UserId = user.Id, Name = a.Name, Balance = a.Initial };
                await _accounts.AddAsync(account, ct);
            }
            accountsMap[a.Name] = account.Id;
        }
        await _accounts.SaveChangesAsync(ct);

        // --- categories ---
        var categoriesMap = new Dictionary<string, Guid>(StringComparer.OrdinalIgnoreCase);
        foreach (var c in dto.Categories)
        {
            var existing = await _categories.GetByUserAsync(user.Id, ct);
            var cat = existing.FirstOrDefault(x => x.Name.Equals(c.Name, StringComparison.OrdinalIgnoreCase));
            if (cat is null)
            {
                cat = new Category { Id = Guid.NewGuid(), UserId = user.Id, Name = c.Name, IsIncome = c.IsIncome };
                await _categories.AddAsync(cat, ct);
            }
            categoriesMap[c.Name] = cat.Id;
        }
        await _categories.SaveChangesAsync(ct);

        // --- operations ---
        foreach (var o in dto.Operations)
        {
            if (!accountsMap.TryGetValue(o.Account, out var accountId))
                throw new InvalidOperationException($"Account '{o.Account}' not found.");

            Guid? categoryId = null;
            if (!string.IsNullOrWhiteSpace(o.Category))
            {
                if (!categoriesMap.TryGetValue(o.Category!, out var cid))
                    throw new InvalidOperationException($"Category '{o.Category}' not found.");
                categoryId = cid;
            }

            var signed = o.IsIncome ? Math.Abs(o.Amount) : -Math.Abs(o.Amount);
            var op = new Operation
            {
                Id = Guid.NewGuid(),
                UserId = user.Id,
                AccountId = accountId,
                CategoryId = categoryId,
                Amount = signed,
                Description = o.Desc,
                OccurredOn = DateOnly.Parse(o.Date),
                CreatedAt = DateTime.UtcNow
            };
            await _operations.AddAsync(op, ct);
            
            var acc = await _accounts.GetByIdAsync(accountId, ct) ?? throw new InvalidOperationException("Account not found");
            acc.Balance += signed;
            await _accounts.UpdateAsync(acc, ct);
        }
        await _operations.SaveChangesAsync(ct);
        await _accounts.SaveChangesAsync(ct);
    }
}