namespace Finance.Domain.Entities;

public class User
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public string Email { get; set; } = null!;
    public string Name { get; set; } = null!;
    public DateTime CreatedAt { get; set; } = DateTime.UtcNow;

    public ICollection<BankAccount> Accounts { get; set; } = new List<BankAccount>();
    public ICollection<Category> Categories { get; set; } = new List<Category>();
    public ICollection<Operation> Operations { get; set; } = new List<Operation>();
}