namespace Finance.Domain.Entities;

public class FinancialGoal
{
   public Guid Id { get; set; } = Guid.NewGuid();
   public Guid UserId { get; set; }
   public Guid AccountId { get; set; }
   public string Name { get; set; } = string.Empty;
   public decimal TargetAmount { get; set; }
   public DateOnly Deadline { get; set; }

   public decimal CurrentAmount { get; set; }
   public decimal Progress => Math.Min(CurrentAmount / TargetAmount * 100, 100);
   public bool IsAchieved => CurrentAmount >= TargetAmount;
}
