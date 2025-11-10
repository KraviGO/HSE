using Finance.Application.Abstractions;
using Finance.Domain.Entities;
using IOperationRepository = Finance.Domain.Ports.Repositories.IOperationRepository;

namespace Finance.Application.Commands.Operations;

public sealed class ListOperationsCommand : ICommand<IReadOnlyList<Operation>>
{
    private readonly IOperationRepository _repo;
    private readonly Guid _userId;
    private readonly DateOnly? _from;
    private readonly DateOnly? _to;

    public ListOperationsCommand(IOperationRepository repo, Guid userId, DateOnly? from = null, DateOnly? to = null)
    { _repo = repo; _userId = userId; _from = from; _to = to; }

    public async Task<IReadOnlyList<Operation>> Execute(CancellationToken ct = default)
    {
        if (_from is { } f && _to is { } t)
            return (await _repo.GetByPeriodAsync(_userId, f, t, ct)).ToList();
        return (await _repo.GetByUserAsync(_userId, ct)).ToList();
    }
}