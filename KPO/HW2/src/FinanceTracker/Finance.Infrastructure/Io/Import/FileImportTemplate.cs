using Finance.Domain.Ports.Services;

namespace Finance.Infrastructure.Io.Import;

public abstract class FileImportTemplate : IImportService {
    public async Task ImportAsync(string path, CancellationToken ct = default) {
        var raw = await File.ReadAllTextAsync(path, ct);
        var dto = Parse(raw); Validate(dto); await PersistAsync(dto, ct);
    }
    protected abstract ImportDto Parse(string raw);
    protected virtual void Validate(ImportDto dto) { }
    protected abstract Task PersistAsync(ImportDto dto, CancellationToken ct);
}
public sealed record ImportDto(
    string UserEmail, string UserName,
    IReadOnlyList<(string Name, decimal Initial)> Accounts,
    IReadOnlyList<(string Name, string Type)> Categories,
    IReadOnlyList<(string Account, string Category, string Type, decimal Amount, DateOnly Date, string? Desc)> Operations
);