using Finance.Domain.Entities;

namespace Finance.Domain.Ports.Repositories;

public interface IGoalRepository : IBaseRepository<FinancialGoal>
{
    Task<IEnumerable<FinancialGoal>> GetByUserAsync(Guid userId, CancellationToken ct = default);
    Task<FinancialGoal?> GetByAccountIdAsync(Guid accountId, CancellationToken ct = default);
}
