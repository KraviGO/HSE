using Finance.Api.Dtos;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class OperationEndpoints
{
    public static void MapOperationEndpoints(this WebApplication app)
    {
        app.MapGet("/operations/{userId:guid}", async (Guid userId, IOperationRepository repo) =>
        {
            var operations = await repo.GetByUserAsync(userId);

            var result = operations.Select(o => new OperationDto
            {
                Id = o.Id,
                AccountId = o.AccountId,
                Amount = o.Amount,
                Description = o.Description,
                OccurredOn = o.OccurredOn,
                CategoryName = o.Category?.Name,
                AccountName = o.Account?.Name ?? "",
                Currency = o.Account?.Currency ?? "RUB"
            });

            return Results.Ok(result);
        });
        
        app.MapGet("/operations/id/{id:guid}", async (Guid id, IOperationRepository repo) =>
        {
            var op = await repo.GetByIdAsync(id);
            if (op is null) return Results.NotFound();

            var dto = new OperationDto
            {
                Id = op.Id,
                AccountId = op.AccountId,
                Amount = op.Amount,
                Description = op.Description,
                OccurredOn = op.OccurredOn,
                CategoryName = op.Category?.Name,
                AccountName = op.Account?.Name ?? "",
                Currency = op.Account?.Currency ?? "RUB"
            };

            return Results.Ok(dto);
        });
        
        app.MapPost("/operations", async (
            IOperationRepository ops,
            IAccountRepository accs,
            ICategoryRepository cats,
            Operation op) =>
        {
            var acc = await accs.GetByIdAsync(op.AccountId);
            if (acc == null)
                return Results.BadRequest("Счёт не найден.");
            
            var cat = op.CategoryId.HasValue ? await cats.GetByIdAsync(op.CategoryId.Value) : null;
            
            acc.Balance += op.Amount;
            await accs.UpdateAsync(acc);
            await accs.SaveChangesAsync();

            await ops.AddAsync(op);
            await ops.SaveChangesAsync();

            var dto = new OperationDto
            {
                Id = op.Id,
                AccountId = op.AccountId,
                Amount = op.Amount,
                Description = op.Description,
                OccurredOn = op.OccurredOn,
                AccountName = acc.Name,
                Currency = acc.Currency,
                CategoryName = cat?.Name
            };

            return Results.Created($"/operations/{op.Id}", dto);
        });
        
        app.MapPut("/operations/{id:guid}", async (
            Guid id,
            IOperationRepository ops,
            IAccountRepository accs,
            ICategoryRepository cats,
            Operation op) =>
        {
            var existing = await ops.GetByIdAsync(id);
            if (existing == null)
                return Results.NotFound();

            var acc = await accs.GetByIdAsync(existing.AccountId);
            if (acc != null)
            {
                acc.Balance -= existing.Amount;
                acc.Balance += op.Amount;
                await accs.UpdateAsync(acc);
                await accs.SaveChangesAsync();
            }

            op.Id = id;
            await ops.UpdateAsync(op);
            await ops.SaveChangesAsync();

            var cat = op.CategoryId.HasValue ? await cats.GetByIdAsync(op.CategoryId.Value) : null;

            var dto = new OperationDto
            {
                Id = op.Id,
                AccountId = op.AccountId,
                Amount = op.Amount,
                Description = op.Description,
                OccurredOn = op.OccurredOn,
                AccountName = acc?.Name ?? "",
                Currency = acc?.Currency ?? "RUB",
                CategoryName = cat?.Name
            };

            return Results.Ok(dto);
        });
        
        app.MapDelete("/operations/{id:guid}", async (
            Guid id,
            IOperationRepository ops,
            IAccountRepository accs) =>
        {
            var existing = await ops.GetByIdAsync(id);
            if (existing == null)
                return Results.NotFound();

            var acc = await accs.GetByIdAsync(existing.AccountId);
            if (acc != null)
            {
                acc.Balance -= existing.Amount;
                await accs.UpdateAsync(acc);
                await accs.SaveChangesAsync();
            }

            await ops.DeleteAsync(id);
            await ops.SaveChangesAsync();

            return Results.Ok(new { message = "Операция удалена" });
        });
    }
}