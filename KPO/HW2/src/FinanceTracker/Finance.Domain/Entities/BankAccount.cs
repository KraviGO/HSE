namespace Finance.Domain.Entities;

public class BankAccount
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public Guid UserId { get; set; }
    public string Name { get; set; } = null!;
    public string Currency { get; set; } = "RUB";
    public decimal Balance { get; set; } = 0m;
    public DateTime CreatedAt { get; set; } = DateTime.UtcNow;

    public User User { get; set; } = null!;
    public ICollection<Operation> Operations { get; set; } = new List<Operation>();
}