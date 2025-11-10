using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class CategoryEndpoints
{
    public static void MapCategoryEndpoints(this WebApplication app)
    {
        app.MapGet("/categories/{userId:guid}", async (Guid userId, ICategoryRepository repo) =>
            Results.Ok(await repo.GetByUserAsync(userId)));

        app.MapPost("/categories", async (ICategoryRepository repo, Category category) =>
        {
            await repo.AddAsync(category);
            await repo.SaveChangesAsync();
            return Results.Created($"/categories/{category.Id}", category);
        });

        app.MapPut("/categories/{id:guid}", async (Guid id, ICategoryRepository repo, Category category) =>
        {
            category.Id = id;
            await repo.UpdateAsync(category);
            await repo.SaveChangesAsync();
            return Results.Ok(category);
        });

        app.MapDelete("/categories/{id:guid}", async (Guid id, ICategoryRepository repo) =>
        {
            await repo.DeleteAsync(id);
            await repo.SaveChangesAsync();
            return Results.Ok();
        });
    }
}