using Finance.Application.Analytics;
using Finance.Application.Services;
using Finance.Domain.Ports.Repositories;
using Finance.Domain.Ports.Services;
using Finance.Infrastructure.Io.Export;
using Finance.Infrastructure.Io.Import;
using Finance.Infrastructure.Persistence;
using Finance.Infrastructure.Repositories;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Spectre.Console;

var services = new ServiceCollection();

// --- DB --------------------------------------
services.AddDbContext<FinanceDbContext>(options =>
{
    options.UseSqlite("Data Source=finance.db");
});

// --- repositories ----------------------------
services.AddScoped<IUserRepository, EfUserRepository>();
services.AddScoped<IAccountRepository, EfAccountRepository>();
services.AddScoped<ICategoryRepository, EfCategoryRepository>();
services.AddScoped<IOperationRepository, EfOperationRepository>();
services.AddSingleton<IGoalRepository, InMemoryGoalRepository>();

// --- services ---------------------------
services.AddScoped<TransferService>();

// --- analitics strateges ---------------------------
services.AddScoped<NetIncomeStrategy>();
services.AddScoped<GroupByCategoryStrategy>();
services.AddScoped<AnomalyDetectionStrategy>();
services.AddScoped<AnalyticsFacade>();

// --- IO ----------------------------------
services.AddScoped<CsvImporter>();
services.AddScoped<JsonImporter>();
services.AddScoped<YamlImporter>();

// мультиимпорт
services.AddScoped<IImportService, CompositeImporter>();

// мультиэкспорт
services.AddScoped<IExportVisitor, MultiFormatExportVisitor>();

services.AddScoped<Cli>();

var sp = services.BuildServiceProvider();

// --- DB migration -----------------------------
await using (var scope = sp.CreateAsyncScope())
{
    var db = scope.ServiceProvider.GetRequiredService<FinanceDbContext>();
    await db.Database.EnsureCreatedAsync();
}

AnsiConsole.MarkupLine("[bold green]Finance CLI запущен[/]");
await sp.GetRequiredService<Cli>().RunAsync();