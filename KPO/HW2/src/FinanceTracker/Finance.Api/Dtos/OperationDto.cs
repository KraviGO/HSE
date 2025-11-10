namespace Finance.Api.Dtos;

public class OperationDto
{
    public Guid Id { get; set; }
    public Guid AccountId { get; set; }
    public decimal Amount { get; set; }
    public string? Description { get; set; }
    public DateOnly OccurredOn { get; set; }
    public string? CategoryName { get; set; }
    public string AccountName { get; set; } = "";
    public string Currency { get; set; } = "";
}