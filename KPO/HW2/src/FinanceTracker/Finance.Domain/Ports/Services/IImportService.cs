namespace Finance.Domain.Ports.Services;
public interface IImportService
{
    Task ImportAsync(string path, CancellationToken ct = default);
}