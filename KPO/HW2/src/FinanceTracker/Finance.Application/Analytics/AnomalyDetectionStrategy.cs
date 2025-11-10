using Finance.Domain.Entities;
using Finance.Domain.Ports.Services;

namespace Finance.Application.Analytics;

public sealed class AnomalyDetectionStrategy :
    IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops), IReadOnlyList<Operation>>
{
    public IReadOnlyList<Operation> Analyze((Guid userId, IEnumerable<Operation> ops) input)
    {
        var (_, ops) = input;

        var expenses = ops
            .Where(o => o.Amount < 0)
            .Select(o => Math.Abs(o.Amount))
            .OrderBy(x => x)
            .ToArray();

        if (expenses.Length < 4)
            return Array.Empty<Operation>();

        decimal Q1 = expenses[expenses.Length / 4];
        decimal Q3 = expenses[(expenses.Length * 3) / 4];
        var iqr = Q3 - Q1;
        var upper = Q3 + 1.5m * iqr;

        return ops
            .Where(o => o.Amount < 0 && Math.Abs(o.Amount) > upper)
            .ToList();
    }
}