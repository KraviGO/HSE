using Finance.Domain.Entities;

namespace Finance.Domain.Ports.Repositories;

public interface IUserRepository : IBaseRepository<User>
{
    Task<IEnumerable<User>> GetAllAsync(CancellationToken ct = default);
    Task<User?> GetByEmailAsync(string email, CancellationToken ct = default);
}