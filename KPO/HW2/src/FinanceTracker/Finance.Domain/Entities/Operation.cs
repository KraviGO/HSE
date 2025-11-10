namespace Finance.Domain.Entities;

public class Operation
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public Guid UserId { get; set; }
    public Guid AccountId { get; set; }
    public Guid? CategoryId { get; set; }
    
    public decimal Amount { get; set; }

    public string? Description { get; set; }
    public DateOnly OccurredOn { get; set; } = DateOnly.FromDateTime(DateTime.UtcNow);
    public DateTime CreatedAt { get; set; } = DateTime.UtcNow;

    public User User { get; set; } = null!;
    public BankAccount Account { get; set; } = null!;
    public Category? Category { get; set; }
}