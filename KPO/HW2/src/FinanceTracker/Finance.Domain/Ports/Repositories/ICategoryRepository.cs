using Finance.Domain.Entities;

namespace Finance.Domain.Ports.Repositories;

public interface ICategoryRepository : IBaseRepository<Category>
{
    Task<IEnumerable<Category>> GetByUserAsync(Guid userId, CancellationToken ct = default);
}