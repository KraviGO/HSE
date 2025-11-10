using Finance.Application.Services;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class TransferEndpoints
{
    public static void MapTransferEndpoints(this WebApplication app)
    {
        app.MapPost("/transfer", async (TransferRequest req, TransferService service) =>
        {
            await service.TransferAsync(req.FromId, req.ToId, req.Amount, req.UserId);
            return Results.Ok(new { message = "Перевод выполнен успешно" });
        });
    }
}

public record TransferRequest(Guid UserId, Guid FromId, Guid ToId, decimal Amount);