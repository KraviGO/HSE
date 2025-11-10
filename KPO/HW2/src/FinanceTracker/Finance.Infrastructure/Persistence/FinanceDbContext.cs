using Finance.Domain.Entities;
using Microsoft.EntityFrameworkCore;

namespace Finance.Infrastructure.Persistence;

public class FinanceDbContext : DbContext
{
    public FinanceDbContext(DbContextOptions<FinanceDbContext> options) : base(options) { }

    public DbSet<User> Users => Set<User>();
    public DbSet<BankAccount> Accounts => Set<BankAccount>();
    public DbSet<Category> Categories => Set<Category>();
    public DbSet<Operation> Operations => Set<Operation>();
    
    public DbSet<FinancialGoal> Goals => Set<FinancialGoal>();

    protected override void OnModelCreating(ModelBuilder b)
    {
        b.Entity<User>(e =>
        {
            e.HasKey(x => x.Id);
            e.HasIndex(x => x.Email).IsUnique();
            e.Property(x => x.Email).IsRequired();
            e.Property(x => x.Name).IsRequired();
        });

        b.Entity<BankAccount>(e =>
        {
            e.HasKey(x => x.Id);
            e.Property(x => x.Name).IsRequired();
            e.Property(x => x.Currency).HasMaxLength(8).HasDefaultValue("RUB");
            e.Property(x => x.Balance).HasPrecision(18, 2);

            e.HasOne(x => x.User)
                .WithMany(u => u.Accounts)
                .HasForeignKey(x => x.UserId)
                .OnDelete(DeleteBehavior.Cascade);
        });

        b.Entity<Category>(e =>
        {
            e.HasKey(x => x.Id);
            e.Property(x => x.Name).IsRequired();

            e.HasOne(x => x.User)
                .WithMany(u => u.Categories)
                .HasForeignKey(x => x.UserId)
                .OnDelete(DeleteBehavior.Cascade);
        });

        b.Entity<Operation>(e =>
        {
            e.HasKey(x => x.Id);
            e.Property(x => x.Amount).HasPrecision(18, 2);

            e.HasOne(x => x.User)
                .WithMany(u => u.Operations)
                .HasForeignKey(x => x.UserId)
                .OnDelete(DeleteBehavior.Cascade);

            e.HasOne(x => x.Account)
                .WithMany(a => a.Operations)
                .HasForeignKey(x => x.AccountId)
                .OnDelete(DeleteBehavior.Cascade);

            e.HasOne(x => x.Category)
                .WithMany()
                .HasForeignKey(x => x.CategoryId)
                .OnDelete(DeleteBehavior.SetNull);
        });
    }
}