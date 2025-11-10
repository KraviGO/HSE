using Finance.Application.Abstractions;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Categories;

public sealed class ListCategoriesCommand : ICommand<IReadOnlyList<Category>>
{
    private readonly ICategoryRepository _repo;
    private readonly Guid _userId;

    public ListCategoriesCommand(ICategoryRepository repo, Guid userId)
    {
        _repo = repo;
        _userId = userId;
    }

    public async Task<IReadOnlyList<Category>> Execute(CancellationToken ct = default)
    {
        var all = await _repo.GetByUserAsync(_userId, ct);
        return all.OrderBy(c => (c.IsIncome ? 0 : 1)) // доходные первыми
            .ThenBy(c => c.Name)
            .ToList();
    }
}