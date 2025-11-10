using System.IO.Compression;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Api.Extensions.EndpointExtensions;

public static class ImportExportEndpoints
{
    public static void MapImportExportEndpoints(this WebApplication app)
    {
        app.MapPost("/import/upload", async (IFormFile file, IImportService importer, ILoggerFactory loggerFactory) =>
        {
            var logger = loggerFactory.CreateLogger("Import");

            if (file is null || file.Length == 0)
                return Results.BadRequest(new { message = "Файл не получен или пуст." });

            var uploadDir = Path.Combine(AppContext.BaseDirectory, "uploads");
            Directory.CreateDirectory(uploadDir);

            var filePath = Path.Combine(uploadDir, file.FileName);

            try
            {
                await using (var stream = new FileStream(filePath, FileMode.Create))
                    await file.CopyToAsync(stream);

                logger.LogInformation("📥 Импорт файла: {FilePath}", filePath);

                await importer.ImportAsync(filePath);

                logger.LogInformation("✅ Импорт завершён успешно: {FilePath}", filePath);
                File.Delete(filePath);

                return Results.Ok(new { message = $"✅ Импорт успешно завершён ({file.FileName})" });
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "Ошибка при импорте файла {FilePath}", filePath);
                return Results.Problem(new
                {
                    message = "Ошибка при импорте файла",
                    detail = ex.Message,
                    stackTrace = ex.StackTrace
                }.ToString());
            }
        });
        
        app.MapPost("/export/download", async (
            ExportRequest req,
            IExportVisitor exporter,
            IUserRepository users,
            IAccountRepository accounts,
            ICategoryRepository categories,
            IOperationRepository operations,
            ILoggerFactory loggerFactory) =>
        {
            var logger = loggerFactory.CreateLogger("Export");

            try
            {
                var user = await users.GetByIdAsync(req.UserId);
                if (user == null)
                    return Results.NotFound(new { message = "Пользователь не найден." });

                var format = (req.Format ?? "json").Trim().ToLower();
                if (!new[] { "json", "csv", "xml" }.Contains(format))
                    return Results.BadRequest(new { message = $"Формат '{format}' не поддерживается." });

                var accs = await accounts.GetByUserAsync(req.UserId);
                var cats = await categories.GetByUserAsync(req.UserId);
                var ops = await operations.GetByUserAsync(req.UserId);

                var exportDir = Path.Combine(AppContext.BaseDirectory, "exports");
                Directory.CreateDirectory(exportDir);

                var baseName = $"export_{DateTime.UtcNow:yyyy-MM-dd_HH-mm-ss}";
                var fullPath = Path.Combine(exportDir, $"{baseName}.{format}");

                logger.LogInformation("📤 Экспорт данных пользователя {UserId} в формат {Format}", req.UserId, format);

                await exporter.VisitAsync(user, accs, cats, ops, fullPath);

                if (Directory.Exists(fullPath))
                {
                    var zipPath = fullPath + ".zip";
                    if (File.Exists(zipPath)) File.Delete(zipPath);
                    ZipFile.CreateFromDirectory(fullPath, zipPath);
                    var zipBytes = await File.ReadAllBytesAsync(zipPath);

                    logger.LogInformation("📦 Каталог экспорта упакован в ZIP: {ZipPath}", zipPath);
                    return Results.File(zipBytes, "application/zip", Path.GetFileName(zipPath));
                }

                if (!File.Exists(fullPath))
                    return Results.Problem("❌ Файл экспорта не был создан.");

                var bytes = await File.ReadAllBytesAsync(fullPath);
                var mime = format switch
                {
                    "csv" => "text/csv",
                    "xml" => "application/xml",
                    _ => "application/json"
                };

                logger.LogInformation("✅ Экспорт успешно завершён: {FilePath}", fullPath);
                return Results.File(bytes, mime, Path.GetFileName(fullPath));
            }
            catch (Exception ex)
            {
                logger.LogError(ex, "Ошибка при экспорте");
                return Results.Problem(new
                {
                    message = "Ошибка при экспорте данных",
                    detail = ex.Message,
                    stackTrace = ex.StackTrace
                }.ToString());
            }
        });
    }

    public record ImportRequest(string Path);
    public record ExportRequest(Guid UserId, string? Format);
}