using Finance.Application.Abstractions;
using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Commands.Operations;

public sealed class DeleteOperationCommand : ICommand<bool>
{
    private readonly IOperationRepository _opRepo;
    private readonly IAccountRepository _accRepo;
    private readonly Guid _operationId;

    public DeleteOperationCommand(
        IOperationRepository opRepo,
        IAccountRepository accRepo,
        Guid operationId)
    {
        _opRepo = opRepo;
        _accRepo = accRepo;
        _operationId = operationId;
    }

    public async Task<bool> Execute(CancellationToken ct = default)
    {
        var op = await _opRepo.GetByIdAsync(_operationId, ct);
        if (op is null) return false;
        
        var acc = await _accRepo.GetByIdAsync(op.AccountId, ct);
        if (acc is not null)
        {
            acc.Balance -= op.Amount;
            await _accRepo.UpdateAsync(acc, ct);
            await _accRepo.SaveChangesAsync(ct);
        }

        return await _opRepo.DeleteAsync(_operationId, ct);
    }
}
