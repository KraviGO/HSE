using Finance.Infrastructure.Repositories;
using Finance.Infrastructure.Io.Import;
using Finance.Infrastructure.Io.Export;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Api.Extensions;

public static class AppRepositories
{
    public static void AddAppRepositories(this IServiceCollection services)
    {
        services.AddScoped<IUserRepository, EfUserRepository>();
        services.AddScoped<IAccountRepository, EfAccountRepository>();
        services.AddScoped<ICategoryRepository, EfCategoryRepository>();
        services.AddScoped<IOperationRepository, EfOperationRepository>();
        services.AddScoped<IGoalRepository, InMemoryGoalRepository>();
    }

    public static void AddAppImportExport(this IServiceCollection services)
    {
        services.AddScoped<CsvImporter>();
        services.AddScoped<JsonImporter>();
        services.AddScoped<YamlImporter>();
        services.AddScoped<IImportService, CompositeImporter>();
        services.AddScoped<IExportVisitor, MultiFormatExportVisitor>();
    }
}