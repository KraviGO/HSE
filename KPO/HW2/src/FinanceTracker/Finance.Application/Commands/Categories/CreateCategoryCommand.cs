using Finance.Application.Abstractions;
using Finance.Domain.Entities;
using Finance.Domain.Ports.Repositories;

namespace Finance.Application.Commands.Categories;

public sealed class CreateCategoryCommand : ICommand<Guid>
{
    private readonly ICategoryRepository _repo;
    private readonly Guid _userId;
    private readonly string _name;
    private readonly bool _isIncome;

    public CreateCategoryCommand(ICategoryRepository repo, Guid userId, string name, bool isIncome)
    {
        _repo = repo;
        _userId = userId;
        _name = name.Trim();
        _isIncome = isIncome;
    }

    public async Task<Guid> Execute(CancellationToken ct = default)
    {
        if (string.IsNullOrWhiteSpace(_name))
            throw new ArgumentException("Category name is required");
        
        var existing = await _repo.GetByUserAsync(_userId, ct);
        if (existing.Any(c => string.Equals(c.Name, _name, StringComparison.OrdinalIgnoreCase)))
            throw new InvalidOperationException("Category with the same name already exists.");

        var cat = new Category
        {
            Id = Guid.NewGuid(),
            UserId = _userId,
            Name = _name,
            IsIncome = _isIncome
        };

        await _repo.AddAsync(cat, ct);
        await _repo.SaveChangesAsync(ct);
        return cat.Id;
    }
}