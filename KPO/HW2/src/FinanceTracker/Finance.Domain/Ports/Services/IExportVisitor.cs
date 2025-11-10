using Finance.Domain.Entities;

namespace Finance.Domain.Ports.Services;

public interface IExportVisitor
{
    Task VisitAsync(User user,
        IEnumerable<BankAccount> accounts,
        IEnumerable<Category> categories,
        IEnumerable<Operation> operations,
        string path,
        CancellationToken ct = default);
}