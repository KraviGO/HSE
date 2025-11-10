using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Infrastructure.Repositories
{
    public class InMemoryGoalRepository : IGoalRepository
    {
        private readonly List<FinancialGoal> _goals = new();

        public Task<IEnumerable<FinancialGoal>> GetAllAsync(CancellationToken ct = default)
            => Task.FromResult(_goals.AsEnumerable());

        public Task<IEnumerable<FinancialGoal>> GetByUserAsync(Guid userId, CancellationToken ct = default)
            => Task.FromResult(_goals.Where(g => g.UserId == userId).AsEnumerable());

        public Task<FinancialGoal?> GetByAccountIdAsync(Guid accountId, CancellationToken ct = default)
            => Task.FromResult(_goals.FirstOrDefault(g => g.AccountId == accountId));

        public Task<FinancialGoal?> GetByIdAsync(Guid id, CancellationToken ct = default)
            => Task.FromResult(_goals.FirstOrDefault(g => g.Id == id));

        public Task AddAsync(FinancialGoal goal, CancellationToken ct = default)
        {
            _goals.Add(goal);
            return Task.CompletedTask;
        }

        public Task UpdateAsync(FinancialGoal goal, CancellationToken ct = default)
        {
            var existing = _goals.FirstOrDefault(g => g.Id == goal.Id);
            if (existing != null)
            {
                existing.Name = goal.Name;
                existing.TargetAmount = goal.TargetAmount;
                existing.CurrentAmount = goal.CurrentAmount;
                existing.Deadline = goal.Deadline;
            }
            return Task.CompletedTask;
        }

        public Task<bool> DeleteAsync(Guid id, CancellationToken ct = default)
        {
            var count = _goals.RemoveAll(g => g.Id == id);
            return Task.FromResult(count > 0);
        }

        public Task SaveChangesAsync(CancellationToken ct = default) => Task.CompletedTask;
    }
}