using Finance.Application.Abstractions;
using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Commands.Operations;

public sealed class EditOperationCommand : ICommand<bool>
{
    private readonly IOperationRepository _opRepo;
    private readonly IAccountRepository _accRepo;
    private readonly Guid _operationId;
    private readonly decimal _newAmount;
    private readonly DateOnly _newDate;
    private readonly string? _newDesc;
    private readonly Guid _newCategoryId;

    public EditOperationCommand(
        IOperationRepository opRepo,
        IAccountRepository accRepo,
        Guid operationId,
        decimal newAmount,
        DateOnly newDate,
        string? newDesc,
        Guid newCategoryId)
    {
        _opRepo = opRepo;
        _accRepo = accRepo;
        _operationId = operationId;
        _newAmount = newAmount;
        _newDate = newDate;
        _newDesc = newDesc;
        _newCategoryId = newCategoryId;
    }

    public async Task<bool> Execute(CancellationToken ct = default)
    {
        var op = await _opRepo.GetByIdAsync(_operationId, ct);
        if (op is null)
            return false;
        
        var acc = await _accRepo.GetByIdAsync(op.AccountId, ct);
        if (acc is null)
            return false;
        
        acc.Balance -= op.Amount;
        
        op.Amount = _newAmount;
        op.Description = _newDesc;
        op.CategoryId = _newCategoryId;
        op.OccurredOn = _newDate;
        
        acc.Balance += op.Amount;

        await _accRepo.UpdateAsync(acc, ct);
        await _opRepo.UpdateAsync(op, ct);

        await _accRepo.SaveChangesAsync(ct);
        await _opRepo.SaveChangesAsync(ct);

        return true;
    }
}