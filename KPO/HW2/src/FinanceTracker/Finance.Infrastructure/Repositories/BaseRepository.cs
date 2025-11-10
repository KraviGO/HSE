using Finance.Domain.Ports.Repositories;
using Finance.Infrastructure.Persistence;
using Microsoft.EntityFrameworkCore;

namespace Finance.Infrastructure.Repositories
{
    public abstract class BaseRepository<TEntity> : IBaseRepository<TEntity>
        where TEntity : class
    {
        protected readonly FinanceDbContext _db;
        protected readonly DbSet<TEntity> _set;

        protected BaseRepository(FinanceDbContext db)
        {
            _db = db;
            _set = db.Set<TEntity>();
        }

        public virtual async Task<IEnumerable<TEntity>> GetAllAsync(CancellationToken ct = default)
            => await _set.AsNoTracking().ToListAsync(ct);

        public virtual async Task<TEntity?> GetByIdAsync(Guid id, CancellationToken ct = default)
            => await _set.FindAsync(new object?[] { id }, ct);

        public virtual async Task AddAsync(TEntity entity, CancellationToken ct = default)
            => await _set.AddAsync(entity, ct);

        public virtual Task UpdateAsync(TEntity entity, CancellationToken ct = default)
        {
            var local = _set.Local.FirstOrDefault(e =>
            {
                var key = _db.Model.FindEntityType(typeof(TEntity))?.FindPrimaryKey();
                if (key == null) return false;

                var entryLocal = _db.Entry(e);
                var entryEntity = _db.Entry(entity);

                return key.Properties.All(p =>
                    Equals(entryLocal.Property(p.Name).CurrentValue,
                        entryEntity.Property(p.Name).CurrentValue));
            });

            if (local != null)
            {
                _db.Entry(local).CurrentValues.SetValues(entity);
            }
            else
            {
                _db.Attach(entity);
                _db.Entry(entity).State = EntityState.Modified;
            }

            return Task.CompletedTask;
        }

        public virtual async Task<bool> DeleteAsync(Guid id, CancellationToken ct = default)
        {
            var existing = await _set.FindAsync(new object?[] { id }, ct);
            if (existing == null)
                return false;

            _set.Remove(existing);
            return true;
        }

        public virtual Task SaveChangesAsync(CancellationToken ct = default)
            => _db.SaveChangesAsync(ct);
    }
}