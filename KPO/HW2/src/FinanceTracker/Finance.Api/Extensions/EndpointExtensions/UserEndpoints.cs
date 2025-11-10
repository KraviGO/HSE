using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class UserEndpoints
{
    public static void MapUserEndpoints(this WebApplication app)
    {
        app.MapGet("/users", async (IUserRepository repo) =>
            Results.Ok(await repo.GetAllAsync()));

        app.MapGet("/users/{id:guid}", async (Guid id, IUserRepository repo) =>
        {
            var user = await repo.GetByIdAsync(id);
            return user is null ? Results.NotFound() : Results.Ok(user);
        });

        app.MapPost("/users", async (IUserRepository repo, User user) =>
        {
            user.CreatedAt = DateTime.UtcNow;
            await repo.AddAsync(user);
            await repo.SaveChangesAsync();
            return Results.Created($"/users/{user.Id}", user);
        });

        app.MapDelete("/users/{id:guid}", async (Guid id, IUserRepository repo) =>
        {
            await repo.DeleteAsync(id);
            await repo.SaveChangesAsync();
            return Results.Ok();
        });
    }
}