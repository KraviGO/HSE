namespace Finance.Domain.Entities;

public class Category
{
    public Guid Id { get; set; } = Guid.NewGuid();
    public Guid UserId { get; set; }
    public string Name { get; set; } = null!;
    public bool IsIncome { get; set; }

    public User User { get; set; } = null!;
}