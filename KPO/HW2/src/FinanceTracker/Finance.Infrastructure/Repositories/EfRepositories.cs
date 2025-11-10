using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using Finance.Infrastructure.Persistence;
using Microsoft.EntityFrameworkCore;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Infrastructure.Repositories;

public class EfUserRepository : BaseRepository<User>, IUserRepository
{
    public EfUserRepository(FinanceDbContext db) : base(db) { }

    public async Task<User?> GetByEmailAsync(string email, CancellationToken ct = default)
        => await _db.Users.AsNoTracking().FirstOrDefaultAsync(u => u.Email == email, ct);
}

public class EfAccountRepository : BaseRepository<BankAccount>, IAccountRepository
{
    public EfAccountRepository(FinanceDbContext db) : base(db) { }

    public async Task<IEnumerable<BankAccount>> GetByUserAsync(Guid userId, CancellationToken ct = default)
        => await _db.Accounts
            .AsNoTracking()
            .Where(a => a.UserId == userId)
            .OrderBy(a => a.CreatedAt)
            .ToListAsync(ct);
}

public class EfCategoryRepository : BaseRepository<Category>, ICategoryRepository
{
    public EfCategoryRepository(FinanceDbContext db) : base(db) { }

    public async Task<IEnumerable<Category>> GetByUserAsync(Guid userId, CancellationToken ct = default)
        => await _db.Categories
            .AsNoTracking()
            .Where(c => c.UserId == userId)
            .OrderBy(c => c.Name)
            .ToListAsync(ct);
}

public class EfOperationRepository : BaseRepository<Operation>, IOperationRepository
{
    public EfOperationRepository(FinanceDbContext db) : base(db) { }

    public async Task<IEnumerable<Operation>> GetByUserAsync(Guid userId, CancellationToken ct = default)
        => await _db.Operations
            .AsNoTracking()
            .Include(o => o.Account)
            .Include(o => o.Category)
            .Where(o => o.UserId == userId)
            .OrderByDescending(o => o.OccurredOn)
            .ToListAsync(ct);

    public async Task<IEnumerable<Operation>> GetByAccountAsync(Guid accountId, CancellationToken ct = default)
        => await _db.Operations
            .AsNoTracking()
            .Include(o => o.Category)
            .Where(o => o.AccountId == accountId)
            .OrderByDescending(o => o.OccurredOn)
            .ToListAsync(ct);

    public async Task<IEnumerable<Operation>> GetByPeriodAsync(Guid userId, DateOnly from, DateOnly to, CancellationToken ct = default)
        => await _db.Operations
            .AsNoTracking()
            .Include(o => o.Account)
            .Include(o => o.Category)
            .Where(o => o.UserId == userId && o.OccurredOn >= from && o.OccurredOn <= to)
            .OrderByDescending(o => o.OccurredOn)
            .ToListAsync(ct);

    public async Task<Operation?> GetByIdAsync(Guid id, CancellationToken ct = default)
        => await _db.Operations
            .Include(o => o.Account)
            .Include(o => o.Category)
            .FirstOrDefaultAsync(o => o.Id == id, ct);
}
