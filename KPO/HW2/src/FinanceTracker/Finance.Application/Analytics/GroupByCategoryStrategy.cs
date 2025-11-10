using Finance.Domain.Entities;
using Finance.Domain.Ports.Services;

namespace Finance.Application.Analytics;

public sealed class GroupByCategoryStrategy :
    IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops, IEnumerable<Category> cats),
        IReadOnlyDictionary<string, decimal>>
{
    public IReadOnlyDictionary<string, decimal> Analyze(
        (Guid userId, IEnumerable<Operation> ops, IEnumerable<Category> cats) input)
    {
        var (_, ops, cats) = input;

        var nameById = cats.ToDictionary(c => c.Id, c => c.Name);

        return ops
            .GroupBy(o => o.CategoryId.HasValue && nameById.TryGetValue(o.CategoryId.Value, out var name)
                ? name
                : "Без категории")
            .ToDictionary(
                g => g.Key,
                g => g.Sum(o => o.Amount)
            );
    }
}