using Finance.Domain.Entities;
using Finance.Domain.Ports.Services;

namespace Finance.Application.Analytics;

public sealed class NetIncomeStrategy :
    IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops), decimal>
{
    public decimal Analyze((Guid userId, IEnumerable<Operation> ops) input)
    {
        var (_, ops) = input;
        return ops.Sum(o => o.Amount);
    }
}