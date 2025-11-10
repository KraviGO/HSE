using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using IAccountRepository = Finance.Domain.Ports.Repositories.IAccountRepository;

namespace Finance.Application.Services;

public class TransferService
{
    private readonly IAccountRepository _accounts;
    private readonly IOperationRepository _operations;
    private readonly IGoalRepository _goals;

    public TransferService(
        IAccountRepository accounts,
        IOperationRepository operations,
        IGoalRepository goals)
    {
        _accounts = accounts;
        _operations = operations;
        _goals = goals;
    }

    public async Task TransferAsync(Guid fromId, Guid toId, decimal amount, Guid userId, DateOnly? date = null, string? note = null)
    {
        if (fromId == toId)
            throw new ArgumentException("Нельзя переводить на тот же самый счёт.");
        if (amount <= 0)
            throw new ArgumentException("Сумма перевода должна быть положительной и больше нуля.");

        var from = await _accounts.GetByIdAsync(fromId) ?? throw new InvalidOperationException("Счёт-источник не найден.");
        var to   = await _accounts.GetByIdAsync(toId)   ?? throw new InvalidOperationException("Счёт-получатель не найден.");
        if (from.UserId != userId || to.UserId != userId)
            throw new InvalidOperationException("Счета принадлежат другому пользователю.");

        if (from.Balance < amount)
            throw new InvalidOperationException("Недостаточно средств на счёте-источнике.");

        var occurred = date ?? DateOnly.FromDateTime(DateTime.Now);
        
        var outOp = new Operation
        {
            UserId = userId,
            AccountId = from.Id,
            Amount = -amount,
            Description = note ?? $"Перевод на {to.Name}",
            OccurredOn = occurred,
            CreatedAt = DateTime.UtcNow
        };

        var inOp = new Operation
        {
            UserId = userId,
            AccountId = to.Id,
            Amount = amount,
            Description = note ?? $"Перевод с {from.Name}",
            OccurredOn = occurred,
            CreatedAt = DateTime.UtcNow
        };
        
        from.Balance -= amount;
        to.Balance   += amount;
        
        var fromGoal = await _goals.GetByAccountIdAsync(from.Id);
        var toGoal   = await _goals.GetByAccountIdAsync(to.Id);

        if (fromGoal is not null)
        {
            if (fromGoal.CurrentAmount < amount)
                throw new InvalidOperationException("Нельзя списать больше, чем накоплено по цели.");
            fromGoal.CurrentAmount -= amount;
            await _goals.UpdateAsync(fromGoal);
        }
        if (toGoal is not null)
        {
            toGoal.CurrentAmount += amount;
            await _goals.UpdateAsync(toGoal);
        }

        await _operations.AddAsync(outOp);
        await _operations.AddAsync(inOp);
        await _accounts.UpdateAsync(from);
        await _accounts.UpdateAsync(to);

        await _operations.SaveChangesAsync();
        await _accounts.SaveChangesAsync();
        await _goals.SaveChangesAsync();
    }
}
