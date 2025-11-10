using Finance.Application.Abstractions;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Accounts;

public sealed class ListAccountsCommand : ICommand<IReadOnlyList<BankAccount>>
{
    private readonly IAccountRepository _repo;
    private readonly Guid _userId;
    public ListAccountsCommand(IAccountRepository repo, Guid userId)
    { _repo = repo; _userId = userId; }

    public async Task<IReadOnlyList<BankAccount>> Execute(CancellationToken ct = default)
        => (await _repo.GetByUserAsync(_userId, ct)).ToList();
}
