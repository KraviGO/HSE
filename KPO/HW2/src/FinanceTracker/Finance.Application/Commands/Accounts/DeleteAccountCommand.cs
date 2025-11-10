using Finance.Application.Abstractions;
using Finance.Domain.Ports.Repositories;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Commands.Accounts;

public sealed class DeleteAccountCommand : ICommand<bool>
{
    private readonly IAccountRepository _accRepo;
    private readonly IOperationRepository _opRepo;
    private readonly Guid _accountId;

    public DeleteAccountCommand(IAccountRepository accRepo, IOperationRepository opRepo, Guid accountId)
    { _accRepo = accRepo; _opRepo = opRepo; _accountId = accountId; }

    public async Task<bool> Execute(CancellationToken ct = default)
    {
        var ops = await _opRepo.GetByAccountAsync(_accountId, ct);
        foreach (var op in ops) await _opRepo.DeleteAsync(op.Id, ct);

        return await _accRepo.DeleteAsync(_accountId, ct);
    }
}