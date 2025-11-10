using Finance.Application.Services;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using IAccountRepository = Finance.Domain.Ports.Repositories.IAccountRepository;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class GoalEndpoints
{
    public static void MapGoalEndpoints(this WebApplication app)
    {
        app.MapGet("/goals/{userId:guid}", async (Guid userId, IGoalRepository repo) =>
            Results.Ok(await repo.GetByUserAsync(userId)));

        app.MapPost("/goals", async (IGoalRepository goals, IAccountRepository accRepo, FinancialGoal goal) =>
        {
            var acc = new BankAccount
            {
                UserId = goal.UserId,
                Name = $"Цель: {goal.Name}",
                Currency = "RUB",
                Balance = 0m,
                CreatedAt = DateTime.UtcNow
            };

            await accRepo.AddAsync(acc);
            await accRepo.SaveChangesAsync();

            goal.AccountId = acc.Id;
            await goals.AddAsync(goal);
            await goals.SaveChangesAsync();

            return Results.Created($"/goals/{goal.Id}", goal);
        });

        app.MapPost("/goals/{id:guid}/deposit", async (Guid id, IGoalRepository goals, IAccountRepository accRepo, TransferService transfer, DepositRequest req) =>
        {
            var goal = await goals.GetByIdAsync(id);
            if (goal == null) return Results.NotFound();

            await transfer.TransferAsync(req.FromId, goal.AccountId, req.Amount, req.UserId, null, $"Пополнение цели: {goal.Name}");
            return Results.Ok(new { message = "Цель успешно пополнена" });
        });

        app.MapDelete("/goals/{id:guid}", async (Guid id, IGoalRepository goals, IAccountRepository accRepo) =>
        {
            var goal = await goals.GetByIdAsync(id);
            if (goal == null) return Results.NotFound();

            var acc = await accRepo.GetByIdAsync(goal.AccountId);
            if (acc != null)
            {
                await accRepo.DeleteAsync(acc.Id);
                await accRepo.SaveChangesAsync();
            }

            await goals.DeleteAsync(id);
            await goals.SaveChangesAsync();

            return Results.Ok(new { message = "Цель удалена" });
        });
    }
}

public record DepositRequest(Guid UserId, Guid FromId, decimal Amount);