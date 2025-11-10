using Finance.Domain.Entities;

namespace Finance.Domain.Factories;

public static class BankAccountFactory
{
    public static BankAccount Create(Guid userId, string name, decimal initial = 0m)
        => new BankAccount
        {
            Id = Guid.NewGuid(),
            UserId = userId,
            Name = name,
            Balance = initial
        };
}

public static class CategoryFactory
{
    public static Category Create(Guid userId, bool isIncome, string name)
        => new Category
        {
            Id = Guid.NewGuid(),
            UserId = userId,
            Name = name,
            IsIncome = isIncome
        };
}

public static class OperationFactory
{
    public static Operation Create(
        Guid userId,
        Guid accountId,
        Guid? categoryId,
        decimal amount,
        DateOnly occurredOn,
        string? description)
        => new Operation
        {
            Id = Guid.NewGuid(),
            UserId = userId,
            AccountId = accountId,
            CategoryId = categoryId,
            Amount = amount,
            OccurredOn = occurredOn,
            Description = description,
            CreatedAt = DateTime.UtcNow
        };
    
    public static Operation CreateIncome(
        Guid userId,
        Guid accountId,
        Guid? categoryId,
        decimal amount,
        DateOnly occurredOn,
        string? description)
        => Create(userId, accountId, categoryId, Math.Abs(amount), occurredOn, description);
    
    public static Operation CreateExpense(
        Guid userId,
        Guid accountId,
        Guid? categoryId,
        decimal amount,
        DateOnly occurredOn,
        string? description)
        => Create(userId, accountId, categoryId, -Math.Abs(amount), occurredOn, description);
    
    public static Operation CreateIncome(
        Guid userId,
        Guid accountId,
        Guid? categoryId,
        decimal amount,
        string? description = null)
        => CreateIncome(userId, accountId, categoryId, amount, DateOnly.FromDateTime(DateTime.UtcNow), description);

    public static Operation CreateExpense(
        Guid userId,
        Guid accountId,
        Guid? categoryId,
        decimal amount,
        string? description = null)
        => CreateExpense(userId, accountId, categoryId, amount, DateOnly.FromDateTime(DateTime.UtcNow), description);
}
