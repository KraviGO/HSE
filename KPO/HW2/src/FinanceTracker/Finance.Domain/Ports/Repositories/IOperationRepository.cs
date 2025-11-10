using Finance.Domain.Entities;

namespace Finance.Domain.Ports.Repositories;

public interface IOperationRepository : IBaseRepository<Operation>
{
    Task<IEnumerable<Operation>> GetByUserAsync(Guid userId, CancellationToken ct = default);
    Task<IEnumerable<Operation>> GetByAccountAsync(Guid accountId, CancellationToken ct = default);
    Task<IEnumerable<Operation>> GetByPeriodAsync(Guid userId, DateOnly from, DateOnly to, CancellationToken ct = default);
}