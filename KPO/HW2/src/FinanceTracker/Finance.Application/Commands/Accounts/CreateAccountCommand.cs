using Finance.Domain.Factories;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Accounts;

public sealed class CreateAccountCommand : Finance.Application.Abstractions.ICommand<Guid> {
    private readonly IAccountRepository _repo;
    private readonly Guid _userId; private readonly string _name; private readonly decimal _initial;
    
    public CreateAccountCommand(IAccountRepository repo, Guid userId, string name, decimal initial = 0m) {
        _repo = repo; _userId = userId; _name = name; _initial = initial;
    }
    
    public async Task<Guid> Execute(CancellationToken ct = default) {
        var acc = BankAccountFactory.Create(_userId, _name, _initial);
        await _repo.AddAsync(acc, ct);
        return acc.Id;
    }
}