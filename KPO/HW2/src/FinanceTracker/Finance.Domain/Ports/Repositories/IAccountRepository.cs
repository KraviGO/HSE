using Finance.Domain.Entities;

namespace Finance.Domain.Ports.Repositories;

public interface IAccountRepository : IBaseRepository<BankAccount>
{
    Task<IEnumerable<BankAccount>> GetByUserAsync(Guid userId, CancellationToken ct = default);
}