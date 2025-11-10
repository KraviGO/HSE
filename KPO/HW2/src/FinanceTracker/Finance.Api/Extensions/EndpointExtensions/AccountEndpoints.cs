using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class AccountEndpoints
{
    public static void MapAccountEndpoints(this WebApplication app)
    {
        app.MapGet("/accounts/{userId:guid}", async (Guid userId, IAccountRepository repo) =>
            Results.Ok(await repo.GetByUserAsync(userId)));

        app.MapGet("/accounts/id/{id:guid}", async (Guid id, IAccountRepository repo) =>
        {
            var acc = await repo.GetByIdAsync(id);
            return acc is null ? Results.NotFound() : Results.Ok(acc);
        });

        app.MapPost("/accounts", async (IAccountRepository repo, BankAccount acc) =>
        {
            acc.CreatedAt = DateTime.UtcNow;
            await repo.AddAsync(acc);
            await repo.SaveChangesAsync();
            return Results.Created($"/accounts/{acc.Id}", acc);
        });

        app.MapPut("/accounts/{id:guid}", async (Guid id, IAccountRepository repo, BankAccount acc) =>
        {
            acc.Id = id;
            await repo.UpdateAsync(acc);
            await repo.SaveChangesAsync();
            return Results.Ok(acc);
        });

        app.MapDelete("/accounts/{id:guid}", async (Guid id, IAccountRepository repo) =>
        {
            await repo.DeleteAsync(id);
            await repo.SaveChangesAsync();
            return Results.Ok();
        });
    }
}