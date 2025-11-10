using Finance.Infrastructure.Persistence;
using Microsoft.EntityFrameworkCore;

namespace Finance.Api.Extensions;

public static class DatabaseExtensions
{
    public static void AddAppDatabase(this IServiceCollection services, string connectionString)
    {
        services.AddDbContext<FinanceDbContext>(opt => opt.UseSqlite(connectionString));
    }
}