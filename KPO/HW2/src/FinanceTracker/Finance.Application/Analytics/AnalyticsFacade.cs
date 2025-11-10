using Finance.Domain.Entities;

namespace Finance.Application.Analytics;

public class AnalyticsFacade
{
    private readonly NetIncomeStrategy _netIncome;
    private readonly GroupByCategoryStrategy _groupByCategory;
    private readonly AnomalyDetectionStrategy _anomalies;

    public AnalyticsFacade(
        NetIncomeStrategy netIncome,
        GroupByCategoryStrategy groupByCategory,
        AnomalyDetectionStrategy anomalies)
    {
        _netIncome = netIncome;
        _groupByCategory = groupByCategory;
        _anomalies = anomalies;
    }

    public decimal CalculateNetIncome(Guid userId, IEnumerable<Operation> ops)
        => _netIncome.Analyze((userId, ops));

    public IReadOnlyDictionary<string, decimal> GroupByCategory(
        Guid userId, IEnumerable<Operation> ops, IEnumerable<Category> cats)
        => _groupByCategory.Analyze((userId, ops, cats));

    public IReadOnlyList<Operation> DetectAnomalies(Guid userId, IEnumerable<Operation> ops)
        => _anomalies.Analyze((userId, ops));
}