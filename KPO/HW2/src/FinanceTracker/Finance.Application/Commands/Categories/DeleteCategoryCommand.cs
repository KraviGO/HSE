using Finance.Application.Abstractions;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Categories;

public sealed class DeleteCategoryCommand : ICommand<bool>
{
    private readonly ICategoryRepository _repo;
    private readonly Guid _categoryId;

    public DeleteCategoryCommand(ICategoryRepository repo, Guid categoryId)
    {
        _repo = repo;
        _categoryId = categoryId;
    }

    public async Task<bool> Execute(CancellationToken ct = default)
    {
        var ok = await _repo.DeleteAsync(_categoryId, ct);
        if (ok) await _repo.SaveChangesAsync(ct);
        return ok;
    }
}