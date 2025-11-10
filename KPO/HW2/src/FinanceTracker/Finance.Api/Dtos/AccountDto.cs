namespace Finance.Api.Dtos;

public class AccountDto
{
    public Guid Id { get; set; }
    public string Name { get; set; } = "";
    public decimal Balance { get; set; }
    public string Currency { get; set; } = "";
}