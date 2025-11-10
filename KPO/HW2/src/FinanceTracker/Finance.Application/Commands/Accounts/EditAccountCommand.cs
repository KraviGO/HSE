using Finance.Application.Abstractions;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Accounts;

public sealed class EditAccountCommand : ICommand<bool>
{
    private readonly IAccountRepository _repo;
    private readonly Guid _accountId;
    private readonly string _newName;

    public EditAccountCommand(IAccountRepository repo, Guid accountId, string newName)
    { _repo = repo; _accountId = accountId; _newName = newName; }

    public async Task<bool> Execute(CancellationToken ct = default)
    {
        var acc = await _repo.GetByIdAsync(_accountId, ct);
        if (acc is null) return false;
        if (string.IsNullOrWhiteSpace(_newName)) return false;
        
        typeof(Finance.Domain.Entities.BankAccount)
            .GetProperty("Name")!.SetValue(acc, _newName);
        await _repo.UpdateAsync(acc, ct);
        return true;
    }
}