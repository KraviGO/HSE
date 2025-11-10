using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Commands.Operations;

public sealed class AddOperationCommand : Finance.Application.Abstractions.ICommand<Guid>
{
    private readonly IOperationRepository _opRepo;
    private readonly IAccountRepository _accRepo;
    private readonly Guid _userId, _accountId, _categoryId;
    private readonly decimal _amount;
    private readonly DateOnly _date;
    private readonly string? _desc;
    private readonly bool _isIncome;

    public AddOperationCommand(
        IOperationRepository opRepo,
        IAccountRepository accRepo,
        Guid userId, Guid accountId, Guid categoryId,
        bool isIncome, decimal amount, DateOnly date, string? desc)
    {
        _opRepo = opRepo;
        _accRepo = accRepo;
        _userId = userId;
        _accountId = accountId;
        _categoryId = categoryId;
        _isIncome = isIncome;
        _amount = amount;
        _date = date;
        _desc = desc;
    }

    public async Task<Guid> Execute(CancellationToken ct = default)
    {
        var finalAmount = _isIncome ? Math.Abs(_amount) : -Math.Abs(_amount);

        var operation = new Operation
        {
            Id = Guid.NewGuid(),
            UserId = _userId,
            AccountId = _accountId,
            CategoryId = _categoryId,
            Amount = finalAmount,
            Description = _desc,
            OccurredOn = _date,
            CreatedAt = DateTime.UtcNow
        };
        
        await _opRepo.AddAsync(operation, ct);
        
        var account = await _accRepo.GetByIdAsync(_accountId, ct)
            ?? throw new InvalidOperationException("Account not found");

        account.Balance += finalAmount;
        await _accRepo.UpdateAsync(account, ct);
        await _accRepo.SaveChangesAsync(ct);

        return operation.Id;
    }
}