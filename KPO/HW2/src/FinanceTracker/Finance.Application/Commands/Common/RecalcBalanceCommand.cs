using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Commands.Common;

public sealed class RecalcBalanceCommand : Finance.Application.Abstractions.ICommand<int>
{
    private readonly IAccountRepository _accRepo;
    private readonly IOperationRepository _opRepo;
    private readonly Guid _userId;

    public RecalcBalanceCommand(
        IAccountRepository accRepo,
        IOperationRepository opRepo,
        Guid userId)
    {
        _accRepo = accRepo;
        _opRepo = opRepo;
        _userId = userId;
    }

    public async Task<int> Execute(CancellationToken ct = default)
    {
        var accounts = (await _accRepo.GetByUserAsync(_userId, ct)).ToList();
        
        var ops = (await _opRepo.GetByPeriodAsync(
            _userId,
            DateOnly.MinValue,
            DateOnly.MaxValue,
            ct)).ToList();
        
        var byAcc = ops.GroupBy(o => o.AccountId)
                       .ToDictionary(g => g.Key, g => g.Sum(o => o.Amount));

        var fixedCount = 0;

        foreach (var acc in accounts)
        {
            var total = byAcc.TryGetValue(acc.Id, out var sum) ? sum : 0m;

            if (acc.Balance != total)
            {
                acc.Balance = total;
                await _accRepo.UpdateAsync(acc, ct);
                fixedCount++;
            }
        }

        await _accRepo.SaveChangesAsync(ct);

        return fixedCount;
    }
}
