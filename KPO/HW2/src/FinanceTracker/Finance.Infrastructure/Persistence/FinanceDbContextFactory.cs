using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;

namespace Finance.Infrastructure.Persistence
{
    public class FinanceDbContextFactory : IDesignTimeDbContextFactory<FinanceDbContext>
    {
        public FinanceDbContext CreateDbContext(string[] args)
        {
            var optionsBuilder = new DbContextOptionsBuilder<FinanceDbContext>();
            optionsBuilder.UseSqlite("Data Source=finance.db");

            return new FinanceDbContext(optionsBuilder.Options);
        }
    }
}