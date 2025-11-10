using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Facades;

public sealed class AnalyticsFacade
{
    private readonly IOperationRepository _opRepo;
    private readonly ICategoryRepository _catRepo;
    private readonly IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops), decimal> _netIncome;
    private readonly IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops, IEnumerable<Category> cats), IReadOnlyDictionary<string, decimal>> _groupByCat;
    private readonly IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops), IReadOnlyList<Operation>> _anomaly;

    public AnalyticsFacade(
        IOperationRepository opRepo,
        ICategoryRepository catRepo,
        IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops), decimal> netIncome,
        IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops, IEnumerable<Category> cats), IReadOnlyDictionary<string, decimal>> groupByCat,
        IAnalyticsStrategy<(Guid userId, IEnumerable<Operation> ops), IReadOnlyList<Operation>> anomaly)
    {
        _opRepo = opRepo;
        _catRepo = catRepo;
        _netIncome = netIncome;
        _groupByCat = groupByCat;
        _anomaly = anomaly;
    }

    public async Task<decimal> NetIncomeAsync(Guid userId, DateOnly from, DateOnly to, CancellationToken ct = default)
    {
        var ops = await _opRepo.GetByPeriodAsync(userId, from, to, ct);
        return _netIncome.Analyze((userId, ops));
    }

    public async Task<IReadOnlyDictionary<string, decimal>> GroupByCategoryAsync(Guid userId, DateOnly from, DateOnly to, CancellationToken ct = default)
    {
        var ops = await _opRepo.GetByPeriodAsync(userId, from, to, ct);
        var cats = await _catRepo.GetByUserAsync(userId, ct);
        return _groupByCat.Analyze((userId, ops, cats));
    }

    public async Task<IReadOnlyList<Operation>> DetectAnomaliesAsync(Guid userId, DateOnly from, DateOnly to, CancellationToken ct = default)
    {
        var ops = await _opRepo.GetByPeriodAsync(userId, from, to, ct);
        return _anomaly.Analyze((userId, ops));
    }
}