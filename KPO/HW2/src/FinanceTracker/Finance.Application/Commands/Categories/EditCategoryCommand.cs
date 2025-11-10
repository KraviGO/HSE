using Finance.Application.Abstractions;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Categories;

public sealed class EditCategoryCommand : ICommand<bool>
{
    private readonly ICategoryRepository _repo;
    private readonly Guid _categoryId;
    private readonly string _newName;
    private readonly bool _newIsIncome;

    public EditCategoryCommand(ICategoryRepository repo, Guid categoryId, string newName, bool newIsIncome)
    {
        _repo = repo;
        _categoryId = categoryId;
        _newName = newName.Trim();
        _newIsIncome = newIsIncome;
    }

    public async Task<bool> Execute(CancellationToken ct = default)
    {
        if (string.IsNullOrWhiteSpace(_newName))
            throw new ArgumentException("Category name is required");

        var cat = await _repo.GetByIdAsync(_categoryId, ct)
                  ?? throw new InvalidOperationException("Category not found");

        cat.Name = _newName;
        cat.IsIncome = _newIsIncome;

        await _repo.UpdateAsync(cat, ct);
        await _repo.SaveChangesAsync(ct);
        return true;
    }
}