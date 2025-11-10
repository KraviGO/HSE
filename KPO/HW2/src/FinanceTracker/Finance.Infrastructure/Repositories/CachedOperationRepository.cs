using Finance.Domain.Entities;
using Microsoft.Extensions.Caching.Memory;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Infrastructure.Repositories;

public class CachedOperationRepository : IOperationRepository
{
    private readonly IOperationRepository _inner;
    private readonly IMemoryCache _cache;

    public CachedOperationRepository(IOperationRepository inner, IMemoryCache cache)
    {
        _inner = inner;
        _cache = cache;
    }

    public async Task<IEnumerable<Operation>> GetByUserAsync(Guid userId, CancellationToken ct = default)
        => await _inner.GetByUserAsync(userId, ct);

    public async Task<IEnumerable<Operation>> GetByAccountAsync(Guid accountId, CancellationToken ct = default)
        => await _inner.GetByAccountAsync(accountId, ct);

    public async Task<IEnumerable<Operation>> GetByPeriodAsync(Guid userId, DateOnly from, DateOnly to, CancellationToken ct = default)
        => await _inner.GetByPeriodAsync(userId, from, to, ct);

    public async Task<Operation?> GetByIdAsync(Guid id, CancellationToken ct = default)
    {
        if (_cache.TryGetValue(id, out Operation? cached))
            return cached;

        var op = await _inner.GetByIdAsync(id, ct);
        if (op != null)
            _cache.Set(id, op, TimeSpan.FromMinutes(10));

        return op;
    }

    public async Task AddAsync(Operation entity, CancellationToken ct = default)
    {
        await _inner.AddAsync(entity, ct);
        _cache.Remove(entity.Id);
    }

    public async Task UpdateAsync(Operation entity, CancellationToken ct = default)
    {
        await _inner.UpdateAsync(entity, ct);
        _cache.Remove(entity.Id);
    }

    public async Task<bool> DeleteAsync(Guid id, CancellationToken ct = default)
    {
        var result = await _inner.DeleteAsync(id, ct);
        _cache.Remove(id);
        return result;
    }

    public async Task SaveChangesAsync(CancellationToken ct = default)
        => await _inner.SaveChangesAsync(ct);
}