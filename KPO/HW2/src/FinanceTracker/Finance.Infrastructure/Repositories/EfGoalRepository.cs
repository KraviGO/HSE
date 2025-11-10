using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using Finance.Infrastructure.Persistence;
using Microsoft.EntityFrameworkCore;

namespace Finance.Infrastructure.Repositories
{
    public class EfGoalRepository : BaseRepository<FinancialGoal>, IGoalRepository
    {
        public EfGoalRepository(FinanceDbContext db) : base(db) { }

        public async Task<IEnumerable<FinancialGoal>> GetByUserAsync(Guid userId, CancellationToken ct = default)
            => await _db.Goals
                .AsNoTracking()
                .Where(g => g.UserId == userId)
                .OrderBy(g => g.Deadline)
                .ToListAsync(ct);

        public async Task<FinancialGoal?> GetByAccountIdAsync(Guid accountId, CancellationToken ct = default)
            => await _db.Goals
                .AsNoTracking()
                .FirstOrDefaultAsync(g => g.AccountId == accountId, ct);
    }
}