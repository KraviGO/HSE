using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class AnalyticsEndpoints
{
    public static void MapAnalyticsEndpoints(this WebApplication app)
    {
        app.MapGet("/analytics/{userId:guid}", async (Guid userId,
            IOperationRepository opsRepo, IAccountRepository accRepo) =>
        {
            var ops = (await opsRepo.GetByUserAsync(userId)).ToList();
            if (!ops.Any()) return Results.Ok(new { message = "Нет операций" });

            var income = ops.Where(o => o.Amount > 0).Sum(o => o.Amount);
            var expense = ops.Where(o => o.Amount < 0).Sum(o => o.Amount);
            var net = income + expense;

            var byCategory = ops
                .GroupBy(o => o.Category?.Name ?? "Без категории")
                .Select(g => new
                {
                    Category = g.Key,
                    Total = g.Sum(x => x.Amount),
                    Type = g.Sum(x => x.Amount) >= 0 ? "Доход" : "Расход"
                });

            return Results.Ok(new
            {
                totalIncome = income,
                totalExpense = expense,
                balance = net,
                byCategory
            });
        });
    }
}